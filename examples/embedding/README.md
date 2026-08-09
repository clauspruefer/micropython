# MicroPython Embedding Examples

This directory contains examples that show how to embed MicroPython as a
static library into a host application.  Two complementary approaches are
covered.

## Function-call embedding (`linux-fc`)

The `linux-fc` example demonstrates a Boost.Python-style function-call API
built on top of the MicroPython embed port.  Enabling
`MICROPY_EMBED_EXEC_STR_FUNCTION` in `mpconfigport.h` exposes
`mp_embed_exec_string_function`, which allows a C host to compile and execute
a Python snippet, call a named Python function with a string argument, and
receive the return value back as a C string — all without a filesystem, REPL,
or separate interpreter process.  This makes it straightforward to drive
Python business logic from C in the same way that Boost.Python or pybind11
allow calling Python from C++, but against a minimal, statically-linked
runtime that is suitable for resource-constrained targets.

See [`linux-fc/README.md`](linux-fc/README.md) for build instructions.

## Generic CMake cross-compilation (`esp32`)

The `esp32` subdirectory demonstrates a generic CMake cross-compilation
concept that targets real microcontroller hardware.  The core idea is to
invoke the manufacturer-supplied compiler, linker, startup code, bootloader,
and RTOS integration that ship inside the vendor SDK (ESP-IDF) rather than
maintaining a separate, hand-crafted toolchain configuration.  Delegating
hardware bring-up to the vendor framework eliminates an entire class of
misconfiguration bugs — wrong clock speeds, incorrect memory maps, missing
peripheral initialisation — that would otherwise need to be discovered and
fixed manually.

Although the examples here target Espressif ESP32 microcontrollers
(ESP32-S3 and ESP32-C3), the same CMake toolchain-file pattern applies to any
manufacturer that ships a CMake-compatible SDK.

See [`esp32/README.md`](esp32/README.md) for an overview and the per-variant
READMEs for step-by-step build instructions.

## `mpconfigport.h` configuration templates

Every sub-example ships its own `mpconfigport.h` that selects the MicroPython
modules and feature flags appropriate for the target platform.  These files
are intended as starting-point templates that you can copy and adapt for your
own project.  The ESP32 variants require additional hardware-specific settings
— object representation, long-integer implementation, and NLR/GC register
strategy — that differ from the Linux variant.  Review those settings
carefully before porting to a new microcontroller architecture.
