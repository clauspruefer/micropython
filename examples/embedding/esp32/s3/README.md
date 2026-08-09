# ESP32-S3 Embedding (Xtensa LX7)

This example builds MicroPython as a static library (`libmicropython.a`) for
the **ESP32-S3** microcontroller using CMake and the ESP-IDF Xtensa toolchain.

## Dependencies

| Dependency | Notes |
|---|---|
| **ESP-IDF** | Install via the [Espressif IDF installer](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/). The `xtensa-esp-elf` cross-compiler must be available under `~/.espressif/tools/xtensa-esp-elf/`. |
| **CMake** ≥ 3.16 | Available via your distro's package manager or the IDF installer. |
| **MicroPython source** | This example lives inside the MicroPython source tree. |

## Implementation details

* `mpconfigport.h` — enables `MICROPY_PY_MATH`, `MICROPY_PY_JSON`, and
  `MICROPY_EMBED_EXEC_STR_FUNCTION`; sets `MICROPY_OBJ_REPR_A` and
  `MICROPY_LONGINT_IMPL_MPZ` for Xtensa; enables `MICROPY_NLR_SETJMP` and
  `MICROPY_GCREGS_SETJMP` for setjmp-based exception handling.
* `CMakeLists.txt` — compiles all embed-package `.c` sources into a static
  library with `-O3 -mlongcalls`; installs the library to
  `/usr/local/lib/esp32s3` and the public header to
  `/usr/local/include/esp32s3`.
* `xtensa-cross.cmake.tpl` / `adjust-cross-build.sh` — CMake toolchain
  template; `adjust-cross-build.sh` auto-detects the installed toolchain
  version and writes the final `xtensa-cross.cmake` file.

## Steps to compile

```bash
# 1. Generate the MicroPython embed package (run from the repo root)
cd examples/embedding/esp32/s3
make -f micropython_embed.mk

# 2. Prepare the CMake toolchain file
./adjust-cross-build.sh

# 3. Configure and build
cmake -B build -DCMAKE_TOOLCHAIN_FILE=./xtensa-cross.cmake .
cmake --build build

# 4. (Optional) Install the library and header for use by other projects
cmake --install build
```

After a successful build, `build/libmicropython.a` can be linked into your
ESP32-S3 application together with `micropython_embed/port/micropython_embed.h`.
