Example of embedding MicroPython as a static library with cmake
==============================================================

This directory contains an example of how to embed MicroPython in a C
application using cmake, including cross-compilation support for RISC-V targets
such as the Espressif ESP32-C3.

A C application is represented by the file `main.c`.  It executes two Python
scripts that demonstrate using the `json` and `math` modules.

Enabling the math module
------------------------

To use the MicroPython `math` module, two conditions must be met in
`mpconfigport.h`:

1. `MICROPY_FLOAT_IMPL` must be set to a non-`None` value.  This activates
   `MICROPY_PY_BUILTINS_FLOAT`, which `modmath.c` requires via the guard:

       #if MICROPY_PY_BUILTINS_FLOAT && MICROPY_PY_MATH

   Without this, even setting `MICROPY_PY_MATH (1)` will have no effect and
   `import math` will fail with an `ImportError` at runtime.

2. `MICROPY_PY_MATH` must be set to `(1)`.

The `mpconfigport.h` in this example uses:

    #define MICROPY_FLOAT_IMPL   (MICROPY_FLOAT_IMPL_DOUBLE)
    #define MICROPY_PY_MATH      (1)

Use `MICROPY_FLOAT_IMPL_FLOAT` for single-precision floats on memory-constrained
targets, or `MICROPY_FLOAT_IMPL_DOUBLE` for double-precision.

Building the example
--------------------

### Step 1: Generate the MicroPython embed package

The embed package is generated on the host (build) machine using the provided
makefile fragment.  This step uses the host C compiler and Python toolchain.

    $ make -f micropython_embed.mk

This creates the `micropython_embed/` directory containing all MicroPython
source and header files needed for the build.

### Step 2: Configure and build with cmake

#### Native (host) build

    $ cmake -B build
    $ cmake --build build

Run the resulting binary:

    $ ./build/embed_staticlib

#### Cross-compilation for RISC-V (ESP32-C3)

Ensure the `riscv32-esp-elf-gcc` cross-compiler from the Espressif esp-idf
toolchain is available in your `PATH`, then:

    $ cmake -B build -DCMAKE_TOOLCHAIN_FILE=riscv32-cross.cmake
    $ cmake --build build

If the cross-compiler is not in `PATH`, provide its location explicitly:

    $ cmake -B build \
        -DCMAKE_TOOLCHAIN_FILE=riscv32-cross.cmake \
        -DRISCV32_TOOLCHAIN_PATH=/path/to/esp-idf/.espressif/tools/riscv32-esp-elf/.../bin
    $ cmake --build build

The result is a `libmicropython.a` static library and an `embed_staticlib`
binary/object that you can link into your firmware.

Out of tree build
-----------------

This example is set up to work as part of the MicroPython source tree.  To use
it outside the tree, set `MICROPYTHON_TOP` in `micropython_embed.mk` to point
to your MicroPython repository clone (which may be a git submodule in your
project).
