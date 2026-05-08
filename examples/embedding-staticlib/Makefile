# This file is part of the MicroPython project, http://micropython.org/
# The MIT License (MIT)
# Copyright (c) 2022-2023 Damien P. George
#
# This is a very simple makefile that demonstrates how to build the embed port
# with support for the json, math, random, sys and time standard modules.
# All it needs to do is build all *.c files in the micropython_embed directory.
# This makefile would be replaced with your custom build system.

EMBED_DIR = micropython_embed
PROG = embed-staticlib

CFLAGS += -I.
CFLAGS += -I$(EMBED_DIR)
CFLAGS += -I$(EMBED_DIR)/port
CFLAGS += -Wall -Og -fno-common

# libm_dbl uses the same internal helper names (__cos, __sin, __tan) as glibc,
# but with different signatures (they're the range-reduced musl-style helpers).
# Rename them to avoid declaration conflicts when compiling on glibc hosts.
LIBM_DBL_RENAME = -D__cos=mp__cos -D__sin=mp__sin -D__tan=mp__tan

# Suppress warnings in the libm_dbl math sources.
$(EMBED_DIR)/lib/libm_dbl/%.o: CFLAGS += $(LIBM_DBL_RENAME) -Wno-double-promotion -Wno-float-conversion
$(EMBED_DIR)/lib/libm_dbl/__rem_pio2_large.o: CFLAGS += -Wno-maybe-uninitialized

SRC += main.c
SRC += $(wildcard $(EMBED_DIR)/*/*.c) $(wildcard $(EMBED_DIR)/*/*/*.c)
OBJ += $(SRC:.c=.o)

$(PROG): $(OBJ)
	$(CC) -o $@ $^

clean:
	/bin/rm -f $(OBJ) $(PROG)
