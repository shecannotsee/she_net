
############################################################################################################
# exec target build
############################################################################################################

# exec src
file(GLOB_RECURSE exec_SRC "${CMAKE_SOURCE_DIR}/exec/*")
# Exclude specific files
# list(REMOVE_ITEM exec_SRC "${CMAKE_SOURCE_DIR}/exec/base64.cpp")

# exec dependency
set(exec_dependency "-pthread")

# exec target
if (generate_exec STREQUAL "ON")
    add_executable(${exec_name} ${exec_SRC} ${SRC})
    target_link_libraries(${exec_name} ${lib} ${src_dependency} ${exec_dependency})
else()
    message(STATUS "CMakeLists.txt error: exec target build error")
endif ()


