#!/bin/bash

cp ./riscv32-cross.cmake.tpl ./riscv32-cross.cmake
sed -i "s#\[HOMEDIR\]#${HOME}#g" ./riscv32-cross.cmake

TOOLCHAIN_BASE="${HOME}/.espressif/tools/riscv32-esp-elf"
TOOLCHAIN_VERSION=$(ls "${TOOLCHAIN_BASE}" 2>/dev/null | head -1)

if [ -z "${TOOLCHAIN_VERSION}" ]; then
    echo "Error: riscv32-esp-elf toolchain not found in ${TOOLCHAIN_BASE}" >&2
    exit 1
fi

sed -i "s#\[RISCV32_TOOLCHAIN_VERSION\]#${TOOLCHAIN_VERSION}#g" ./riscv32-cross.cmake
