/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2022-2023 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// This file provides a minimal `mp_builtin_open`/`mp_import_stat` implementation
// for the embed port, backed directly by the host's POSIX IO functions
// (open/read/write/close), without requiring the full MICROPY_VFS subsystem.
//
// It is only compiled in when MICROPY_PY_IO is enabled and MICROPY_VFS is not,
// which is the case for a "bare" embed configuration such as the one used by
// examples/embedding/linux-fc.  When MICROPY_VFS is enabled, the VFS layer
// (extmod/vfs*.c) is expected to provide mp_builtin_open/mp_import_stat instead.

#include "py/mpconfig.h"

#if MICROPY_PY_IO && !MICROPY_VFS

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "py/builtin.h"
#include "py/runtime.h"
#include "py/stream.h"

typedef struct _mp_obj_embed_file_t {
    mp_obj_base_t base;
    int fd;
} mp_obj_embed_file_t;

static void embed_file_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    mp_obj_embed_file_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<io.FileIO %d>", self->fd);
}

static mp_uint_t embed_file_read(mp_obj_t o_in, void *buf, mp_uint_t size, int *errcode) {
    mp_obj_embed_file_t *o = MP_OBJ_TO_PTR(o_in);
    if (o->fd < 0) {
        *errcode = EBADF;
        return MP_STREAM_ERROR;
    }
    ssize_t r;
    do {
        r = read(o->fd, buf, size);
    } while (r == -1 && errno == EINTR);
    if (r == -1) {
        *errcode = errno;
        return MP_STREAM_ERROR;
    }
    return (mp_uint_t)r;
}

static mp_uint_t embed_file_write(mp_obj_t o_in, const void *buf, mp_uint_t size, int *errcode) {
    mp_obj_embed_file_t *o = MP_OBJ_TO_PTR(o_in);
    if (o->fd < 0) {
        *errcode = EBADF;
        return MP_STREAM_ERROR;
    }
    ssize_t r;
    do {
        r = write(o->fd, buf, size);
    } while (r == -1 && errno == EINTR);
    if (r == -1) {
        *errcode = errno;
        return MP_STREAM_ERROR;
    }
    return (mp_uint_t)r;
}

static mp_uint_t embed_file_ioctl(mp_obj_t o_in, mp_uint_t request, uintptr_t arg, int *errcode) {
    mp_obj_embed_file_t *o = MP_OBJ_TO_PTR(o_in);
    switch (request) {
        case MP_STREAM_FLUSH:
            return 0;
        case MP_STREAM_SEEK: {
            if (o->fd < 0) {
                *errcode = EBADF;
                return MP_STREAM_ERROR;
            }
            struct mp_stream_seek_t *s = (struct mp_stream_seek_t *)arg;
            off_t off = lseek(o->fd, s->offset, s->whence);
            if (off == (off_t)-1) {
                *errcode = errno;
                return MP_STREAM_ERROR;
            }
            s->offset = off;
            return 0;
        }
        case MP_STREAM_CLOSE:
            if (o->fd >= 0) {
                close(o->fd);
                o->fd = -1;
            }
            return 0;
        case MP_STREAM_GET_FILENO:
            return o->fd;
        default:
            *errcode = EINVAL;
            return MP_STREAM_ERROR;
    }
}

static const mp_rom_map_elem_t embed_file_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_read), MP_ROM_PTR(&mp_stream_read_obj) },
    { MP_ROM_QSTR(MP_QSTR_readinto), MP_ROM_PTR(&mp_stream_readinto_obj) },
    { MP_ROM_QSTR(MP_QSTR_readline), MP_ROM_PTR(&mp_stream_unbuffered_readline_obj) },
    { MP_ROM_QSTR(MP_QSTR_readlines), MP_ROM_PTR(&mp_stream_unbuffered_readlines_obj) },
    { MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(&mp_stream_write_obj) },
    { MP_ROM_QSTR(MP_QSTR_seek), MP_ROM_PTR(&mp_stream_seek_obj) },
    { MP_ROM_QSTR(MP_QSTR_tell), MP_ROM_PTR(&mp_stream_tell_obj) },
    { MP_ROM_QSTR(MP_QSTR_flush), MP_ROM_PTR(&mp_stream_flush_obj) },
    { MP_ROM_QSTR(MP_QSTR_close), MP_ROM_PTR(&mp_stream_close_obj) },
    { MP_ROM_QSTR(MP_QSTR___del__), MP_ROM_PTR(&mp_stream_close_obj) },
    { MP_ROM_QSTR(MP_QSTR___enter__), MP_ROM_PTR(&mp_identity_obj) },
    { MP_ROM_QSTR(MP_QSTR___exit__), MP_ROM_PTR(&mp_stream___exit___obj) },
};
static MP_DEFINE_CONST_DICT(embed_file_locals_dict, embed_file_locals_dict_table);

static const mp_stream_p_t embed_file_stream_p = {
    .read = embed_file_read,
    .write = embed_file_write,
    .ioctl = embed_file_ioctl,
};

static MP_DEFINE_CONST_OBJ_TYPE(
    mp_type_embed_fileio,
    MP_QSTR_FileIO,
    MP_TYPE_FLAG_ITER_IS_STREAM,
    print, embed_file_print,
    protocol, &embed_file_stream_p,
    locals_dict, &embed_file_locals_dict
    );

static const mp_stream_p_t embed_textio_stream_p = {
    .read = embed_file_read,
    .write = embed_file_write,
    .ioctl = embed_file_ioctl,
    .is_text = true,
};

static MP_DEFINE_CONST_OBJ_TYPE(
    mp_type_embed_textio,
    MP_QSTR_TextIOWrapper,
    MP_TYPE_FLAG_ITER_IS_STREAM,
    print, embed_file_print,
    protocol, &embed_textio_stream_p,
    locals_dict, &embed_file_locals_dict
    );

// A port can provide this function (see py/builtin.h): used by the compiler
// and __import__ to determine if a path is a file, directory, or does not exist.
mp_import_stat_t mp_import_stat(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) {
        return MP_IMPORT_STAT_NO_EXIST;
    }
    if (S_ISDIR(st.st_mode)) {
        return MP_IMPORT_STAT_DIR;
    }
    return MP_IMPORT_STAT_FILE;
}

// A port can provide this function (see py/builtin.h): implements the `open()`
// builtin using the host's POSIX open()/read()/write()/close() functions.
mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_file, ARG_mode };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_file, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_rom_obj = MP_ROM_NONE} },
        { MP_QSTR_mode, MP_ARG_OBJ, {.u_rom_obj = MP_ROM_QSTR(MP_QSTR_r)} },
        { MP_QSTR_buffering, MP_ARG_INT, {.u_int = -1} },
        { MP_QSTR_encoding, MP_ARG_OBJ, {.u_rom_obj = MP_ROM_NONE} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    const char *fname = mp_obj_str_get_str(args[ARG_file].u_obj);
    const char *mode_s = mp_obj_str_get_str(args[ARG_mode].u_obj);

    int mode_rw = O_RDONLY;
    int mode_x = 0;
    const mp_obj_type_t *type = &mp_type_embed_textio;
    for (const char *m = mode_s; *m; ++m) {
        switch (*m) {
            case 'r':
                mode_rw = O_RDONLY;
                break;
            case 'w':
                mode_rw = O_WRONLY;
                mode_x = O_CREAT | O_TRUNC;
                break;
            case 'a':
                mode_rw = O_WRONLY;
                mode_x = O_CREAT | O_APPEND;
                break;
            case '+':
                mode_rw = O_RDWR;
                break;
            case 'b':
                type = &mp_type_embed_fileio;
                break;
            case 't':
                type = &mp_type_embed_textio;
                break;
            default:
                break;
        }
    }

    int fd = open(fname, mode_x | mode_rw, 0644);
    if (fd < 0) {
        mp_raise_OSError(errno);
    }

    mp_obj_embed_file_t *o = mp_obj_malloc_with_finaliser(mp_obj_embed_file_t, type);
    o->fd = fd;
    return MP_OBJ_FROM_PTR(o);
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

#endif // MICROPY_PY_IO && !MICROPY_VFS
