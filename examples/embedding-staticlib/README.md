Embedding / cross-compiling MicroPython as static library
=========================================================

This directory contains instructions how to integrate / cross-compile a static (.a archive file) library which easily can be *linked* into your *external* C or C++ application on platforms which support ELF (Executable and Linkable Format).

CMake is the current build tool for many arduino based boards, e.g. the ESP-IDF framework provides a high-quality CMake based cross-compile environment / toolchain which can easily integrated into another, external CMake project. This example will show howto use such an external cross-compiler tool-chain to build the static .a library for the specified hardware which then can be used for embedding (without copying / duplicating the MicroPython sources).

But first check if the executable **and** the static library will be built for your host system before cross-compiling.

Prerequisites
-------------

Before building, the `mpy-cross` tool and the *embed port* must be prepared.

1. Build `mpy-cross`

Change to project root and do the following:

```bash
cd ../../mpy-cross
make
```

2. Build the embed port (source files and headers) using:

```bash
cd ./examples/embedding-staticlib
make -f micropython_embed.mk
```

This will generate the `micropython_embed` directory which is a self-contained copy of MicroPython used to **build** the static library.

Building (host)
---------------

First check if the executable **and** the static library will be built for your host system by using the provided `CMakeLists.txt` configuration before cross-compiling.

```bash
cmake .
make
```

This will compile the `main.c` into the host-executable `embed` binary **and** the `libmicropython.a` static library archive file. If everything compiled fine, you can continue with cross-compiling.

Cross-Compiling
---------------

CMake can be adjusted to use a different compiler toolchain easily (using GNU make is a torture) by providing a simple external toolchain config file with the `-DCMAKE_TOOLCHAIN_FILE=settings.cmake`. This example provides `riscv32-cross.cmake` which uses the current release (v6.0.1) of Espressif ESP-IDF with pre-selected RISC-V32 (ESP32-C3) toolchain.

> [!INFO]
> Change the toolchain / paths in this file to adjust settings.

```bash
cmake .
make
```

This will compile the `libmicropython.a` static library for the selected architecture.

```bash
make install
```

