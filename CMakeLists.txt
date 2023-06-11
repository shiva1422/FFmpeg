cmake_minimum_required(VERSION 3.22)
project(FFmpeg)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(GLFW_DIR "/opt/homebrew/Cellar/glfw/3.3.8")
include_directories(./Core ./MediaLib ./MediaLib/Codec ${GLFW_DIR}/include glloader/include)
link_directories(${GLFW_DIR}/lib)
set(GLFW_LIBRARIES ${GLFW_DIR}/lib/libglfw.3.3.dylib )
add_subdirectory(MediaLib ./medialib)
#set(CMAKE_CXX_FLAGS "-x objective-c++")
set(CMAKE_EXE_LINKER_FLAGS "-framework AudioToolbox -framework VideoToolbox -framework CoreFoundation -framework CoreMedia -framework AVFoundation -framework CoreVideo")
#-framework QuartzCore -framework UIKit")

set(srcs
        main.cpp
        GLFWApplication.cpp
        GLFWApplication.h
        Application.cpp
        glloader/src/glad.c
        )

add_executable(FFmpeg ${srcs})
target_link_libraries(FFmpeg ${GLFW_LIBRARIES} medialib)


