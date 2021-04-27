cmake_minimum_required(VERSION 3.5)
project(neuronal_networks_server)

# set macro-directory
list(APPEND CMAKE_MODULE_PATH
        "${CMAKE_SOURCE_DIR}/cmake/macros")

# build in Release-mode by default if not explicitly set
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif ()

# set default buildoptions and print them
include(cmake/options.cmake)

include(GroupSources)
include(AutoCollect)

add_subdirectory(src)
