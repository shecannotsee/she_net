
########################################################################################################################
# src build                                                                                                            #
########################################################################################################################

# source src
file(GLOB_RECURSE SRC "${CMAKE_SOURCE_DIR}/src/*")
# Exclude specific files
# list(REMOVE_ITEM SRC "${CMAKE_SOURCE_DIR}/src/base64.cpp")

# src dependency
set(src_dependency "-pthread")

# src build to lib
if (generate_lib STREQUAL "ON")
    add_library(${library_static_name} STATIC ${SRC})
    # Rename the generated static library
    set_target_properties(${library_static_name} PROPERTIES OUTPUT_NAME ${project_name})

    add_library(${library_dynamic_name} SHARED ${SRC})
    # Rename the generated dynamic library
    set_target_properties(${library_dynamic_name} PROPERTIES OUTPUT_NAME ${project_name})
else()
    message(STATUS "CMakeLists.txt error:src build error")
endif ()


