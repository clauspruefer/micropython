/* This file is part of the MicroPython project, http://micropython.org/
 * The MIT License (MIT)
 * Copyright (c) 2022-2023 Damien P. George
 */

#include <stdio.h>
#include "port/micropython_embed.h"
#include "pong-micropython.h"
#include "pong-debug.h"

// This is example 1 script, which will be compiled and executed.
static const char* example_1 =
    "print('hello world!', list(x + 1 for x in range(10)), end='eol\\n')";

// This is example 2 script, which will be compiled and executed.
static const char* example_2 =
    "for i in range(10):\n"
    "    print('iter {:08}'.format(i))\n"
    "\n"
    "try:\n"
    "    1//0\n"
    "except Exception as er:\n"
    "    print('caught exception', repr(er))\n"
    "\n"
    "import gc\n"
    "print('run GC collect')\n"
    "gc.collect()\n"
    "\n"
    "print('finish')\n";

// This is example 3 script, which will be compiled, executed and myfunction called
// with a JSON string parameter. The function parses it, adds a key, and returns the result.
static const char* example_3 =
    "import json\n"
    "\n"
    "print('testing JSON function definition - start')\n"
    "def myfunction(jsonstring):\n"
    "    r = json.loads(jsonstring)\n"
    "    r['payload2'] = 'test2'\n"
    "    return json.dumps(r)\n"
    "\n"
    "print('testing JSON function definition - end')\n";

// Input JSON string passed to example 3's myfunction.
static const char* example_3_function_parameter = "{ \"payload\": \"test\" }";

static const char* example_4 =
    "import math\n"
    "\n"
    "print('testing math functions - start')\n"
    "try:\n"
    "    a = math.pi\n"
    "    b = math.sqrt(1)\n"
    "    c = math.atan2(1, 2)\n"
    "    d = float(a)\n"
    "    e = math.sin(a)\n"
    "    f = math.cos(a)\n"
    "except Exception as er:\n"
    "    print('caught exception', repr(er))\n"
    "print('testing math functions - end')\n"
    "\n";

static const char* pong_parameter_start = "{ \"start\": \"multi\" }";
static const char* pong_parameter_step1 = "{ \"player1\": \"down\" }";
static const char* pong_parameter_step2 = "{ \"player2\": \"up\" }";

// This array is the MicroPython GC heap.
static char heap[64 * 1024];

int main() {
    // Initialise MicroPython.
    int stack_top;
    mp_embed_init(&heap[0], sizeof(heap), &stack_top);

    // Run the example scripts (they will be compiled first).
    mp_embed_exec_str(example_1);
    mp_embed_exec_str(example_2);

    // Run example 3: call myfunction(jsonstring) and print the result.
    mp_embed_exec_str(example_3);
    const char* result = NULL;
    result = mp_embed_exec_string_function("myfunction", example_3_function_parameter);
    if (result) { printf("example 3 result: %s\n", result); }

    // Check math functions
    mp_embed_exec_str(example_4);

    mp_embed_exec_str(arduino_pong_code);

    result = mp_embed_exec_string_function("render_frame_no_dt", pong_parameter_start);
    if (result) { printf("pong result: %s\n", result); }

    for (int i = 0; i < 1000; i++) {
        result = mp_embed_exec_string_function("render_frame_no_dt", pong_parameter_step1);
        if (result) { printf("pong result: %s\n", result); }
        render_pong_frame_str(result);
        rpf_sleep_ms(30);
        result = mp_embed_exec_string_function("render_frame_no_dt", pong_parameter_step2);
        if (result) { printf("pong result: %s\n", result); }
        render_pong_frame_str(result);
        rpf_sleep_ms(30);
    }

    // Deinitialise MicroPython.
    mp_embed_deinit();

    return 0;
}
