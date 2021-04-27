cmake_minimum_required(VERSION 3.8)
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

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

CollectSourceFiles(${CMAKE_CURRENT_SOURCE_DIR} PRIVATE_SOURCES)
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(neuronal_networks_server main.cpp)

target_link_libraries(neuronal_networks_server ${CMAKE_THREAD_LIBS_INIT})

if (UNIX AND NOT NOJEM AND NOT APPLE)
    set(neuronal_networks_server_LINK_FLAGS "-pthread -lncurses ${neuronal_networks_server_LINK_FLAGS}")
endif ()

set_target_properties(neuronal_networks_server PROPERTIES LINK_FLAGS "${neuronal_networks_server_LINK_FLAGS}")

CollectIncludeDirectories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC_INCLUDES)

target_include_directories(neuronal_networks_server
        PUBLIC
        ${PUBLIC_INCLUDES}
        PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR})

if( UNIX )
    install(TARGETS neuronal_networks_server DESTINATION bin)
elseif( WIN32 )
    install(TARGETS neuronal_networks_server DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()