# ESP32-S3 Embedding (Xtensa LX7)

This example builds *MicroPython* as a static library (`libmicropython.a`) for
the **ESP32-S3** microcontroller using *CMake* and the *ESP-IDF* Xtensa toolchain.

## Dependencies

The following tools must be installed before building.

- **ESP-IDF** — install via the
  [Espressif IDF installer](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/).
  The `xtensa-esp-elf` cross-compiler must be present under `~/.espressif/tools/xtensa-esp-elf/`.
- **CMake 3.16 or later** — available through your distribution's package manager or the IDF installer.
- **MicroPython source tree** — this example is part of the MicroPython repository and must be built from within it.

## Implementation details

`mpconfigport.h` enables `MICROPY_PY_MATH`, `MICROPY_PY_JSON`, and
`MICROPY_EMBED_EXEC_STR_FUNCTION`.  It sets `MICROPY_OBJ_REPR_A` and
`MICROPY_LONGINT_IMPL_MPZ` for the Xtensa ABI, and enables
`MICROPY_NLR_SETJMP` together with `MICROPY_GCREGS_SETJMP` for
setjmp-based exception handling, which is required on Xtensa targets where
the compiler does not expose machine registers to the GC scanner directly.

`CMakeLists.txt` collects all embed-package `.c` sources and compiles them
into a static library with `-O3 -mlongcalls`.  The installed artefacts are
placed in `/usr/local/lib/esp32s3` (library) and
`/usr/local/include/esp32s3` (public header).

`xtensa-cross.cmake.tpl` is a CMake toolchain template.
`adjust-cross-build.sh` auto-detects the installed `xtensa-esp-elf` toolchain
version and writes the final `xtensa-cross.cmake` from the template.

## Steps to compile

```bash
# 1. Generate the MicroPython embed package (run from within this directory)
make -f micropython_embed.mk

# 2. Prepare the CMake toolchain file

