cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_NAME "buffer_test")
set(CMAKE_CXX_FLAGS "-g")# 断点无效处理方案
project(${PROJECT_NAME})

get_filename_component(test ${CMAKE_SOURCE_DIR} DIRECTORY)
message(STATUS "a_demo's path:${test}")
get_filename_component(DECS ${test} DIRECTORY)
message(STATUS "testcode's path:${DECS}")

set(DECS_SRC "${DECS}/src")

# include
include_directories(${DECS_SRC})
# lib
link_directories()

file(GLOB_RECURSE SRC "${CMAKE_SOURCE_DIR}/main.cpp")
file(GLOB_RECURSE DESC_SRC_link "${DECS_SRC}/net/*.cpp")
add_executable(${PROJECT_NAME} ${DESC_SRC_link} ${SRC})

# 链接库
target_link_libraries(${PROJECT_NAME} "-pthread")