cmake_minimum_required(VERSION 3.22)
project(krkr2core)

set(KRKR2CORE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(AXMOL_PATH ${_AX_ROOT})

# prepare static lib
function(add_static_library TARGET LIBNAME)
    if ("${LIBNAME}" STREQUAL "")
        set(LIBNAME "lib${TARGET}.a")
    endif()
    add_library("${TARGET}_static" STATIC IMPORTED GLOBAL)
    set_target_properties("${TARGET}_static"
        PROPERTIES IMPORTED_LOCATION
        ${PORTBUILD_PATH}/lib/${LIBNAME}
    )
endfunction()

add_static_library(jpeg libturbojpeg.a)
add_static_library(ogg "")
add_static_library(opus "")
add_static_library(opusfile "")
add_static_library(vorbis "")
add_static_library(vorbisfile "")
add_static_library(vorbisenc "")
add_static_library(openal "")
add_static_library(archive "")
add_static_library(avutil "")
add_static_library(avfilter "")
add_static_library(avformat "")
add_static_library(avcodec "")
add_static_library(swscale "")
add_static_library(swresample "")
add_static_library(onig "")
add_static_library(sdl2 libSDL2.a)

# search kr2core src
file(GLOB KRKR2CORE_CODE
    ${KRKR2CORE_PATH}/visual/*.cpp
    ${KRKR2CORE_PATH}/base/*.cpp
    ${KRKR2CORE_PATH}/base/win32/*.cpp
    ${KRKR2CORE_PATH}/environ/*.cpp
    ${KRKR2CORE_PATH}/environ/ConfigManager/*.cpp
    ${KRKR2CORE_PATH}/environ/cocos2d/*.cpp
    ${KRKR2CORE_PATH}/environ/linux/*.cpp
    ${KRKR2CORE_PATH}/environ/ui/*.cpp
    ${KRKR2CORE_PATH}/environ/ui/extension/*.cpp
    ${KRKR2CORE_PATH}/environ/win32/SystemControl.cpp 
    ${KRKR2CORE_PATH}/extension/*.cpp
    ${KRKR2CORE_PATH}/movie/*.cpp
    ${KRKR2CORE_PATH}/movie/*/*.cpp
    ${KRKR2CORE_PATH}/msg/*.cpp
    ${KRKR2CORE_PATH}/msg/win32/MsgImpl.cpp
    ${KRKR2CORE_PATH}/msg/win32/OptionsDesc.cpp 
    ${KRKR2CORE_PATH}/sound/*.cpp
    ${KRKR2CORE_PATH}/sound/win32/*.cpp
    ${KRKR2CORE_PATH}/tjs2/*.cpp
    ${KRKR2CORE_PATH}/utils/*.c
    ${KRKR2CORE_PATH}/utils/*.cpp
    ${KRKR2CORE_PATH}/utils/encoding/*.c
    # minizip - include only zip.c (writing); unzip/ioapi provided by axmol
    ${KRKR2CORE_PATH}/utils/minizip/zip.c
    ${KRKR2CORE_PATH}/utils/tinyxml2/*.cpp
    ${KRKR2CORE_PATH}/utils/win32/*.cpp
    ${KRKR2CORE_PATH}/visual/gl/*.cpp
    ${KRKR2CORE_PATH}/visual/ogl/*.cpp
    ${KRKR2CORE_PATH}/visual/win32/*.cpp
)

file(GLOB KRKR2CORE_CODEANDROID
    ${KRKR2CORE_PATH}/environ/android/*.cpp
    ${KRKR2CORE_PATH}/sound/ARM/*.c
    ${KRKR2CORE_PATH}/sound/ARM/*.cpp
    ${KRKR2CORE_PATH}/visual/ARM/*.cpp
    ${KRKR2CORE_PATH}/visual/ARM/*.c
)

list(REMOVE_ITEM KRKR2CORE_CODE
    ${KRKR2CORE_PATH}/base/win32/FuncStubs.cpp
    ${KRKR2CORE_PATH}/base/win32/SusieArchive.cpp
    ${KRKR2CORE_PATH}/environ/MainFormUnit.cpp
    ${KRKR2CORE_PATH}/environ/XP3ArchiveRepack.cpp
    ${KRKR2CORE_PATH}/environ/ui/XP3RepackForm.cpp
    ${KRKR2CORE_PATH}/sound/xmmlib.cpp
    ${KRKR2CORE_PATH}/sound/WaveFormatConverter_SSE.cpp
    ${KRKR2CORE_PATH}/visual/Resampler.cpp
    ${KRKR2CORE_PATH}/visual/win32/GDIFontRasterizer.cpp 
    ${KRKR2CORE_PATH}/visual/win32/NativeFreeTypeFace.cpp
    ${KRKR2CORE_PATH}/visual/win32/TVPSysFont.cpp 
    ${KRKR2CORE_PATH}/visual/win32/VSyncTimingThread.cpp
    ${KRKR2CORE_PATH}/visual/LoadJXR.cpp
    ${KRKR2CORE_PATH}/visual/LoadBPG.cpp
)

if(CMAKE_SYSTEM_NAME MATCHES "Android")
    list(APPEND KRKR2CORE_CODE ${KRKR2CORE_CODEANDROID})
    set(PLATFORM android)
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(PLATFORM linux)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(PLATFORM win32)
else()
    set(PLATFORM android)
endif()

# make krkr2core
add_library(${PROJECT_NAME} STATIC ${KRKR2CORE_CODE})
if(CMAKE_SYSTEM_NAME MATCHES "Android")
    target_include_directories(${PROJECT_NAME} PUBLIC
        ${PORTBUILD_PATH}/sdk/native/jni/include
        ${CMAKE_ANDROID_NDK}/sources/android/cpufeatures
    )
    target_link_libraries(${PROJECT_NAME} PUBLIC
        -Wl,-Bdynamic
        log
        android
        EGL
        GLESv2
        GLESv1_CM
        OpenSLES
    )
endif()
target_include_directories(${PROJECT_NAME} PUBLIC     
    ${KRKR2CORE_PATH}/
    ${KRKR2CORE_PATH}/base
    ${KRKR2CORE_PATH}/base/win32 
    ${KRKR2CORE_PATH}/environ 
    ${KRKR2CORE_PATH}/environ/cocos2d 
    ${KRKR2CORE_PATH}/environ/win32 
    ${KRKR2CORE_PATH}/environ/android 
    ${KRKR2CORE_PATH}/environ/sdl 
    ${KRKR2CORE_PATH}/msg 
    ${KRKR2CORE_PATH}/msg/win32 
    ${KRKR2CORE_PATH}/extension
    ${KRKR2CORE_PATH}/sound 
    ${KRKR2CORE_PATH}/sound/win32 
    ${KRKR2CORE_PATH}/tjs2
    ${KRKR2CORE_PATH}/utils 
    ${KRKR2CORE_PATH}/utils/win32
    ${KRKR2CORE_PATH}/visual
    ${KRKR2CORE_PATH}/visual/ARM
    ${KRKR2CORE_PATH}/visual/win32
    ${KRKR2CORE_PATH}/visual/RenderScript/rs
    ${KRKR2CORE_PATH}/../plugins

    # axmol engine includes
    ${AXMOL_PATH}
    ${AXMOL_PATH}/core
    ${AXMOL_PATH}/core/platform
    ${AXMOL_PATH}/extensions
    ${AXMOL_PATH}/extensions/cocostudio/src
    ${AXMOL_PATH}/extensions/GUI/src
    ${AXMOL_PATH}/extensions/Particle3D/src
    ${AXMOL_PATH}/3rdparty/box2d/include

    # thirdparty includes
    ${PORTBUILD_PATH}/include
    ${THIRDPARTY_PATH}/port
    ${THIRDPARTY_PATH}/port/libarchive
    ${THIRDPARTY_PATH}/port/opusfile-0.12/include
    ${THIRDPARTY_PATH}/port/opus-1.3.1/include
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    -DTJS_TEXT_OUT_CRLF -D__STDC_CONSTANT_MACROS -DUSE_UNICODE_FSTRING
    -DTJS_TEXT_OUT_CRLF -D_7ZIP_ST 
)
target_compile_options(${PROJECT_NAME} PUBLIC 
    -fPIC
)

target_link_directories(${PROJECT_NAME} PUBLIC
    ${PORTBUILD_PATH}/lib
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    -Wl,-Bstatic
    # audio
    ogg_static
    opus_static
    vorbis_static 
    vorbisfile_static
    vorbisenc_static
    opusfile_static
    oboe
    openal_static

    # image
    jpeg_static
    opencv_imgproc
    opencv_core
    tegra_hal
    
    # video
    avutil_static
    avfilter_static
    avformat_static
    avcodec_static
    swscale_static
    swresample_static

    # archive
    unrar
    lz4
    archive_static #  libbpg_static jxrlib_static
    7za

    # others
    onig_static
    breakpad
    breakpad_client
    
    # framework - axmol engine (replaces cocos2d + ext_*)
    sdl2_static
    axmol
    GUI
    cocostudio

    z
)