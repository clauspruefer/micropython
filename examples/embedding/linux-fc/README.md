# Linux embedding example with function call, JSON and math support

This example demonstrates embedding MicroPython on Linux with:

- The `json` module (`import json`), using `MICROPY_PY_JSON`.
- The `math` module (`import math`), using `MICROPY_PY_MATH` together with
  `MICROPY_FLOAT_IMPL`.
- The `open()` builtin and `io` module (`MICROPY_PY_IO`), backed directly by
  the host's POSIX `open()`/`read()`/`write()`/`close()` functions (see
  `ports/embed/port/embed_posix_open.c`), without requiring the full VFS
  subsystem.
- `mp_embed_exec_string_function()`, which calls a named Python function with
  a single string argument and returns its string result.

## Build and run

```bash
$ make -f micropython_embed.mk
$ make
$ ./embed
```

## Notes

Enabling `MICROPY_PY_MATH` requires a floating point implementation
(`MICROPY_FLOAT_IMPL`) to be set, otherwise `MICROPY_PY_BUILTINS_FLOAT` stays
disabled and `import math` is not registered as a builtin module. Enabling
floats requires linking against `libm` (`-lm`) on Linux, otherwise functions
such as `powf`/`nearbyintf`/`roundf` (used by `round()` and `math`) fail to
link.

Enabling `MICROPY_PY_JSON` (or any other module using `io.StringIO`) requires
`MICROPY_PY_IO` to be enabled as well. Because `py/modio.c` expects the port
to supply `mp_builtin_open_obj`/`mp_import_stat` when `MICROPY_VFS` is not
enabled, this example provides a minimal implementation of both, backed by
POSIX `open()`, in `ports/embed/port/embed_posix_open.c`.
