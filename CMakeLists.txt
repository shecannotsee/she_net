
cmake_minimum_required(VERSION 3.11)

########################################################################################################################
# Project Settings                                                                                                     #
########################################################################################################################

set(project_name "she_net")
PROJECT(${project_name}
        VERSION      0.0.1
        LANGUAGES    CXX
        )

# [Release] or [Debug] or [MinSizeRel]
set(compilation_options "Debug")

set(library_static_name "${project_name}_static")
set(library_dynamic_name "${project_name}_dynamic")

########################################################################################################################
# include                                                                                                              #
########################################################################################################################

include(${CMAKE_SOURCE_DIR}/cmake/compilation_options.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/config.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/import.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/src_build.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/target_exec.cmake )
include(${CMAKE_SOURCE_DIR}/cmake/target_test.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/z_install.cmake)


