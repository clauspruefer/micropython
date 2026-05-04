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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void mp_embed_init(void *gc_heap, size_t gc_heap_size, void *stack_top);
void mp_embed_deinit(void);

// Only available if MICROPY_ENABLE_COMPILER is enabled.
void mp_embed_exec_str(const char *src);
// Calls the named function with param1_value and copies the string result into
// result_buf (at most result_buf_size-1 characters, always null-terminated).
// Uses no dynamic allocation — safe on bare-metal targets without a system malloc.
// If exec_str_before_function_call is true (the default), src is compiled and
// executed first (via mp_embed_exec_str) to populate globals with defined functions.
// Returns true on success, false if an exception was raised or the result is not a string.
bool mp_embed_exec_string_function(const char *src, const char *function_name, const char *param1_value, char *result_buf, size_t result_buf_size, bool exec_str_before_function_call);

// Only available if MICROPY_PERSISTENT_CODE_LOAD is enabled.
void mp_embed_exec_mpy(const uint8_t *mpy, size_t len);

#endif // MICROPY_INCLUDED_MICROPYTHON_EMBED_H
