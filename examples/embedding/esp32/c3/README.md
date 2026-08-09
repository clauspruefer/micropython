# ESP32-C3 Embedding (RISC-V RV32IMC)

This example builds MicroPython as a static library (`libmicropython.a`) for
the **ESP32-C3** microcontroller using CMake and the ESP-IDF RISC-V toolchain.

## Dependencies

| Dependency | Notes |
|---|---|
| **ESP-IDF** | Install via the [Espressif IDF installer](https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/get-started/). The `riscv32-esp-elf` cross-compiler must be available under `~/.espressif/tools/riscv32-esp-elf/`. |
| **CMake** ≥ 3.16 | Available via your distro's package manager or the IDF installer. |
| **MicroPython source** | This example lives inside the MicroPython source tree. |

## Implementation details

* `mpconfigport.h` — enables `MICROPY_PY_MATH`, `MICROPY_PY_JSON`, and
  `MICROPY_EMBED_EXEC_STR_FUNCTION`; selects `MICROPY_NLR_SETJMP` and
  `MICROPY_GCREGS_SETJMP` for setjmp-based exception handling on RISC-V.
* `CMakeLists.txt` — compiles all embed-package `.c` sources into a static
  library with `-O3`; installs the library to `/usr/local/lib/esp32c3` and
  the public header to `/usr/local/include/esp32c3`.
* `riscv32-cross.cmake.tpl` / `adjust-cross-build.sh` — CMake toolchain
  template; `adjust-cross-build.sh` auto-detects the installed toolchain
  version and writes the final `riscv32-cross.cmake` file.

## Steps to compile

```bash
# 1. Generate the MicroPython embed package (run from the repo root)
cd examples/embedding/esp32/c3
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
ESP32-C3 application together with `micropython_embed/port/micropython_embed.h`.
