<!-- markdownlint-disable-file no-inline-html first-line-h1 -->
<div align="center">
<picture>
  <source media="(prefers-color-scheme: dark)" srcset="https://github.com/user-attachments/assets/68bb6d18-e94f-4cb3-bcf3-2b8ac15013a8">
  <source media="(prefers-color-scheme: light)" srcset="https://github.com/user-attachments/assets/7e0be05c-872f-417b-96e4-85b279eae65f">
  <img alt="" src="https://github.com/jpxs-intl/RosaServer/assets/19525688/9e7b9d80-005b-41a9-a898-1190a7550ca5">
</picture>

![Build](https://github.com/jpxs-intl/RosaServer/actions/workflows/release.yml/badge.svg) ![CodeQL](https://github.com/jpxs-intl/RosaServer/actions/workflows/codeql.yml/badge.svg) ![Test](https://github.com/jpxs-intl/RosaServer/actions/workflows/test.yml/badge.svg) ![Build (clang)](https://github.com/jpxs-intl/RosaServer/actions/workflows/release-clang.yml/badge.svg) ![Test](https://github.com/jpxs-intl/RosaServer/actions/workflows/test-clang.yml/badge.svg)

A Linux server scripting API for [Sub Rosa](https://store.steampowered.com/app/272230/Sub_Rosa) utilizing [LuaJIT](https://github.com/moonjit/moonjit).
</div>

# Getting Started

## Installing

- Build the library or download the latest [Release](https://github.com/jpxs-intl/RosaServer/releases).
- Your directory should contain `libluajit.so`, `librosaserver.so`, `subrosadedicated.x64`, and the `data` folder (the last two can be found with your game install).
  - You will also need the `rosaserversatellite` binary if you plan to use the ChildProcess API. Make sure it has execute permissions.
- There's a 99% chance you'll also want to use [RosaServerCore](https://github.com/jpxs-intl/RosaServerCore).
- To run it, you'll need the following Ubuntu (or equivalent on your distro) packages: `lz4`, `libopus-dev`, `libsqlite3-dev`, OpenSSL (by default on most distros), and glibc from GCC 13.

## Running

```bash
LD_PRELOAD="$(pwd)/libluajit.so $(pwd)/librosaserver.so" ./subrosadedicated.x64
```

The server will start as normal and `main/init.lua` will be executed.

# Documentation

For complete reference on using the Lua API, go to the [wiki](https://github.com/jpxs-intl/RosaServer/wiki).

# Building

Make sure all submodules are cloned, and run `./build.sh`

## Required Packages

- `build-essential` on Debian/Ubuntu
- `cmake`
- `libssl-dev`
- `libsqlite3-dev`
- `libopus-dev`
- `liblz4-dev`

Here's a basic script I use to copy the required files after they're compiled. For example, `./build.sh && ./postbuild.sh`

```bash
#!/bin/bash

DEST=../RosaServerCore/

cp ./moonjit/src/libluajit.so "$DEST"
cp ./release/RosaServer/librosaserver.so "$DEST"
cp ./release/RosaServerSatellite/rosaserversatellite "$DEST"
```

---

Thanks to these open source libraries:

- [moonjit](https://github.com/moonjit/moonjit)
- [Sol3](https://github.com/ThePhD/sol2)
- [SubHook](https://github.com/Zeex/subhook)
- [cpp-httplib](https://github.com/yhirose/cpp-httplib)
- [stb](https://github.com/nothings/stb)
