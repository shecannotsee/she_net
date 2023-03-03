########################################################################################################################
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g")# to deal clion debug invalid

set(PROJECT_NAME "she_net" VERSION 0.0.1)
set(CMAKE_BUILD_TYPE "RELEASE") # Release or Debug or MinSizeRel
project(${PROJECT_NAME})

########################################################################################################################
set(unit_test "ON")
set(library "ON")

# test set
if (unit_test STREQUAL "ON")
    set(unit_test_name "${PROJECT_NAME}_test")
else()
    message(STATUS "CMakeLists.txt error")
endif ()

# lib set
if (library STREQUAL "ON")
    set(library_static_name "${PROJECT_NAME}_static")
    set(library_dynamic_name "${PROJECT_NAME}_dynamic")
else()
    message(STATUS "CMakeLists.txt error")
endif ()
#message(STATUS "${unit_test_name}")
#message(STATUS "${library_static_name}")
#message(STATUS "${library_dynamic_name}")

########################################################################################################################
# set path

########################################################################################################################
# include
include_directories(${CMAKE_SOURCE_DIR}/src)
# lib
link_directories()

########################################################################################################################
file(GLOB_RECURSE SRC "${CMAKE_SOURCE_DIR}/src/*")
# test target
if (unit_test STREQUAL "ON")
    file(GLOB_RECURSE TEST_SRC "${CMAKE_SOURCE_DIR}/test/*")
    add_executable(${unit_test_name} ${TEST_SRC} ${SRC})
    target_link_libraries(${unit_test_name} "-pthread")
else()
    message(STATUS "CMakeLists.txt error")
endif ()

# lib target
if (library STREQUAL "ON")
    add_library(${library_static_name} STATIC ${SRC})
    # Rename the generated static library
    set_target_properties(${library_static_name} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
    add_library(${library_dynamic_name} SHARED ${SRC})
    # Rename the generated dynamic library
    set_target_properties(${library_dynamic_name} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
else()
    message(STATUS "CMakeLists.txt error")
endif ()

########################################################################################################################
# local debug
set(CMAKE_INSTALL_PREFIX "./")
# Release Code
#set(CMAKE_INSTALL_PREFIX "/")

message(STATUS "The default installation path is ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Please use \"make install DESTDIR=./PATH\" to set install path")

install(TARGETS
        ${unit_test_name}
        DESTINATION
        ${PROJECT_NAME}/bin
        )

# To use make install to build library
# install include
install(FILES
        ${CMAKE_SOURCE_DIR}/src/sheBase64.h DESTINATION ${PROJECT_NAME}/include
        )
# install lib
install(TARGETS
        ${library_static_name} ${library_dynamic_name} DESTINATION ${PROJECT_NAME}/lib
        )


