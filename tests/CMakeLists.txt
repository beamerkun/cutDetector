find_package( OpenCV REQUIRED )

enable_testing()
include_directories( ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} )

add_executable( runUnitTests testgtest.cpp
                             frame_comparator_mock.hpp
                             frame_comparator_test.cpp
                             scene_detector_mock.hpp
                             scene_detector_test.cpp
                             video_reader_mock.hpp
                             video_reader_test.cpp )

target_link_libraries( runUnitTests gmock
                                    gmock_main
                                    gtest
                                    gtest_main
                                    detector )

add_custom_command( TARGET runUnitTests POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/tests/res
                    $<TARGET_FILE_DIR:runUnitTests>/res )

add_test( runUnitTests runUnitTests )
