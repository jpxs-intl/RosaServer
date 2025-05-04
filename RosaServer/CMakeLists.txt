cmake_minimum_required (VERSION 3.8)

add_compile_definitions (
	SUBHOOK_SEPARATE_SOURCE_FILES
	SUBHOOK_IMPLEMENTATION
	SOL_ALL_SAFETIES_ON
	SOL_EXCEPTIONS_SAFE_PROPAGATION
	SOL_SAFE_FUNCTION
	SOL_LUAJIT
)

execute_process (
    COMMAND git log -1 --format=%H
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
configure_file(${CMAKE_CURRENT_LIST_DIR}/git_version.cpp.in ${CMAKE_CURRENT_LIST_DIR}/git_version.cpp @ONLY)
add_custom_target(AlwaysCheckGit COMMAND ${CMAKE_COMMAND}
	-DGIT_DIR=${PROJECT_SOURCE_DIR}
    -DPRE_CONFIGURE_FILE=${CMAKE_CURRENT_LIST_DIR}/git_version.cpp.in
    -DPOST_CONFIGURE_FILE=${CMAKE_CURRENT_LIST_DIR}/git_version.cpp
    -P ${CMAKE_CURRENT_LIST_DIR}/updateGit.cmake
    BYPRODUCTS ${CMAKE_CURRENT_LIST_DIR}/git_version.cpp
)

add_compile_options(-Wall -Wfatal-errors)
add_link_options(LINKER:--no-undefined)

# Allow threading
set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)

find_package (OpenSSL REQUIRED)

if(NOT CMAKE_CROSSCOMPILING)
  find_package(PkgConfig QUIET)
  pkg_check_modules(PC_OPUS opus)
  pkg_check_modules(LZ4 liblz4)
endif()

find_library(OPUS_LIBRARY
  NAMES opus
  HINTS ${HINTS_OPUS_LIBDIR} ${PC_OPUS_LIBDIR} ${PC_OPUS_LIBRARY_DIRS}
  PATHS ${PATHS_OPUS_LIBDIR}
  ${CROSSCOMPILING_NO_CMAKE_SYSTEM_PATH}
)

find_path(OPUS_INCLUDEDIR opus.h
  PATH_SUFFIXES opus
  HINTS ${HINTS_OPUS_INCLUDEDIR} ${PC_OPUS_INCLUDEDIR} ${PC_OPUS_INCLUDE_DIRS}
  PATHS ${PATHS_OPUS_INCLUDEDIR}
  ${CROSSCOMPILING_NO_CMAKE_SYSTEM_PATH}
)

find_library(LZ4_LIBRARY
    NAMES lz4
	PATH_SUFFIXES lz4
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Opus DEFAULT_MSG OPUS_INCLUDEDIR)
find_package_handle_standard_args(Lz4 DEFAULT_MSG LZ4_LIBRARY)

mark_as_advanced(OPUS_INCLUDEDIR OPUS_LIBRARY)
mark_as_advanced(LZ4_LIBRARY LZ4_INCLUDE_DIRS LZ4_LIBRARY_DIRS LZ4_LIBRARIES)

if(OPUS_FOUND)
  set(OPUS_INCLUDE_DIRS ${OPUS_INCLUDEDIR})
  if(OPUS_LIBRARY)
    set(OPUS_LIBRARIES ${OPUS_LIBRARY})
  else()
    set(OPUS_LIBRARIES)
  endif()
endif()

add_library (rosaserver SHARED
	api.cpp
	childprocess.cpp
	console.cpp
	crypto.cpp
	engine.cpp
	filewatcher.cpp
	hooks.cpp
	image.cpp
	opusencoder.cpp
	pointgraph.cpp
	rosaserver.cpp
	sqlite.cpp
	tcpserver.cpp
	tcpclient.cpp
	worker.cpp
	lz4impl.cpp
	git_version.cpp
	../subhook/subhook.c
	../subhook/subhook_unix.c
	../subhook/subhook_x86.c
)

add_dependencies (rosaserver AlwaysCheckGit)

set_property (TARGET rosaserver PROPERTY CXX_STANDARD 20)

target_link_libraries (rosaserver Threads::Threads)
target_link_libraries (rosaserver stdc++fs)
target_link_libraries (rosaserver OpenSSL::SSL)
target_link_libraries (rosaserver OpenSSL::Crypto)
target_link_libraries (rosaserver ${OPUS_LIBRARY})
target_link_libraries (rosaserver sqlite3)
target_link_libraries (rosaserver ${CMAKE_SOURCE_DIR}/moonjit/src/libluajit.so)
target_link_libraries (rosaserver ${LZ4_LIBRARY})
target_include_directories (rosaserver PUBLIC ${CMAKE_SOURCE_DIR}/moonjit/src)
target_include_directories (rosaserver PUBLIC ${CMAKE_SOURCE_DIR}/stb)
target_include_directories (rosaserver PUBLIC ${OPUS_INCLUDEDIR})
target_include_directories (rosaserver PUBLIC ${CMAKE_SOURCE_DIR}/shared)
target_include_directories (rosaserver PUBLIC ${CMAKE_SOURCE_DIR}/sol2/include)
target_include_directories (rosaserver PUBLIC ${CMAKE_SOURCE_DIR}/subhook)