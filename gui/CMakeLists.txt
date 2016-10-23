# Copyright (c) 2014-2016 Jakub 'beamerkun' Sokół
# Licensed under MIT License
# Full text avaliable in LICENSE file located in project root
# http://github.com/beamerkun/cutDetector/blob/master/LICENSE
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )

if(WIN32)
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows" )
endif()

find_package( Qt5Widgets REQUIRED )
find_package( Qt5PrintSupport REQUIRED )
find_package( OpenCV REQUIRED )

set( gui_library_sources
      comparator_options_dialog.cpp
      comparator_options_dialog.h
      comparator_options_dialog.ui
      cut_detector_qt_interface.cpp
      cut_detector_qt_interface.h
      main_window.cpp
      main_window.h
      main_window.ui
      opencv_image_widget.cpp
      opencv_image_widget.h
      scene_list_preview_dialog.cpp
      scene_list_preview_dialog.h
      scene_list_preview_dialog.ui )

set( gui_library_sources ${gui_library_sources}
                         QCustomPlot_gen/qcustomplot.cpp
                         QCustomPlot_gen/qcustomplot.h )
include_directories( QCustomPlot_gen )

add_library( gui SHARED ${gui_library_sources} )

target_link_libraries( gui detector
                           Qt5::Core
                           Qt5::Widgets
                           Qt5::PrintSupport
                           ${OpenCV_LIBS} )

add_executable( cutDetector_gui main_gui.cpp)

target_link_libraries( cutDetector_gui gui )
