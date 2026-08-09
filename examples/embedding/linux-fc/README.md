# Linux Function-Call Embedding (`linux-fc`)

This example demonstrates embedding MicroPython into a Linux host application
and calling a Python function from C using the
`mp_embed_exec_string_function` / `MICROPY_EMBED_EXEC_STR_FUNCTION` API.

## Boost.Python-style function calls

`mp_embed_exec_string_function(name, arg)` lets a C host:

1. Execute a Python source snippet (compiling it on the fly).
2. Retrieve a named Python function defined in that snippet.
3. Call it with a single string argument.
4. Receive the return value back as a C `const char *`.

This mirrors the Boost.Python / `pybind11` pattern of calling Python
callables from C++ without requiring a full Python interpreter process or
a shared `libpython`.  The key difference is that this API operates on a
minimal, statically-linked MicroPython runtime — suitable for
resource-constrained environments where CPython is impractical.

Example (`main.c`):

```c
mp_embed_exec_str(example_3);   // defines myfunction(jsonstring)
const char *result = mp_embed_exec_string_function("myfunction", "{\"payload\":\"test\"}");
printf("result: %s\n", result); // prints JSON with an extra key added
```

## Compiler settings

The build uses the following flags:

| Flag | Purpose |
|------|---------|
| `-Wall -Og` | Warnings + debug-friendly optimisation |
| `-lm` | Link libm for `MICROPY_PY_MATH` / float parsing |

For production use on Linux, replace `-Og` with `-O3`.

The following MicroPython modules are enabled in `mpconfigport.h`:

| Module | Purpose |
|--------|---------|
| `MICROPY_PY_MATH` | Floating-point math (`math` module) |
| `MICROPY_PY_JSON` | JSON encode/decode (`json` module) |
| `MICROPY_PY_GC` | Garbage collector control (`gc` module) |
| `MICROPY_EMBED_EXEC_STR_FUNCTION` | C→Python function call API |

## Pre-testing MicroPython code on Linux

Because the `linux-fc` build is a native Linux executable, it is a practical
development workflow to **write and test your MicroPython logic on Linux
first**, then cross-compile the same `mpconfigport.h` / Python source for
your target microcontroller.  The module set (`math`, `json`, `gc`) is kept
deliberately close to the ESP32 examples to maximise code portability.

## IO stub / Linux workaround

This example enables `MICROPY_PY_IO` but does not provide a real filesystem.
The file `linux_fc_stub.c` provides dummy implementations of the three
symbols the MicroPython core requires when `MICROPY_VFS` is disabled:

* `mp_import_stat` — always reports "file not found", so `import` of
  filesystem modules is gracefully rejected.
* `mp_builtin_open` / `mp_builtin_open_obj` — raises `NotImplementedError`
  if `open()` is called from Python.

These stubs are intentionally scoped to this example directory so that the
shared `ports/embed/` port and `embed.mk` remain untouched.

## Building

```bash
# From the repo root, generate the embed package first:
make -C examples/embedding/linux-fc -f micropython_embed.mk

# Then build the example executable:
make -C examples/embedding/linux-fc

# Run it:
./examples/embedding/linux-fc/embed
```
