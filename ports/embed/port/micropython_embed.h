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
#ifndef MICROPY_INCLUDED_MICROPYTHON_EMBED_H
#define MICROPY_INCLUDED_MICROPYTHON_EMBED_H

#include <stddef.h>
#include <stdint.h>

void mp_embed_init(void *gc_heap, size_t gc_heap_size, void *stack_top);
void mp_embed_deinit(void);

// Only available if MICROPY_ENABLE_COMPILER is enabled.
void mp_embed_exec_str(const char *src);

// Call a Python function (previously defined via mp_embed_exec_str) with a
// single string argument and return the result as a C string.
// Returns NULL if the function is not found, raises an exception, or the
// return value is not a string.
// Only available if MICROPY_ENABLE_COMPILER is enabled.
const char *mp_embed_exec_string_function(const char *func_name, const char *arg);

// Only available if MICROPY_PERSISTENT_CODE_LOAD is enabled.
void mp_embed_exec_mpy(const uint8_t *mpy, size_t len);

#endif // MICROPY_INCLUDED_MICROPYTHON_EMBED_H
