/* This file is part of the MicroPython project, http://micropython.org/
 * The MIT License (MIT)
 * Copyright (c) 2022-2023 Damien P. George
 */

#include <stdio.h>
#include "port/micropython_embed.h"

// This script imports the standard library modules and defines myfunction,
// which loads a JSON string, adds a key, and returns the modified JSON.
static const char *example_4 =
    "import json\n"
    "import math\n"
    "import random\n"
    "import sys\n"
    "import time\n"
    "\n"
    "def myfunction(jsonstring):\n"
    "    r = json.loads(jsonstring)\n"
    "    r['payload2'] = 'test2'\n"
    "    return json.dumps(r)\n"
    "\n"
    ;

// Input JSON string passed to myfunction.
static const char *example_4_function_string = "{ \"payload\": \"test\" }";

// This array is the MicroPython GC heap.
static char heap[32 * 1024];

int main() {
    // Initialise MicroPython.
    //
    // Note: &stack_top below should be good enough for many cases.
    // However, depending on environment, there might be more appropriate
    // ways to get the stack top value.
    // eg. pthread_get_stackaddr_np, pthread_getattr_np,
    // __builtin_frame_address/__builtin_stack_address, etc.
    int stack_top;
    mp_embed_init(&heap[0], sizeof(heap), &stack_top);

    // Run example 4: define myfunction and call it with a JSON string.
    mp_embed_exec_str(example_4);
    const char *result = mp_embed_exec_string_function("myfunction", example_4_function_string);
    if (result) {
        printf("example 4 result: %s\n", result);
    }

    // Deinitialise MicroPython.
    mp_embed_deinit();

    return 0;
}
