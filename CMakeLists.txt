cmake_minimum_required(VERSION 3.13)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{DEVKITPRO})
        set(DEVKITPRO $ENV{DEVKITPRO})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{DEVKITPRO}/cmake/Switch.cmake" CACHE PATH "toolchain file")
    else()
        message(FATAL_ERROR "Please define DEVKITPRO to point to your SDK path!")
    endif()
endif()

project(renpy-switch)

set(SWITCH_APP_NAME "Ren'Py")
set(SWITCH_AUTHOR   "renpytom")
set(SWITCH_VERSION  "8.3.7")

set(NSWITCH TRUE)
set(DKA_SWITCH_C_FLAGS "-D__SWITCH__ -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -ftls-model=local-exec -ffunction-sections -fdata-sections")

set(CMAKE_EXE_LINKER_FLAGS_INIT "-specs=${DEVKITPRO}/libnx/switch.specs")

set(CMAKE_FIND_ROOT_PATH
  ${DEVKITPRO}/devkitA64
  ${DEVKITPRO}/tools
  ${DEVKITPRO}/portlibs/switch
  ${DEVKITPRO}/libnx
)

set(OPENGL_egl_LIBRARY "/opt/devkitpro/portlibs/switch/lib/libEGL.a")
set(OPENGL_gl_LIBRARY "/opt/devkitpro/portlibs/switch/lib/libglapi.a")
set(OPENGL_EGL_INCLUDE_DIR "/opt/devkitpro/portlibs/switch/include")
set(OPENGL_INCLUDE_DIR "/opt/devkitpro/portlibs/switch/include")

set(CMAKE_PREFIX_PATH "${DEVKITPRO}/portlibs/switch")
set(ENV{PKG_CONFIG_PATH} "${DEVKITPRO}/portlibs/switch/lib/pkgconfig")

add_compile_definitions(__SWITCH__)

add_compile_options(
  -O2 -std=gnu99  # This O2 literally won't do anything for official builds btw
  -w
  -fsingle-precision-constant -fno-common
)

include_directories(
    ${DEVKITPRO}/portlibs/switch/include/python3.9
    ${DEVKITPRO}/portlibs/switch/include/fribidi
    include
    include/module
    include/module/src
    include/module/pygame_sdl2
    include/module/pygame_sdl2/src
)

#find_package(OpenSSL REQUIRED)

file(GLOB_RECURSE simple_SRC
    source/*.c
)

add_executable(${PROJECT_NAME} ${simple_SRC})

target_link_libraries("${PROJECT_NAME}"
  python3.9
  SDL2_mixer
  SDL2_image
  SDL2_ttf
  SDL2_gfx
  SDL2
  taihen_stub
  libgpu_es4_ext_stub_weak
  libIMGEGL_stub_weak
  libGLESv2_stub_weak
  SceAppMgr_stub
  SceProcessmgr_stub
  SceIme_stub_weak
  avformat
  avfilter
  swscale 
  avcodec
  webpdemux
  webp
  vorbis
  vorbisfile
  ogg
  mpg123
  FLAC
  mikmod
  opus
  swresample
  mp3lame
  avutil
  freetype
  mpdec
  jpeg
  png
  bz2
  SceMotion_stub
  SceCommonDialog_stub
  SceHid_stub
  SceGxm_stub
  SceAudio_stub
  SceAudioIn_stub
  SceAvPlayer_stub
  ScePower_stub
  SceDisplay_stub
  SceCtrl_stub
  SceTouch_stub
  SceIofilemgr_stub
  SceNet_stub
  SceNetCtl_stub
  SceSsl_stub
  SceVshBridge_stub
  SceSysmodule_stub
  ${SDL2_LIBRARIES}
  #${OPENSSL_LIBRARIES}
  pthread
  intl
  fribidi
  #harfbuzz
  gcc
  z
  m
)

add_custom_target(${PROJECT_NAME}.nso ALL
    COMMAND ${DEVKITPRO}/tools/bin/elf2nso
        ${PROJECT_NAME}.elf
        ${PROJECT_NAME}.nso
    DEPENDS ${PROJECT_NAME}
    VERBATIM
)
