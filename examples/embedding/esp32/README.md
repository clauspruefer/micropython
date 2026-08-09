# MicroPython Embedding — ESP32

This directory shows how to embed MicroPython as a static library into an
application that is built with the Espressif ESP-IDF framework.  ESP-IDF is
Espressif's own comprehensive IoT development framework; it ships its own
CMake-based build system, cross-compiler toolchain, FreeRTOS integration,
bootloader, and hardware abstraction layer for all ESP32 variants.  Building
MicroPython inside this framework means that all hardware configuration —
clock trees, memory layout, peripheral drivers, startup sequences — is
inherited directly from the manufacturer-supplied SDK rather than replicated
by hand.  This is the approach recommended for production use because it
tracks Espressif's own board bring-up and removes a large category of
hardware-misconfiguration issues.

## Compiler settings

Both variants are compiled with `-O3` for maximum runtime performance.
Single-precision floating-point (`MICROPY_FLOAT_IMPL_FLOAT`) is selected to
reduce code size and stack usage on microcontroller hardware.

The `mpconfigport.h` in each variant enables `MICROPY_PY_MATH` for
floating-point math, `MICROPY_PY_JSON` for JSON serialisation, and
`MICROPY_EMBED_EXEC_STR_FUNCTION` for the Boost.Python-style C-to-Python
function call API.

## Supported targets

Two ESP32 variants are provided.

- [`s3/`](s3/README.md) — ESP32-S3, Xtensa LX7 core, built with the
  `xtensa-esp-elf` toolchain.
- [`c3/`](c3/README.md) — ESP32-C3, RISC-V RV32IMC core, built with the
  `riscv32-esp-elf` toolchain.

Refer to the per-variant README for dependencies and step-by-step build
instructions.
