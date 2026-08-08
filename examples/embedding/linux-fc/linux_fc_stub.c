/* This file is part of the MicroPython project, http://micropython.org/
 * The MIT License (MIT)
 *
 * This file is project-local to examples/embedding/linux-fc and is NOT part
 * of the shared ports/embed port. It exists to satisfy the `mp_builtin_open`
 * / `mp_builtin_open_obj` / `mp_import_stat` symbols required by the core
 * when MICROPY_PY_IO is enabled and MICROPY_VFS is disabled (see
 * py/builtin.h). Keeping this file here, instead of adding it to
 * ports/embed/port/, ensures the shared embed port and embed.mk remain
 * untouched and other projects that include ports/embed/embed.mk are not
 * affected.
 */

#include "py/builtin.h"
#include "py/runtime.h"

// This example does not provide a real filesystem, so all imports are
// reported as "not found".
mp_import_stat_t mp_import_stat(const char *path) {
    (void)path;
    return MP_IMPORT_STAT_NO_EXIST;
}

// Dummy implementation of the "open" builtin: this example has no
// filesystem to open files from, so calling it always raises.
mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    (void)n_args;
    (void)args;
    (void)kwargs;
    mp_raise_NotImplementedError(MP_ERROR_TEXT("open() is not supported in this example"));
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);
