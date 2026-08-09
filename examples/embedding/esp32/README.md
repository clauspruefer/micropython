## MicroPython Embedding — ESP32

This directory demonstrates how to run and embed the MicroPython interpreter with absolute **main-loop independence**
within an *Espressif ESP-IDF* application. Instead of allowing *MicroPython* to seize the primary execution thread, the host
C/C++ application retains complete control over the main loop.

As a developer, you are fully responsible for initializing the interpreter instance, managing its lifecycle, and explicitly
handling garbage collection (GC) or resource allocation within your native firmware loop. 

Crucially, this architecture leverages MicroPython's C-API to invoke a single Python function directly from native C/C++ code.
This allows the host application to pass parameters, trigger precise Python logic at runtime, and receive the execution result back
as a native C/C++ variable seamlessly.

By building directly inside the manufacturer's SDK, all low-level hardware configuration (clock trees, memory mapping,
and FreeRTOS scheduling) is inherited as-is, ensuring a reliable, production-ready foundation for your custom C/C++ control loop.

## Compiler Settings

Both variants are compiled with `-O3` for maximum runtime performance. Single-precision floating-point (`MICROPY_FLOAT_IMPL_FLOAT`)
is selected to reduce code size and stack usage on microcontroller hardware.

The `mpconfigport.h` in each variant enables `MICROPY_PY_MATH` for floating-point math, `MICROPY_PY_JSON` for JSON serialisation, and
`MICROPY_EMBED_EXEC_STR_FUNCTION` for the Boost.Python-style C-to-Python function call API.

## Supported Targets

Two ESP32 variants are provided.

- [`s3/`](s3/README.md) — ESP32-S3, Xtensa LX7 core, built with the `xtensa-esp-elf` toolchain.
- [`c3/`](c3/README.md) — ESP32-C3, RISC-V RV32IMC core, built with the `riscv32-esp-elf` toolchain.

Refer to the per-variant README for dependencies and step-by-step build instructions.
