cmake_minimum_required(VERSION 2.8.11)

if( UNIX )
  set( CMAKE_CXX_COMPILER "/usr/bin/clang++" )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++ -Werror" )
endif()

project( cutDetector CXX )

set( CMAKE_BUILD_TYPE "Debug" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

set( CMAKE_DEBUG_POSTFIX _d )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/out )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )

find_package( OpenCV REQUIRED )

include_directories( include
                     detector
                     rapidjson/include
                     tclap/include
                     ${OpenCV_INCLUDE_DIRS} )

add_subdirectory( detector )
add_subdirectory( gui )

if( CMAKE_BUILD_TYPE MATCHES "Debug" )
  add_subdirectory( googletest )
  add_subdirectory( tests )
endif()

add_executable( cutDetector main.cpp
                include/scene_detector.h
                include/command_line.h
                include/frame_comparator.h
                include/video_reader.h )

target_link_libraries( cutDetector detector )
