# Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
# Licensed under MIT License
# Full text avaliable in LICENSE file located in project root
# http://github.com/beamerkun/cutDetector/blob/master/LICENSE
add_library(detector SHARED
    adjacent_frames_scene_detector.cpp
    adjacent_frames_scene_detector.hpp
    command_line_debug.cpp
    command_line_debug.hpp
    command_line_impl.cpp
    command_line_impl.hpp
    command_line_strings.cpp
    command_line_strings.hpp
    cut_detector.hpp
    cut_detector.cpp
    frame_comparator_strings.cpp
    frame_comparator_strings.hpp
    histogram_based_frame_comparator.cpp
    histogram_based_frame_comparator.hpp
    open_cv_video_reader.cpp
    open_cv_video_reader.hpp )

target_link_libraries( detector ${OpenCV_LIBS} )
