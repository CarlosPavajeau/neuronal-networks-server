cmake_minimum_required(VERSION 3.19)
project(neuronal_networks_server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

add_executable(neuronal_networks_server main.cpp Message.hpp Session.hpp Server.hpp Opcodes.hpp Define.hpp)

target_link_libraries(neuronal_networks_server ${CMAKE_THREAD_LIBS_INIT})

if(UNIX AND NOT NOJEM AND NOT APPLE)
    set(neuronal_networks_server_LINK_FLAGS "-pthread -lncurses ${neuronal_networks_server_LINK_FLAGS}")
endif()

set_target_properties(neuronal_networks_server PROPERTIES LINK_FLAGS "${neuronal_networks_server_LINK_FLAGS}")