# ESP32-C3 Embedding (RISC-V RV32IMC)

This example builds MicroPython as a static library (`libmicropython.a`) for
the ESP32-C3 microcontroller.  It uses CMake together with the ESP-IDF RISC-V
cross-compilation toolchain.

## Dependencies

ESP-IDF must be installed using the
[Espressif IDF installer](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/get-started/)
before building.  The `riscv32-esp-elf` cross-compiler is installed by the IDF
installer under `~/.espressif/tools/riscv32-esp-elf/`.  CMake 3.16 or later is
also required and is available through the IDF installer or your distribution's
package manager.

## Implementation details

`mpconfigport.h` enables `MICROPY_PY_MATH`, `MICROPY_PY_JSON`, and
`MICROPY_EMBED_EXEC_STR_FUNCTION`.  `MICROPY_NLR_SETJMP` and
`MICROPY_GCREGS_SETJMP` are enabled for setjmp-based exception handling and
GC register scanning, which are required on the ESP32-C3's RISC-V core.

`CMakeLists.txt` compiles all embed-package `.c` sources into a static library
with `-O3` and installs the result to `/usr/local/lib/esp32c3` with the public
header to `/usr/local/include/esp32c3`.

`riscv32-cross.cmake.tpl` is the CMake toolchain template.
`adjust-cross-build.sh` queries the installed `riscv32-esp-elf` toolchain
version and writes the resolved `riscv32-cross.cmake` from that template
before the CMake configure step.

## Building

```bash
# 1. Generate the MicroPython embed package (run from within this directory)
make -f micropython_embed.mk

# 2. Resolve and write the CMake toolchain file
./adjust-cross-build.sh

# 3. Configure and build
cmake -B build -DCMAKE_TOOLCHAIN_FILE=./riscv32-cross.cmake .
cmake --build build

# 4. (Optional) Install the library and header
cmake --install build
```

The resulting `build/libmicropython.a` can be linked into an ESP32-C3
application alongside `micropython_embed/port/micropython_embed.h`.
