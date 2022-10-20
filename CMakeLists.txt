cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_NAME "DeviceEnrollmentCloudService")
##############################################################
# set(CMAKE_CXX_FLAGS "-g")
project(${PROJECT_NAME})
##############################################################
set(src_head "${CMAKE_SOURCE_DIR}/src")
set(3rd "${CMAKE_SOURCE_DIR}/3rd")
##############################################################
set(sheIni_src "${3rd}/sheIni/src")
set(spdlog "${3rd}/spdlog")
message(STATUS "spdlog: ${spdlog}")
##############################################################
# include
include_directories(
        ${src_head}
        ${sheIni_src}
        ${spdlog}/include
)
# lib
link_directories(
)

##############################################################
file(GLOB_RECURSE SRC "${CMAKE_SOURCE_DIR}/src/*")
file(GLOB_RECURSE sheIni_part1 "${sheIni_src}/DataStructure/*.cpp")
file(GLOB_RECURSE sheIni_part2 "${sheIni_src}/Parse/*.cpp")
add_executable(${PROJECT_NAME}
        ${SRC}
        ${sheIni_part1}
        ${sheIni_part2}
        )
##############################################################
target_link_libraries(${PROJECT_NAME} "-pthread")
