# MicroPython Embedding — ESP32

This directory contains exemplary engineering work showing how to embed
MicroPython as a static library into an application built with the Espressif
ESP-IDF framework, using a CMake-based cross-compilation workflow.

## Build concept

CMake toolchain files are used to invoke the manufacturer-supplied
cross-compiler that ships with ESP-IDF.  Startup code, linker scripts,
bootloader initialisation, and FreeRTOS integration are all provided by the
vendor SDK rather than maintained separately.  This ensures that hardware
settings such as clock configuration, memory layout, and peripheral drivers
are always consistent with the officially supported board bring-up, removing
a common source of subtle hardware-misconfiguration bugs.

## Compiler settings and enabled modules

Both variants are compiled with `-O3` for maximum runtime performance on the
target MCU.  Single-precision floating-point (`MICROPY_FLOAT_IMPL_FLOAT`) is
used to keep code size and stack consumption low on resource-constrained
hardware.

The following MicroPython modules are enabled in `mpconfigport.h`:

- `MICROPY_PY_MATH` — floating-point math functions (`math` module).
- `MICROPY_PY_JSON` — JSON serialisation and deserialisation (`json` module).
- `MICROPY_EMBED_EXEC_STR_FUNCTION` — Boost.Python-style C-to-Python
  function call API (`mp_embed_exec_string_function`).

## Supported targets

Two ESP32 variants are provided.

- [`s3/`](s3/README.md) — ESP32-S3 (Xtensa LX7, `xtensa-esp-elf` toolchain).
- [`c3/`](c3/README.md) — ESP32-C3 (RISC-V RV32IMC, `riscv32-esp-elf` toolchain).

Refer to the per-variant README for step-by-step build instructions and
dependency information.
