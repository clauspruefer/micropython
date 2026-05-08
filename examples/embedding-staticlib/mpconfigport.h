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
#define MICROPY_PY_GC                           (1)
#define MICROPY_PY_SYS                          (0)

// Float support.
// MICROPY_FLOAT_IMPL must be set to a non-None value to activate
// MICROPY_PY_BUILTINS_FLOAT, which is required by the math module.
// Use MICROPY_FLOAT_IMPL_FLOAT (single precision) for memory-constrained
// targets, or MICROPY_FLOAT_IMPL_DOUBLE (double precision) for higher accuracy.
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_DOUBLE)

// MicroPython math module.
// Requires MICROPY_FLOAT_IMPL to be set to a non-None value above.
#define MICROPY_PY_MATH                         (1)

// MicroPython JSON module.
// Requires MICROPY_PY_IO for StringIO support used in json.loads.
#define MICROPY_PY_IO                           (1)
#define MICROPY_PY_JSON                         (1)
