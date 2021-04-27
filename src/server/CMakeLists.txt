CollectSourceFiles(
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE_SOURCES)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_library(server
        ${PRIVATE_SOURCES}
        models/Madeline.h models/AllModels.h)

target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})

CollectIncludeDirectories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC_INCLUDES)

target_include_directories(server
        PUBLIC
        ${PUBLIC_INCLUDES}
        common
        PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(server
        PUBLIC
        common)