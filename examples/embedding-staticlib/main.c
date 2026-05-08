/* This file is part of the MicroPython project, http://micropython.org/
 * The MIT License (MIT)
 * Copyright (c) 2022-2023 Damien P. George
 */

#include "port/micropython_embed.h"

// This is example 1 script, which uses the json module.
static const char *example_1 =
    "import json\n"
    "\n"
    "def myfunction(jsonstring):\n"
    "    r = json.loads(jsonstring)\n"
    "    r['payload2'] = 'test2'\n"
    "    return json.dumps(r)\n"
    "\n"
    "result = myfunction('{\"payload\": \"test\"}')\n"
    "print(result)\n"
    ;

// This is example 2 script, which uses both the json and math modules.
static const char *example_2 =
    "import json\n"
    "import math\n"
    "\n"
    "def myfunction(jsonstring):\n"
    "    r = json.loads(jsonstring)\n"
    "    r['payload2'] = 'test2'\n"
    "    r['pi'] = math.pi\n"
    "    r['sqrt2'] = math.sqrt(2)\n"
    "    return json.dumps(r)\n"
    "\n"
    "result = myfunction('{\"payload\": \"test\"}')\n"
    "print(result)\n"
    ;

// This array is the MicroPython GC heap.
static char heap[8 * 1024];

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

    // Run the example scripts (they will be compiled first).
    mp_embed_exec_str(example_1);
    mp_embed_exec_str(example_2);

    // Deinitialise MicroPython.
    mp_embed_deinit();

    return 0;
}
