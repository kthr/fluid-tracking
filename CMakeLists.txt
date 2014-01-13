cmake_minimum_required(VERSION 2.8.10)
project(Labeling)

set(SOURCES 
    src/alg/labeling.cpp
    src/alg/connectedComponents.cpp
    src/alg/componentsMeasurements.cpp
    src/alg/fluidTracks.cpp
    src/alg/graphcut.cpp
    src/io/xmlExport.cpp
    src/logic/annotation.cpp
    src/logic/frame.cpp
    src/logic/link.cpp
    src/logic/object.cpp
    src/logic/trackingData.cpp
    lib/gco/graph.cpp
    lib/gco/maxflow.cpp
    lib/gco/GCoptimization.cpp
    lib/gco/LinkedBlockList.cpp
    lib/maxflow/graph.cpp
    lib/maxflow/maxflow.cpp
    src/utils/parameters.cpp
    src/utils/rle.cpp
    src/utils/utilities.cpp
    src/utils/vectorArray2D.cpp
)

add_custom_target(
    Version
    COMMAND /bin/bash version.sh
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_executable(fluidTracking
    src/fluidTracking.cpp
    ${SOURCES}
)
add_dependencies(fluidTracking Version)

include_directories(
    src/
    lib
    /usr/include/libxml2
)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost COMPONENTS filesystem system regex program_options) 
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(fluidTracking ${Boost_LIBRARIES} X11 xml2 pthread)                                                                                                                                                                                                                     
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4.8
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
        message(FATAL_ERROR "GCC version must be at least 4.7!")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # require at least clang 3.2
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.2)
        message(FATAL_ERROR "Clang version must be at least 3.2!")
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC.")
endif()

if(UNIX AND NOT APPLE)
    message("\nEntering configuration for UNIX computers:\n")
    set(CMAKE_CXX_FLAGS "-g -pipe -Wall -std=c++11")
    set_target_properties(fluidTracking
        PROPERTIES
        LINK_FLAGS ""
     )
endif()

if(APPLE)
    set(CMAKE_CXX_COMPILER g++-mp-4.8)
    message("\nEntering configuration for APPLE computers:\n")
    set(CMAKE_CXX_FLAGS "-g -pipe -Wall -I/opt/local/include -std=c++11")
    set_target_properties(fluidTracking
        PROPERTIES
        LINK_FLAGS "-v -L/opt/X11/lib"
     )
endif()


