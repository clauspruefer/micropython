# MicroPython Embedding Examples

This directory contains examples showing how to embed *MicroPython* in host applications.

- [`linux/`](linux/) — A simple embedding example for a standalone Linux C application.
- [`linux-fc/`](linux-fc/) — Demonstrates executing a MicroPython function directly from a C/C++ host application via a function-call API.
- [`esp32/`](esp32/) — CMake-based cross-compilation utilizing the ESP-IDF framework's native cross-compiler toolchain (C/C++).

## 1. CMake Cross-Compilation For Microcontrollers

This setup demonstrates a generic *CMake* cross-compilation pattern for
embedding *MicroPython* on microcontroller hardware. The core concept is to build
directly inside the manufacturer's SDK so that the vendor-supplied toolchain,
linker scripts, bootloader, startup code, and RTOS integration are used as-is.
This pattern is universally applicable to any SDK that supports *CMake*.

By establishing this architecture, developers achieve **main-loop independent**
*MicroPython* integration, allowing the host C/C++ application to maintain absolute
control over the primary execution cycle. Furthermore, this foundation seamlessly
supports the **Function-call Embedding** approach described below, enabling 
fine-grained, direct C/C++ control over *MicroPython* execution on any 
microcontroller target.

Currently, the `esp32` subdirectory serves as the initial example using the
*Espressif ESP-IDF* framework. More CMake-based examples for other architectures
will follow.

See the subdirectory-specific READMEs (e.g., [`esp32/README.md`](esp32/README.md))
for an overview and step-by-step build instructions.

## 2. Function-call Embedding (`linux-fc`)

The `linux-fc` example shows how to use a *Boost.Python*-style **function-call API**
which has been added recently to the *MicroPython* embed port. Enabling
`MICROPY_EMBED_EXEC_STR_FUNCTION` in `mpconfigport.h` exposes the
`mp_embed_exec_string_function` function, which allows a C host to execute a
named Python function with a string argument, and receive the return value back as
a C string — without a filesystem, REPL, or separate interpreter process.
This pattern is applicable to any platform, including Linux and bare-metal microcontrollers.

See [`linux-fc/README.md`](linux-fc/README.md) for build instructions.

## 3. `mpconfigport.h` Configuration

Every sub-example ships its own `mpconfigport.h` that selects the *MicroPython*
feature flags and modules appropriate for the target.  These files are
intended as starting-point templates.  The *ESP32* variants contain additional
hardware-specific settings — object representation, long-integer
implementation, and NLR/GC register strategy — that do not apply to Linux.
These settings must be reviewed and adjusted whenever the example is ported to
a different microcontroller architecture.
