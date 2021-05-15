CollectSourceFiles(
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE_SOURCES)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

add_executable(neuronal_networks_server
        ${PRIVATE_SOURCES}
        )

target_link_libraries(neuronal_networks_server ${CMAKE_THREAD_LIBS_INIT} common server)

if (UNIX AND NOT NOJEM AND NOT APPLE)
    set(neuronal_networks_server_LINK_FLAGS "-pthread ${neuronal_networks_server_LINK_FLAGS}")
endif ()

set_target_properties(neuronal_networks_server PROPERTIES LINK_FLAGS "${neuronal_networks_server_LINK_FLAGS}")


CollectIncludeDirectories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC_INCLUDES)

target_include_directories(neuronal_networks_server
        PUBLIC
        ${PUBLIC_INCLUDES}
        common
        server
        PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR})

ADD_CUSTOM_COMMAND(TARGET neuronal_networks_server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/neuronalserver.conf" "${CMAKE_CURRENT_BINARY_DIR}")
