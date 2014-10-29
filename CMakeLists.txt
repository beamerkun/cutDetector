cmake_minimum_required(VERSION 2.8)

SET( CMAKE_CXX_COMPILER_INIT clang++ )
SET( CMAKE_BUILD_TYPE Debug )
SET( CMAKE_CXX_FLAGS -std=c++11 )
project( main )

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} includes )

ADD_SUBDIRECTORY( lib )

add_executable( main main.cpp )

target_link_libraries( main ${OpenCV_LIBS} lib )
