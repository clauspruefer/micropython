# This file is part of the MicroPython project, http://micropython.org/
# The MIT License (MIT)
# Copyright (c) 2022-2023 Damien P. George

# Toolchain file for cross-compiling to RISC-V 32-bit targets (e.g. ESP32-C3).
#
# Usage:
#   cmake -B build -DCMAKE_TOOLCHAIN_FILE=riscv32-cross.cmake
#
# The cross-compiler is expected to be in PATH or set via RISCV32_TOOLCHAIN_PATH.
# For ESP32-C3 using esp-idf, the toolchain is typically:
#   riscv32-esp-elf-gcc (from the Espressif toolchain)
#
# Set RISCV32_TOOLCHAIN_PATH to the directory containing the cross-compiler
# binaries if they are not in PATH, for example:
#   cmake -B build \
#     -DCMAKE_TOOLCHAIN_FILE=riscv32-cross.cmake \
#     -DRISCV32_TOOLCHAIN_PATH=/path/to/esp-idf/toolchain/bin

cmake_minimum_required(VERSION 3.16)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv32)

# Toolchain binary prefix and optional path override.
set(RISCV32_TOOLCHAIN_PREFIX "riscv32-esp-elf")
if(DEFINED RISCV32_TOOLCHAIN_PATH)
    set(_TOOLCHAIN_BIN "${RISCV32_TOOLCHAIN_PATH}/${RISCV32_TOOLCHAIN_PREFIX}")
else()
    set(_TOOLCHAIN_BIN "${RISCV32_TOOLCHAIN_PREFIX}")
endif()

set(CMAKE_C_COMPILER   "${_TOOLCHAIN_BIN}-gcc")
set(CMAKE_CXX_COMPILER "${_TOOLCHAIN_BIN}-g++")
set(CMAKE_ASM_COMPILER "${_TOOLCHAIN_BIN}-gcc")
set(CMAKE_AR           "${_TOOLCHAIN_BIN}-ar")
set(CMAKE_RANLIB       "${_TOOLCHAIN_BIN}-ranlib")
set(CMAKE_OBJCOPY      "${_TOOLCHAIN_BIN}-objcopy")
set(CMAKE_OBJDUMP      "${_TOOLCHAIN_BIN}-objdump")
set(CMAKE_SIZE         "${_TOOLCHAIN_BIN}-size")

# Prevent cmake from searching host system paths for libraries/includes.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Compiler flags for ESP32-C3 (RV32IMC architecture).
set(CMAKE_C_FLAGS_INIT   "-march=rv32imc -mabi=ilp32 -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS_INIT "-march=rv32imc -mabi=ilp32 -ffunction-sections -fdata-sections")

# Skip compiler test for cross-compilation (no emulator available).
set(CMAKE_C_COMPILER_WORKS   1)
set(CMAKE_CXX_COMPILER_WORKS 1)
