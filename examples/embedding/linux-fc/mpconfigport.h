/* This file is part of the MicroPython project, http://micropython.org/
 * The MIT License (MIT)
 * Copyright (c) 2022-2023 Damien P. George
 */

// Include common MicroPython embed configuration.
#include <port/mpconfigport_common.h>

// Use the minimal starting configuration (disables all optional features).
#define MICROPY_CONFIG_ROM_LEVEL                (MICROPY_CONFIG_ROM_LEVEL_MINIMUM)

// MicroPython configuration.
#define MICROPY_ENABLE_COMPILER                 (1)
#define MICROPY_ENABLE_GC                       (1)
#define MICROPY_PERSISTENT_CODE_LOAD            (1)

// Enable Garbage Collector, IO and JSON modules.
#define MICROPY_PY_GC                           (1)

#define MICROPY_PY_SYS                          (1)
#define MICROPY_PY_SYS_PLATFORM                 "linux"

// MICROPY_PY_IO enables the `io` module and the `open()` builtin.  This port
// does not enable MICROPY_VFS, so `mp_builtin_open`/`mp_import_stat` are
// instead provided by ports/embed/port/embed_posix_open.c, which implements
// them directly on top of the host's POSIX open()/read()/write()/close().
#define MICROPY_PY_IO                           (1)
#define MICROPY_READER_POSIX                    (1)

#define MICROPY_PY_JSON                         (1)

// MICROPY_PY_MATH (and the `round()`/`pow()` builtins) require a floating
// point implementation to be selected, otherwise MICROPY_PY_BUILTINS_FLOAT
// stays disabled and the math functions below are never linked in the first
// place.  Enabling floats also requires linking against libm (see -lm in the
// Makefile) to satisfy functions like powf/nearbyintf/roundf.
#define MICROPY_PY_MATH                         (1)
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_FLOAT)

// Enable the string-argument function-call API (mp_embed_exec_string_function).
#define MICROPY_EMBED_EXEC_STR_FUNCTION         (1)
