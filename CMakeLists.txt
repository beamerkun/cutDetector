cmake_minimum_required(VERSION 2.8.11)

project( cutDetector CXX )

set( CMAKE_BUILD_TYPE Release )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_EXE_LINKER_FLAGS "-static")

find_package( OpenCV REQUIRED )

include_directories( include
                     detector
                     rapidjson/include
                     tclap/include
                     ${OpenCV_INCLUDE_DIRS} )

add_subdirectory( detector )
add_subdirectory( gui )

add_executable( cutDetector main.cpp
                include/scene_detector.h
                include/command_line.h
                include/frame_comparator.h
                include/video_reader.h )

target_link_libraries( cutDetector detector
                                   gui )
