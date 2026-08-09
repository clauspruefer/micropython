Example of embedding MicroPython in a C application
===================================================

This directory is the recommended starting point for embedding MicroPython in a
host application. It shows the simplest end-to-end workflow: generate the
embedding sources, compile them together with a small `main.c`, and run the
resulting executable. It does **not** build a static library.

Contents
--------

- [Directory layout](#directory-layout)
- [Build and run](#build-and-run)
- [Use from an out-of-tree project](#use-from-an-out-of-tree-project)

Directory layout
----------------

- `main.c` - minimal host application that initialises MicroPython and runs two
  small Python snippets.
- `mpconfigport.h` - MicroPython configuration used by this example.
- `micropython_embed.mk` - helper makefile that generates the
  `micropython_embed/` source tree.
- `Makefile` - simple example build that compiles `main.c` together with the
  generated `micropython_embed/` sources.
- `micropython_embed/` - generated directory created by
  `make -f micropython_embed.mk`.

Build and run
-------------

1. Generate the embedding sources:

       $ make -f micropython_embed.mk

   This creates the `micropython_embed/` directory, which contains the
   self-contained `.c` and `.h` files needed to embed MicroPython in your
   project.

2. Build the example executable:

       $ make

   This example makefile compiles `main.c` together with all generated sources
   and produces an executable named `embed`.

3. Run the example:

       $ ./embed

Use from an out-of-tree project
--------------------------------

This example works as-is inside the MicroPython source tree. For an external
project, update `MICROPYTHON_TOP` in `micropython_embed.mk` so that it points to
this repository. The generated `micropython_embed/` sources can then be built by
whatever build system your project uses.
