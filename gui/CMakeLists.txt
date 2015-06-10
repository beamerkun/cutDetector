set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )

find_package( Qt5Core )
find_package( Qt5Widgets )

add_library( gui SHARED
             comparator_options_dialog.cpp
             comparator_options_dialog.h
             comparator_options_dialog.ui
             cut_detector_qt_interface.cpp
             cut_detector_qt_interface.h
             gui.cpp
             gui.hpp
             main_window.cpp
             main_window.h
             main_window.ui
             opencv_image_widget.cpp
             opencv_image_widget.h
             scene_list_preview_dialog.cpp
             scene_list_preview_dialog.h
             scene_list_preview_dialog.ui )

target_link_libraries( gui Qt5::Core
                           Qt5::Widgets )
