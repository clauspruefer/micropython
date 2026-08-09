# ESP32-S3 Embedding (Xtensa LX7)

This example builds *MicroPython* as a static library (`libmicropython.a`) for
the **ESP32-S3** microcontroller using *CMake* and the *ESP-IDF* Xtensa toolchain
and shows how to integrate into an ESP-IDF project / component easily.

## Dependencies

The following tools and packages are required to build this example.

### ESP-IDF / Dependencies

1. **Python3 Virtual Environment**

```
apt-get install python3-venv
```

2. **ESP-IDF Framework**

Clone repository and install crosscompiler toolchain.

```
git clone https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh
```

3. **CMake and Compiler Environment**

Current Debian or Ubuntu ships with gcc-14 and g++-14, for modern C++23 practices we recommend using
the latest available versions. Install the required build tools with:

```
apt-get install cmake gcc-14 g++-14 build-essential
```

4. **MicroPython Source Tree**

Also clone the MicroPython source tree, as it is required for the embed build step.

```
git clone https://github.com/micropython/micropython.git
```

## Implementation Details

The following hardware ...

- `MICROPY_LONGINT_IMPL` to `MICROPY_LONGINT_IMPL_MPZ`

MicroPython features enabled for this build:

- `MICROPY_PY_MATH`
- `MICROPY_PY_JSON`
- `MICROPY_EMBED_EXEC_STR_FUNCTION`

The compiler flags `-O3` for performance and `-mlongcalls` are enabled.
Set `-Og` for debugging to retain useful debug information while keeping
the code reasonably optimised.

## Building / Installing

1. Activate ESP-IDF environment.

```
cd src/esp-idf
. ./export.sh
```

2. Build embedding example

```
# Change to micropython embedding example
cd src/micropython/examples/embedding/esp32/s3/

# Generate a cmake compatible toolchain setup from current user environment
./adjust-cross-build.sh

# Prepare / generate micropython sources
make -f micropython_embed.mk

# Tell cmake to use the cross-compiler settings
cmake -DCMAKE_TOOLCHAIN_FILE=xtensa-cross.cmake

# Build Source
make -j2

# Install
sudo make install
```

3. Check installation status

The static library and the header file is installed here:

- Library: `/usr/local/lib/esp32s3/libmicropython.a`
- Header: `/usr/local/include/esp32s3/micropython_embed.h`

> [!NOTE]
> We insist on hardcoded paths despite other proposals to do this in user environment; if a path is fixed there are no misunderstandings
> and a correct package managing system like apt / dpkg builds the installation layer: classical, approved, no-misunderstanding!

## ESP-IDF C++ Project Integration

Once the static library is installed, integrating *MicroPython* into an ESP-IDF component requires
two steps: linking the library in the component's `CMakeLists.txt` and exposing the C API to your
C++ source files via a wrapper header.

You should be familiar with the ESP-IDF Build System. Here is a good start on how to set up a basic project:
https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-guides/build-system.html

### Linking The Static Library

Register your ESP-IDF component as usual with `idf_component_register`, then declare
`libmicropython.a` as a pre-built imported static library and link it privately to the component.
This makes all MicroPython C-API symbols available to your component's source files without
leaking them into other components.

```
idf_component_register(
    SRCS Micropython.cpp
    PRIV_REQUIRES main
    INCLUDE_DIRS "."
)

add_library(micropython_lib STATIC IMPORTED)
set_property(TARGET micropython_lib PROPERTY IMPORTED_LOCATION /usr/local/lib/esp32s3/libmicropython.a)

target_link_libraries(${COMPONENT_LIB} PRIVATE micropython_lib)
```

### C++ Wrapper Header

Because MicroPython's public API is written in C, it must be wrapped in an `extern "C"` block
when included from C++ translation units. Create a dedicated header (e.g. `Micropython.h`) in
your component that wraps the installed embed header. Any `.cpp` file that needs to call
MicroPython's C-API then includes this wrapper instead of the embed header directly.

```
#pragma once

extern "C" {
#include "/usr/local/include/esp32s3/micropython_embed.h"
}
```

### ESP-IDF Reference Implementation

The classic PONG game on the ESP32-S3, featuring browser-app paddle control, uses *MicroPython* as the game rendering engine and
a high-speed C++ web application server to handle paddle movement in real-time.

Project on GitHub: https://github.com/WEBcodeX1/micropython-as
