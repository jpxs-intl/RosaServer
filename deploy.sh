#!/bin/bash
STYLE="\e[36;1m\e[1m"
RESET="\e[0m"

DEST=../RosaServer/

echo -e "Copying ${STYLE}LuaJIT/src/libluajit.so${RESET} to ${STYLE}${DEST}${RESET}"
cp ./LuaJIT/src/libluajit.so "$DEST"
echo -e "Copying ${STYLE}release/RosaServer/librosaserver.so${RESET} to ${STYLE}${DEST}${RESET}"
cp ./release/RosaServer/librosaserver.so "$DEST"
echo -e "Copying ${STYLE}release/RosaServerSatellite/rosaserversatellite${RESET} to ${STYLE}${DEST}${RESET}"
cp ./release/RosaServerSatellite/rosaserversatellite "$DEST"
echo -e "All done"
