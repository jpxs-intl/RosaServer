#!/bin/bash
STYLE="\e[36;1m\e[1m"
RESET="\e[0m"

TYPE=${TYPE:-Release}

rm -rf release
mkdir -p release
cd release || exit

echo -e "${STYLE}Compiling (cmake) moonjit...${RESET}"
pushd ../moonjit/src || exit
make clean
make XCFLAGS+=-DLUAJIT_ENABLE_LUA52COMPAT -j"${nproc}"
popd || exit

echo -e "${STYLE}Compiling (cmake) RosaServer (${TYPE})...${RESET}"
cmake -DCMAKE_BUILD_TYPE="${TYPE}" ..
echo -e "${STYLE}Compiling (make) RosaServer (${TYPE})...${RESET}"
make -j"${nproc}"
