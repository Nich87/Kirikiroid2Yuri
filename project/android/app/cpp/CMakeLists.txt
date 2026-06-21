cmake_minimum_required(VERSION 3.22)
project(krkr2yuri_android)

set(KRKR2YURI_DIR ${CMAKE_SOURCE_DIR}/../../../..)
if(${ANDROID_ABI} MATCHES "arm64-v8a")
    set(PORTBUILD_PATH ${KRKR2YURI_DIR}/thirdparty/build/arch_androida64)
else()
    set(PORTBUILD_PATH ${KRKR2YURI_DIR}/thirdparty/build/arch_androida64)
endif()

# Set AX_ROOT for axmol (needed by shader compiler etc.)
set(ENV{AX_ROOT} "${KRKR2YURI_DIR}/thirdparty/port/axmol")

# Set android project dir for axmol shader output
set(_AX_ANDROID_PROJECT_DIR ${CMAKE_SOURCE_DIR}/.. CACHE STRING "" FORCE)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
endif()

add_subdirectory(${KRKR2YURI_DIR} ${KRKR2YURI_DIR}/build_android)