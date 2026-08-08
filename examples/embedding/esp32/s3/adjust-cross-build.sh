#!/bin/bash

cp ./xtensa-cross.cmake.tpl ./xtensa-cross.cmake
sed -i "s#\[HOMEDIR\]#${HOME}#g" ./xtensa-cross.cmake

TOOLCHAIN_BASE="${HOME}/.espressif/tools/xtensa-esp-elf"
TOOLCHAIN_VERSION=$(ls -1 "${TOOLCHAIN_BASE}" 2>/dev/null | sort -V | tail -1)

if [ -z "${TOOLCHAIN_VERSION}" ]; then
    echo "Error: xtensa-esp-elf toolchain not found in ${TOOLCHAIN_BASE}" >&2
    exit 1
fi

sed -i "s#\[XTENSA_TOOLCHAIN_VERSION\]#${TOOLCHAIN_VERSION}#g" ./xtensa-cross.cmake
