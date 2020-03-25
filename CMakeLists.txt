cmake_minimum_required(VERSION 3.1)
# Enable c++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
project(face)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


set(SRCS src/FaceSwapper.cpp)
set(HEADERS include/FaceSwapper.h)

add_executable(face ${SRCS} ${HEADERS})

target_link_libraries( face ${OpenCV_LIBS})