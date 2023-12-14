
########################################################################################################################
# test target build                                                                                                    #
########################################################################################################################

file(GLOB_RECURSE TEST_SRC "${CMAKE_SOURCE_DIR}/test/*")
# Exclude specific files
# list(REMOVE_ITEM TEST_SRC "${CMAKE_SOURCE_DIR}/test/base.cpp")

# test dependency
set(test_dependency "-pthread"
                    "-lgtest"
                    "-lgmock" )

# test target
if (generate_test STREQUAL "ON")
    add_executable(${test_name} ${TEST_SRC} ${SRC})
    target_link_libraries(${test_name} ${src_dependency} ${test_dependency})
else()
    message(STATUS "CMakeLists.txt error:test target build error")
endif ()


