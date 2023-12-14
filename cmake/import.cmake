
########################################################################################################################
# Import lib                                                                                                           #
########################################################################################################################

# include
include_directories(${CMAKE_SOURCE_DIR}/third_party/googletest/include)

# lib
link_directories(${CMAKE_SOURCE_DIR}/third_party/googletest/lib)

# Internal project
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/test)
include_directories(${CMAKE_SOURCE_DIR}/exec)


