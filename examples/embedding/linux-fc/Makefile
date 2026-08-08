# This file is part of the MicroPython project, http://micropython.org/
# The MIT License (MIT)
# Copyright (c) 2022-2023 Damien P. George
#
# This is a very simple makefile that demonstrates how to build the embed port.
# All it needs to do is build all *.c files in the micropython_embed directory.
# This makefile would be replaced with your custom build system.

EMBED_DIR = micropython_embed
PROG = embed

CFLAGS += -I.
CFLAGS += -I$(EMBED_DIR)
CFLAGS += -I$(EMBED_DIR)/port
CFLAGS += -Wall -Og -fno-common

SRC += main.c
SRC += linux_fc_stub.c
SRC += $(wildcard $(EMBED_DIR)/*/*.c) $(wildcard $(EMBED_DIR)/*/*/*.c)
OBJ += $(SRC:.c=.o)

# MICROPY_PY_MATH/MICROPY_PY_JSON (via float parsing) need libm; scoped to
# this project only, ports/embed/embed.mk is not modified.
LDFLAGS += -lm

$(PROG): $(OBJ)
	$(CC) -no-pie -o $@ $(OBJ) $(LDFLAGS)

clean:
	/bin/rm -f $(OBJ) $(PROG)
