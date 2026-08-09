# ESP32-C3 Embedding (RISC-V RV32IMC)

This example builds MicroPython as a static library (`libmicropython.a`) for
the ESP32-C3 microcontroller using CMake and the ESP-IDF RISC-V toolchain.

## Dependencies

The following tools must be installed before building.

- **ESP-IDF** — install via the
  [Espressif IDF installer](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/get-started/).
  The `riscv32-esp-elf` cross-compiler must be present under
  `~/.espressif/tools/riscv32-esp-elf/`.
- **CMake 3.16 or later** — available through your distribution's package
  manager or the IDF installer.
- **MicroPython source tree** — this example is part of the MicroPython
  repository and must be built from within it.

## Implementation details

`mpconfigport.h` enables `MICROPY_PY_MATH`, `MICROPY_PY_JSON`, and
`MICROPY_EMBED_EXEC_STR_FUNCTION`.  It enables `MICROPY_NLR_SETJMP` and
`MICROPY_GCREGS_SETJMP` for setjmp-based exception handling and GC register
scanning, which is necessary on the ESP32-C3's RISC-V core.

`CMakeLists.txt` collects all embed-package `.c` sources and compiles them
into a static library with `-O3`.  The installed artefacts are placed in
`/usr/local/lib/esp32c3` (library) and `/usr/local/include/esp32c3` (public
header).

`riscv32-cross.cmake.tpl` is a CMake toolchain template.
`adjust-cross-build.sh` auto-detects the installed `riscv32-esp-elf` toolchain
version and writes the final `riscv32-cross.cmake` from the template.

## Steps to compile

```bash
# 1. Generate the MicroPython embed package (run from within this directory)
make -f micropython_embed.mk

# 2. Prepare the CMake toolchain file
./adjust-cross-build.sh

# 3. Configure and build
cmake -B build -DCMAKE_TOOLCHAIN_FILE=./riscv32-cross.cmake .
cmake --build build

# 4. (Optional) Install the library and header for use by other projects
cmake --install build
```

After a successful build, `build/libmicropython.a` can be linked into your
ESP32-C3 application together with
`micropython_embed/port/micropython_embed.h`.
