set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )

find_package( Qt5Core )
find_package( Qt5Widgets )

add_library( gui SHARED
             gui.cpp
             gui.hpp
             opencv_image_widget.cpp
             opencv_image_widget.h
             main_window.cpp
             main_window.h
             main_window.ui )

target_link_libraries( gui Qt5::Core
                           Qt5::Widgets )
