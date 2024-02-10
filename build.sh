#!/bin/bash
STYLE="\e[36;1m\e[1m"
RESET="\e[0m"

TYPE=${TYPE:-Release}

rm -rf release
mkdir -p release
cd release || exit

echo -e "${STYLE}Compiling moonjit...${RESET}"
pushd ../moonjit/src || exit
make clean
make XCFLAGS+=-DLUAJIT_ENABLE_LUA52COMPAT -DLUAJIT_ENABLE_GC64 -j"${nproc}"
popd || exit

echo -e "${STYLE}Generating build files with CMake (${TYPE})...${RESET}"
cmake -DCMAKE_BUILD_TYPE="${TYPE}" ..
echo -e "${STYLE}Compiling RosaServer (${TYPE})...${RESET}"
make -j"${nproc}"
