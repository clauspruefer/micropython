# MicroPython Embedding Examples

This directory contains examples demonstrating how to embed MicroPython as a
library into a host application.  The examples cover two complementary
approaches.

## Sub-directories

### `linux-fc/` — Function-call embedding on Linux

Demonstrates the **Boost.Python-style function-call API** introduced by
`MICROPY_EMBED_EXEC_STR_FUNCTION` / `mp_embed_exec_string_function`.  A C
host compiles and executes a Python snippet, then calls a named Python
function with a string argument and receives the return value back in C — all
without a filesystem or REPL.  This example is ideal for pre-testing
MicroPython code on Linux before deploying to a microcontroller.

See [`linux-fc/README.md`](linux-fc/README.md) for build instructions.

### `esp32/` — Cross-compiled embedding for ESP32 microcontrollers

Demonstrates a **generic CMake cross-compilation concept** that targets
real microcontroller hardware.  The build system relies on toolchain files
that invoke the manufacturer-supplied compiler, linker, and startup code
(bootloader, RTOS) shipped with the ESP-IDF framework.  Using the vendor
framework directly minimises the risk of incorrect hardware configuration
(clock speeds, memory maps, peripheral drivers) that would arise from
maintaining a separate, hand-crafted toolchain setup.

Currently two ESP32 variants are supported:

| Sub-directory | MCU | Architecture |
|---|---|---|
| [`esp32/s3/`](esp32/s3/README.md) | ESP32-S3 | Xtensa LX7 |
| [`esp32/c3/`](esp32/c3/README.md) | ESP32-C3 | RISC-V |

See [`esp32/README.md`](esp32/README.md) for an overview and
[`esp32/s3/README.md`](esp32/s3/README.md) /
[`esp32/c3/README.md`](esp32/c3/README.md) for per-variant build instructions.

## `mpconfigport.h` templates

Each sub-example ships its own `mpconfigport.h` that selects the MicroPython
modules and feature flags appropriate for the target.  These files are
intended as starting-point **templates** — you can copy and adjust them for
your own platform.  Note that the ESP32 variants require additional
hardware-specific settings (object representation, long-integer
implementation, NLR strategy) that differ from the Linux variant; review
those settings carefully before porting to a new microcontroller.
