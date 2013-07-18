cmake_minimum_required(VERSION 2.8.4)
project(Labeling)

set(SOURCES 
    src/libraryLink.c
    src/c_wrapper.cpp
    src/alg/labeling.cpp
    src/alg/connectedComponents.cpp
    src/alg/componentsMeasurements.cpp
    src/alg/fluidTracks.cpp
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
    src/utils/parameters.cpp
    src/utils/rle.cpp
    src/utils/utilities.cpp
    src/utils/vectorArray2D.cpp
)

add_executable(fluidTracking
    src/fluidTracking.cpp
    ${SOURCES}
)

add_library(Labeling SHARED
    ${SOURCES}
)

include_directories(
    src/
    lib
    /usr/include/libxml2
)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}" ${CMAKE_MODULE_PATH})
find_package(Mathematica)
if(${Mathematica_WolframLibrary_FOUND})
    include_directories(${Mathematica_WolframLibrary_INCLUDE_DIR})
else(
	message(ERROR "Mathematica not found")
)
endif()

if(WIN32)
        # Execute Win32 Specific commands - none yet.
else(WIN32)
    # Execute commands for other platforms
    message("Create command")
    add_custom_command(
        OUTPUT revision
        COMMAND ${CMAKE_SOURCE_DIR}/version.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Creates/updates git Revision Header"
    )
endif(WIN32)
message(${CMAKE_SOURCE_DIR}/version.sh)
add_custom_target(version ALL DEPENDS revision)

set(CMAKE_CXX_FLAGS
    "-g -pipe -Wall -I/Users/kthierbach/dev/boost-c++11/include -I/opt/local/include -std=c++11 -stdlib=libc++"
)
set(CMAKE_C_FLAGS
    "-g -pipe -Wall -I/Users/kthierbach/dev/boost-c++11/include"
)

if(APPLE)
    message("\nEntering configuration for APPLE computers:\n")
    set_target_properties(Labeling fluidTracking
        PROPERTIES
        LINK_FLAGS "-v -lstdc++ -lpthread  -L/opt/X11/lib -lX11 -lxml2 -L/Users/kthierbach/dev/boost-c++11/lib -lboost_program_options -lboost_regex -lboost_filesystem -lboost_system"
     )
endif()


