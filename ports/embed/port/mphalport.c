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

#include <stdio.h>
#include <time.h>
#include "py/builtin.h"
#include "py/mperrno.h"
#include "py/mphal.h"
#include "py/runtime.h"

// Send string of given length to stdout, converting \n to \r\n.
void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len) {
    printf("%.*s", (int)len, str);
}

#if MICROPY_PY_IO && !MICROPY_VFS
// Stub for the built-in open() function.  Full file I/O is not supported in
// the embed port; raise OSError to indicate that.
mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    mp_raise_OSError(MP_ENOENT);
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);
#endif

mp_uint_t mp_hal_ticks_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (mp_uint_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

mp_uint_t mp_hal_ticks_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (mp_uint_t)(ts.tv_sec * 1000000 + ts.tv_nsec / 1000);
}

mp_uint_t mp_hal_ticks_cpu(void) {
    return mp_hal_ticks_us();
}

void mp_hal_delay_ms(mp_uint_t ms) {
    struct timespec ts = { (time_t)(ms / 1000), (long)((ms % 1000) * 1000000UL) };
    nanosleep(&ts, NULL);
}

void mp_hal_delay_us(mp_uint_t us) {
    struct timespec ts = { (time_t)(us / 1000000), (long)((us % 1000000) * 1000UL) };
    nanosleep(&ts, NULL);
}
