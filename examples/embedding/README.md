# MicroPython Embedding Examples

This directory contains examples showing how to embed MicroPython as a static
library into a host application.  Two approaches are covered.

## Function-call embedding (`linux-fc`)

The `linux-fc` example shows how to use the Boost.Python-style function-call
API provided by the MicroPython embed port.  Enabling
`MICROPY_EMBED_EXEC_STR_FUNCTION` in `mpconfigport.h` exposes the
`mp_embed_exec_string_function` function, which allows a C host to compile and
execute a Python snippet, call a named Python function with a string argument,
and receive the return value back as a C string — without a filesystem, REPL,
or separate interpreter process.  This pattern is applicable to any platform,
including Linux and bare-metal microcontrollers.

See [`linux-fc/README.md`](linux-fc/README.md) for build instructions.

## CMake cross-compilation for microcontrollers (`esp32`)

The `esp32` subdirectory shows a generic CMake cross-compilation approach for
embedding MicroPython on microcontroller hardware.  The concept is to build
directly inside the manufacturer's SDK — in this case the Espressif ESP-IDF
framework — so that the vendor-supplied toolchain, linker scripts, bootloader,
startup code, and RTOS integration are used as-is.  This avoids having to
replicate or second-guess hardware configuration (clock trees, memory maps,
peripheral initialisation) that the SDK already provides correctly.  The same
CMake toolchain-file pattern is applicable to any SDK that supports CMake.

See [`esp32/README.md`](esp32/README.md) for an overview and the per-variant
READMEs for step-by-step build instructions.

## `mpconfigport.h` configuration

Every sub-example ships its own `mpconfigport.h` that selects the MicroPython
feature flags and modules appropriate for the target.  These files are
intended as starting-point templates.  The ESP32 variants contain additional
hardware-specific settings — object representation, long-integer
implementation, and NLR/GC register strategy — that do not apply to Linux.
These settings must be reviewed and adjusted whenever the example is ported to
a different microcontroller architecture.
