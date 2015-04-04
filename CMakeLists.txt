cmake_minimum_required(VERSION 2.8.11)

project( cutDetector CXX )

set( CMAKE_CXX_COMPILER "/usr/bin/clang++" )
set( CMAKE_BUILD_TYPE Debug )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra" )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} 
                     include
                     detector
                     rapidjson/include
                     tclap/include )

add_subdirectory( detector )
add_subdirectory( gui )

add_executable( cutDetector main.cpp
                include/scene_detector.h
                include/command_line.h
                include/frame_comparator.h
                include/video_reader.h )

target_link_libraries( cutDetector ${OpenCV_LIBS}
                                   detector
                                   gui )
