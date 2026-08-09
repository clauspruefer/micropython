# MicroPython Embedding — ESP32

This directory contains exemplary engineering work showing how to embed
MicroPython as a static library into an application built with the
**ESP-IDF** framework using a **CMake-based cross-compilation** workflow.

## Concept

The build system uses CMake toolchain files to invoke the manufacturer-supplied
cross-compiler (Xtensa or RISC-V) that ships with ESP-IDF.  This approach
deliberately delegates startup code, linker scripts, bootloader, and RTOS
integration to the vendor framework so that hardware settings (clock
configuration, memory layout, peripheral drivers) are always consistent with
the officially supported board bring-up.

## Compiler settings

Both variants are compiled with `-O3` for maximum runtime performance on the
target MCU.  The following MicroPython modules are enabled in
`mpconfigport.h`:

| Module | Purpose |
|--------|---------|
| `MICROPY_PY_MATH` | Floating-point math (`math` module) |
| `MICROPY_PY_JSON` | JSON encode/decode (`json` module) |
| `MICROPY_EMBED_EXEC_STR_FUNCTION` | Boost.Python-style C→Python function call API |

`MICROPY_FLOAT_IMPL_FLOAT` (single-precision) is used to keep code-size and
stack-usage low on resource-constrained MCUs.

## Supported targets

| Sub-directory | MCU | ISA | Toolchain |
|---|---|---|---|
| [`s3/`](s3/README.md) | ESP32-S3 | Xtensa LX7 | `xtensa-esp-elf` |
| [`c3/`](c3/README.md) | ESP32-C3 | RISC-V (RV32IMC) | `riscv32-esp-elf` |

See the per-variant README for step-by-step build instructions.
