CollectSourceFiles(
        ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE_SOURCES)

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_library(common
        ${PRIVATE_SOURCES}
        )

CollectIncludeDirectories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC_INCLUDES)

target_include_directories(common
        PUBLIC
        ${CMAKE_BINARY_DIR}
        ${PUBLIC_INCLUDES}
        server
        PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(common
        PUBLIC
        boost
        server)