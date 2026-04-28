Embedding / Cross-Compiling MicroPython as a Static Library
===========================================================

This directory contains instructions on how to build MicroPython as a static library (`.a` archive) that can easily be *linked* into an *external* C or C++ application, including for cross-compiled target platforms that support static libraries. Using a pre-built static library avoids copying or duplicating the MicroPython sources into the consuming project.

CMake is the standard build tool for many Arduino-based boards. For example, the ESP-IDF framework provides a high-quality, CMake-based cross-compilation environment and toolchain that can be integrated easily into another build system.

Before cross-compiling, first verify that both the executable **and** the static library can be built for your host system.

Prerequisites
-------------

Before building, the `mpy-cross` tool and the *embed port* must be prepared.

1. Build `mpy-cross`

   Change to the project root and run:

   ```bash
   cd ../../mpy-cross
   make
   ```

2. Build the embed port (source files and headers)

   ```bash
   cd ./examples/embedding-staticlib
   make -f micropython_embed.mk
   ```

This will generate the `micropython_embed` directory which is a self-contained copy of MicroPython used to **build** the static library.

Building for the Host System
----------------------------

First, verify that the executable **and** the static library build correctly for your host system using the provided `CMakeLists.txt` configuration.

```bash
cmake .
make
```

This compiles `main.c` into the host executable `embed` **and** creates the static library archive `libmicropython.a`. If everything compiles successfully, you can continue with cross-compiling.

Cross-Compiling
---------------

CMake can easily be configured to use a different compiler toolchain by providing an external toolchain configuration file with `-DCMAKE_TOOLCHAIN_FILE=<toolchain-file>`. This example provides `riscv32-cross.cmake` which uses the current release (v6.0.1) of Espressif ESP-IDF with a pre-selected RISC-V32 (ESP32-C3) toolchain.

> [!NOTE]
> Adjust the toolchain and path settings in `riscv32-cross.cmake` as needed.

```bash
cmake -DCMAKE_TOOLCHAIN_FILE=riscv32-cross.cmake .
make
```

This builds the `libmicropython.a` static library for the selected target architecture.

```bash
make install
```

The previous command installs the library **and** the MicroPython C header files in the following locations:

1. Library: `/usr/local/lib/esp32c3/libmicropython.a`
2. Header: `/usr/local/include/esp32c3/micropython_embed.h`

How to integrate the static library into an ESP-IDF C++ project is described here: https://github.com/WEBcodeX1/micropython-as.
