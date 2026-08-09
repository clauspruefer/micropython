# Embedding MicroPython in an ESP-IDF Project (ESP32-S3)

This guide describes how to build MicroPython as a static library and integrate it
into an ESP-IDF project targeting the ESP32-S3.

---

## Prerequisites

### Toolchain

A recent GCC/G++ version is strongly advisable.  Using **gcc-14** and **g++-14**
is recommended to benefit from the latest optimisations and standard-compliance
improvements.

Install them on Debian/Ubuntu:

```bash
sudo apt install gcc-14 g++-14
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 100
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-14 100
```

### ESP-IDF

Clone ESP-IDF and run its installer, then activate the environment for the current
shell session.  The steps below use `~/esp/esp-idf` as the installation directory;
adjust the path to suit your setup.

```bash
# 1. Clone the ESP-IDF repository (use the release branch that matches your target):
git clone --recursive https://github.com/espressif/esp-idf.git ~/esp/esp-idf
cd ~/esp/esp-idf

# 2. Run the installer – downloads the Xtensa/RISC-V toolchains and tools:
./install.sh esp32s3

# 3. Activate the ESP-IDF environment in the current shell session:
. ./export.sh
```

> **Note:** The `. ./export.sh` step must be repeated in every new shell session
> before using `idf.py`, or added to your shell's startup file.

---

## MicroPython Configuration

### `mpconfigport.h`

The `mpconfigport.h` file controls which MicroPython features are compiled into the
static library.  Place it in your project root (or a dedicated config directory) and
adjust the defines to match your requirements.  Key settings include:

- `MICROPY_CONFIG_ROM_LEVEL` – selects the base feature set; set to
  `MICROPY_CONFIG_ROM_LEVEL_MINIMUM` to start lean and add only what you need.
- `MICROPY_ENABLE_COMPILER` – set to `1` to include the bytecode compiler so
  scripts can be passed as source strings at runtime.
- `MICROPY_ENABLE_GC` – set to `1` to enable the garbage collector; required for
  most real-world use cases.
- `MICROPY_PY_GC` – set to `1` to expose the `gc` module to Python code.
- `MICROPY_PY_SYS` – set to `0` to exclude the `sys` module when it is not needed,
  reducing code size.
- `MICROPY_HEAP_SIZE` – define the size (in bytes) of the MicroPython GC heap that
  your application will provide.

A minimal starting configuration:

```c
/* mpconfigport.h */
#include <port/mpconfigport_common.h>

#define MICROPY_CONFIG_ROM_LEVEL    (MICROPY_CONFIG_ROM_LEVEL_MINIMUM)
#define MICROPY_ENABLE_COMPILER     (1)
#define MICROPY_ENABLE_GC           (1)
#define MICROPY_PY_GC               (1)
#define MICROPY_PY_SYS              (0)
```

### CMake Settings

When integrating the MicroPython embed output into an ESP-IDF component, the
`CMakeLists.txt` of your component must be configured correctly.  Important points:

- Add the `micropython_embed` source directory to `SRCS` using a glob expression,
  e.g. `file(GLOB_RECURSE MICROPYTHON_SRCS "micropython_embed/**/*.c")`.
- Add the following include directories to `INCLUDE_DIRS`:
  - The project root (where `mpconfigport.h` lives).
  - `micropython_embed` – the top-level generated directory.
  - `micropython_embed/port` – contains `micropython_embed.h` and HAL headers.
- Set `WHOLE_ARCHIVE` or `LINK_WHOLE_ARCHIVE` when registering the component if
  you encounter missing symbol errors at link time caused by linker garbage
  collection.
- Pass `-DMICROPY_PY_THREAD=0` (or the relevant defines) via `COMPILE_OPTIONS` if
  you need to override feature flags at the CMake level rather than in
  `mpconfigport.h`.

---

## Building the MicroPython Embed Package

From the MicroPython repository root, generate the self-contained embed source
package for your configuration:

```bash
cd /path/to/micropython/examples/embedding
make -f micropython_embed.mk
```

This produces the `micropython_embed/` directory containing all `.c` and `.h` files
needed to build MicroPython – no other part of the MicroPython repository is
required from this point.

---

## Installing the Static Library and Headers into Your ESP-IDF Project

The recommended approach is to copy the generated embed package directly into your
ESP-IDF project as a component.

```bash
# Assume your ESP-IDF project lives at ~/my_project
PROJECT_DIR=~/my_project

# 1. Create the component directory:
mkdir -p ${PROJECT_DIR}/components/micropython

# 2. Copy the generated embed sources:
cp -r micropython_embed ${PROJECT_DIR}/components/micropython/

# 3. Copy your mpconfigport.h into the component directory
#    (the build system expects to find it at the root of the component):
cp mpconfigport.h ${PROJECT_DIR}/components/micropython/

# 4. Create the component CMakeLists.txt (see the Reference Implementation below).
```

After this step, the directory layout of the component should be:

```
components/micropython/
├── CMakeLists.txt          # see Reference Implementation
├── mpconfigport.h
└── micropython_embed/
    ├── port/
    │   ├── micropython_embed.h
    │   ├── mpconfigport_common.h
    │   └── ...
    ├── py/
    │   └── ...
    └── ...
```

---

## Reference Implementation

The following files show the minimal wiring needed to use the MicroPython component
from an ESP-IDF application.

### `components/micropython/CMakeLists.txt`

```cmake
# Collect all MicroPython embed C sources recursively.
file(GLOB_RECURSE MICROPYTHON_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/micropython_embed/**/*.c"
)

idf_component_register(
    SRCS
        ${MICROPYTHON_SRCS}
    INCLUDE_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/micropython_embed"
        "${CMAKE_CURRENT_SOURCE_DIR}/micropython_embed/port"
    REQUIRES
        # Add any ESP-IDF components your MicroPython modules depend on, e.g.:
        # esp_timer freertos
)
```

### `main/main.c`

```c
#include <stdio.h>
#include "port/micropython_embed.h"

/* MicroPython GC heap – adjust the size to match your application's RAM budget. */
static char mp_heap[64 * 1024];

/* A simple Python script executed at startup. */
static const char *script =
    "print('Hello from MicroPython on ESP32-S3!')\n"
    "import gc\n"
    "gc.collect()\n"
    "print('Free heap after GC:', gc.mem_free())\n"
;

void app_main(void)
{
    /*
     * Initialise MicroPython.
     * The third argument is the address of a stack variable; MicroPython uses it
     * to determine the top of the C stack for GC root scanning.
     */
    int stack_top;
    mp_embed_init(mp_heap, sizeof(mp_heap), &stack_top);

    /* Execute the Python script (it is compiled to bytecode internally). */
    mp_embed_exec_str(script);

    /* Deinitialise MicroPython cleanly. */
    mp_embed_deinit();
}
```

### `main/CMakeLists.txt`

```cmake
idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES micropython
)
```

### `CMakeLists.txt` (project root)

```cmake
cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(my_micropython_app)
```

---

## Building and Flashing

With the ESP-IDF environment active (`. ~/esp/esp-idf/export.sh`) and the target
set, build and flash the project:

```bash
cd ~/my_project

# Configure the target chip once:
idf.py set-target esp32s3

# Build:
idf.py build

# Flash and open the serial monitor (replace /dev/ttyUSB0 with your port):
idf.py -p /dev/ttyUSB0 flash monitor
```

Press `Ctrl-]` to exit the serial monitor.
