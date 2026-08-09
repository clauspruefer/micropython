# Linux Function-Call Embedding (`linux-fc`)

This example demonstrates embedding MicroPython into a Linux host application
and calling a Python function from C using the
`mp_embed_exec_string_function` API.

## Function-call API and Boost.Python comparison

Enabling `MICROPY_EMBED_EXEC_STR_FUNCTION` in `mpconfigport.h` exposes the
`mp_embed_exec_string_function(name, arg)` function. This allows a C host to
execute Python code—whether source code, Python bytecode, or previously loaded
modules—and retrieve a defined, named callable. The host can then invoke this
callable with a single string argument and receive the return value back as a
C string.

This mirrors the pattern made popular by *Boost.Python* and *pybind11*, where C++
code calls into Python without spawning a separate interpreter process.  The
key distinction is that `mp_embed_exec_string_function` targets a minimal,
statically-linked MicroPython runtime with no dependency on a system *Python*
installation, making it practical on platforms where CPython is unavailable or
too large.

A representative extract from `main.c`:

```c
static const char *example_3 =
    "import json\n"
    "\n"
    "def myfunction(jsonstring):\n"
    "    r = json.loads(jsonstring)\n"
    "    r['payload2'] = 'test2'\n"
    "    return json.dumps(r)\n"
    "\n"
    ;

mp_embed_exec_str(example_3);   // defines myfunction(jsonstring)
const char *result = mp_embed_exec_string_function("myfunction", "{\"payload\":\"test\"}");
printf("result: %s\n", result); // {"payload": "test", "payload2": "test2"}
```

## Compiler settings and enabled modules

The build links against `libm` (`-lm`) because `MICROPY_PY_MATH` and
single-precision float parsing require it.  The default optimisation level is
`-Og` for a debug-friendly build; replace it with `-O3` for production use.

The following MicroPython modules are enabled in `mpconfigport.h`:

- `MICROPY_PY_MATH` — floating-point math functions (`math` module).
- `MICROPY_PY_JSON` — JSON serialisation and deserialisation (`json` module).
- `MICROPY_PY_GC` — garbage collector control (`gc` module).
- `MICROPY_EMBED_EXEC_STR_FUNCTION` — C-to-Python function call API.

## Pre-testing MicroPython code on Linux

Because this example compiles to a native Linux executable, it provides a
convenient way to develop and validate MicroPython application logic on a
developer workstation before cross-compiling for a target microcontroller.
The module set (`math`, `json`, `gc`) is kept deliberately close to the ESP32
examples so that the same *Python* source can be reused across platforms with
minimal changes to `mpconfigport.h`.

## IO stub

This example enables `MICROPY_PY_IO` but does not provide a real filesystem.
`linux_fc_stub.c` supplies the three symbols that the MicroPython core
requires when `MICROPY_VFS` is disabled:

- `mp_import_stat` always returns `MP_IMPORT_STAT_NO_EXIST`, so attempts to
  import modules from the filesystem are rejected gracefully.
- `mp_builtin_open` and `mp_builtin_open_obj` raise `NotImplementedError` if
  `open()` is called from *Python*.

These stubs live entirely within this example directory.  The shared
`ports/embed/` port and `embed.mk` are untouched, so no other project that
consumes the embed port is affected.

## Building

```bash
# Generate the MicroPython embed package (run from within this directory):
make -f micropython_embed.mk

# Build the example executable:
make

# Run it:
./embed
```
