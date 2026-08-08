# This file is part of the MicroPython project, http://micropython.org/
# The MIT License (MIT)
# Copyright (c) 2022-2023 Damien P. George

# Set the location of the top of the MicroPython repository.
MICROPYTHON_TOP = ../../..

# This project enables MICROPY_PY_JSON, implemented by extmod/modjson.c.
# That file is not part of the shared embed port package (embed.mk only
# copies extmod/modplatform.h into the generated package), so without this
# it is never scanned for QSTRs/module registration and "json" ends up
# missing from the builtin module table. Add it to the qstr/moduledefs scan
# here (resolved against MICROPYTHON_TOP via the vpath set up by
# py/mkrules.mk), scoped to this project only; embed.mk itself is untouched.
SRC_QSTR += extmod/modjson.c

# Include the main makefile fragment to build the MicroPython component.
include $(MICROPYTHON_TOP)/ports/embed/embed.mk

# Also copy extmod/modjson.c into the generated package so it gets compiled
# by the project Makefile, again without modifying embed.mk.
all: micropython_embed/extmod/modjson.c

micropython_embed/extmod/modjson.c: micropython-embed-package
	$(CP) $(MICROPYTHON_TOP)/extmod/modjson.c $@
