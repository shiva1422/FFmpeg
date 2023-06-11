cmake_minimum_required(VERSION 3.22)

add_subdirectory(Codec/libffmpeg ./fflib)
add_subdirectory(Codec/KSImage ./ksimage)
include_directories(Codec/libffmpeg Codec/KSImage)
add_library(medialib MediaMain.cpp)
target_link_libraries(medialib fflib ksimage)
