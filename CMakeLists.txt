cmake_minimum_required(VERSION 2.8)

project( cut CXX )

set( CMAKE_CXX_COMPILER_INIT clang++ )
set( CMAKE_BUILD_TYPE Debug )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

find_package( OpenCV REQUIRED )
find_package( RapidJSON REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} include )

add_subdirectory( detector )

add_executable( cutDetector main.cpp )

target_link_libraries( cutDetector ${OpenCV_LIBS} detector )
