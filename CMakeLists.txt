cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_NAME "DeviceEnrollmentCloudService")
##############################################################

##############################################################
set(CMAKE_CXX_FLAGS "-g")
project(${PROJECT_NAME})
##############################################################
# include
include_directories(
)
# lib
link_directories(
)

##############################################################
file(GLOB_RECURSE SRC "${CMAKE_SOURCE_DIR}/src/*")
add_executable(${PROJECT_NAME}
        ${SRC}
        )
##############################################################
target_link_libraries(${PROJECT_NAME} pthread)
