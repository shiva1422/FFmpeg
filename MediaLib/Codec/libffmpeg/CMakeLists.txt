cmake_minimum_required(VERSION 3.10.2)

##ffmpeg
#set(FFMPEG_DIR /opt/homebrew/Cellar/ffmpeg/5.1.2)

set(FFMPEG_DIR /Users/shivaaz/projects/kalasoft/external/ffsrc/FFmpeg/build/mac)
#set(FFMPEG_DIR /opt/homebrew/)

find_package(ZLIB REQUIRED)
find_package(Iconv REQUIRED)



add_library(avformat STATIC IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION "${FFMPEG_DIR}/lib/libavformat.a")

add_library(avutil STATIC IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION "${FFMPEG_DIR}/lib/libavutil.a")

add_library(avcodec STATIC IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION "${FFMPEG_DIR}/lib/libavcodec.a")

add_library(swresample STATIC IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION "${FFMPEG_DIR}/lib/libswresample.a")

add_library(swscale STATIC IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${FFMPEG_DIR}/lib/libswscale.a)
set(FFMPEG_LIBS avutil avformat avcodec swresample swscale)

include_directories(${FFMPEG_DIR}/include)

add_library(ffutils FFUtils/FFMpegFileUtils.cpp FFUtils/FFMpegLogUtils.cpp FFUtils/FFMpegUtils.cpp FFUtils/FFFrameUtils.cpp)
#TODO KSCoded to seperate also ksffmpeg as shared lib
add_library(fflib Codec/FFMpegDecoder.cpp Codec/FFMpegDecoder.cpp Codec/FFMpegAudioDecoder.cpp Codec/FFMpegVideoDecoder.cpp  Codec/KSCodec.cpp)
target_link_libraries(fflib ffutils ${FFMPEG_LIBS}  ZLIB::ZLIB Iconv::Iconv)