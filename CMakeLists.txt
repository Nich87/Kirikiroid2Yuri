cmake_minimum_required(VERSION 3.22)
project(krkr2yuri)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(KRKR2CORE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/core)
set(KRKR2PLUGIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/plugins)
set(THIRDPARTY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

# axmol engine paths
set(_AX_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/port/axmol)
set(AXMOL_PATH ${_AX_ROOT})
set(CMAKE_MODULE_PATH ${_AX_ROOT}/cmake/Modules/)

# Disable axmol extensions/features we don't need
set(AX_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(AX_EXT_HINT OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_EXT_LUA OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_EXT_GUI ON CACHE BOOL "" FORCE)
set(AX_ENABLE_EXT_ASSETMANAGER OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_EXT_SPINE OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_EXT_DRAGONBONES OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_EXT_COCOSTUDIO ON CACHE BOOL "" FORCE)
set(AX_ENABLE_EXT_FAIRYGUI OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_EXT_LIVE2D OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_EXT_EFFEKSEER OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_EXT_IMGUI OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_EXT_INSPECTOR OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_EXT_SDFGEN OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_EXT_PARTICLE3D OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_EXT_PHYSICS_NODE OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_EXT_JSONDEFAULT OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_PHYSICS OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_3D OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_3D_PHYSICS OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_NAVMESH OFF CACHE BOOL "" FORCE)
# Disable axmol's built-in audio (we use our own OpenAL/SDL2 audio stack)
set(AX_ENABLE_AUDIO OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_MEDIA OFF CACHE BOOL "" FORCE)
set(AX_ENABLE_CONSOLE OFF CACHE BOOL "" FORCE)
set(AX_UPDATE_BUILD_VERSION OFF CACHE BOOL "" FORCE)

# works before build libaxmol
include(AXBuildSet)

# build axmol engine core
set(ENGINE_BINARY_PATH ${PROJECT_BINARY_DIR}/engine)
add_subdirectory(${_AX_ROOT}/core ${ENGINE_BINARY_PATH}/axmol/core)
set(BUILD_ENGINE_DONE ON)

# build krkr2 libraries
add_subdirectory(${KRKR2CORE_PATH})
add_subdirectory(${KRKR2PLUGIN_PATH})

# build main
if(CMAKE_SYSTEM_NAME MATCHES "Android")
    add_library(${PROJECT_NAME} SHARED 
        project/android/app/cpp/krkr2_android.cpp
    )
    target_link_options(${PROJECT_NAME} PRIVATE -Wl,-z,max-page-size=16384 -Wl,-z,muldefs)
    target_compile_options(${PROJECT_NAME} PUBLIC -fPIE)
    target_include_directories(${PROJECT_NAME} PUBLIC
        src/cocos
        ${KRKR2CORE_PATH}/environ
        ${KRKR2CORE_PATH}/environ/cocos2d
        ${PORTBUILD_PATH}/include
        ${PORTBUILD_PATH}/include/breakpad
        ${_AX_ROOT}/core
        ${_AX_ROOT}/core/platform
    )
    target_link_directories(${PROJECT_NAME} PUBLIC ${PORTBUILD_PATH}/lib)
    target_link_libraries(${PROJECT_NAME} PUBLIC 
        -Wl,-Bstatic
        -Wl,--whole-archive
        cpp_android_spec krkr2plugin 
        -Wl,--no-whole-archive
        krkr2core 
    )
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    # Linux build - compile krkr2core library for validation
    # (no executable target needed for compilation testing)
    add_custom_target(${PROJECT_NAME} ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Linux build complete - see krkr2core target"
    )
    add_dependencies(${PROJECT_NAME} krkr2core)
else()
    message("${CMAKE_SYSTEM_NAME} not support yet")
    # Still create a dummy target so cmake can process the core library
    add_custom_target(${PROJECT_NAME} ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Build not supported on ${CMAKE_SYSTEM_NAME}"
    )
    add_dependencies(${PROJECT_NAME} krkr2core)
endif()