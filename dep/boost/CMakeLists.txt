if (WIN32)
    set(BOOST_DEBUG ON)
    if (DEFINED ENV{BOOST_ROOT})
        set(BOOST_ROOT $ENV{BOOST_ROOT})
        list(APPEND BOOST_LIBRARYDIR
                ${BOOST_ROOT}/lib${PLATFORM}-msvc-14.2)
    elseif (NOT DEFINED Boost_DIR AND NOT DEFINED BOOST_ROOT AND NOT DEFINED BOOSTROOT)
        message(FATAL_ERROR "No BOOST_ROOT environment variable could be found! Please make sure it is set and the points to your Boost installation.")
    endif ()

    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
endif ()

if (CMAKE_VERSION VERSION_LESS "3.16.0")
    # boost's own cmake file adds extra compile definitions that cotire fails to pick up
    set(Boost_NO_BOOST_CMAKE ON)
endif ()

find_package(Boost 1.76 REQUIRED system iostreams container filesystem log)

add_library(boost INTERFACE)

target_link_libraries(boost
        INTERFACE
        ${Boost_LIBRARIES})

target_include_directories(boost
        INTERFACE
        ${Boost_INCLUDE_DIRS})
