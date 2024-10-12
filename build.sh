#!/bin/bash
STYLE="\e[36;1m\e[1m"
RESET="\e[0m"

TYPE=${TYPE:-Release}

echo -e "${STYLE}Compiling moonjit...${RESET}"
pushd ./moonjit/src || exit
make clean
make -j"${nproc}" XCFLAGS+="-DLUAJIT_ENABLE_LUA52COMPAT -DLUAJIT_ENABLE_GC64"
popd || exit

echo -e "${STYLE}Generating build files with CMake (${TYPE})...${RESET}"
cmake -G "Ninja" -DCMAKE_BUILD_TYPE="${TYPE}" .
echo -e "${STYLE}Compiling RosaServer (${TYPE})...${RESET}"
cmake --build . --config "${TYPE}" --parallel "${nproc}"
