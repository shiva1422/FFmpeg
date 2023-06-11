cmake_minimum_required(VERSION 3.18.1)
add_library(ksimage KSImageLoader.cpp  ImageCodec/STBImageLoader.cpp ImageCodec/stb_image.h)
