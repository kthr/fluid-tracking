cmake_minimum_required(VERSION 2.8.4)
project(Labeling)

set(SOURCES 
    src/libraryLink.c
    src/c_wrapper.cpp
    src/labeling.cpp
    src/alg/connectedComponents.cpp
    src/alg/componentsMeasurements.cpp
    src/alg/fluidTracks.cpp
    src/utils/parameters.cpp
    src/gco/graph.cpp
    src/gco/maxflow.cpp
    src/gco/GCoptimization.cpp
    src/gco/LinkedBlockList.cpp
)

add_executable(label
    src/main.cpp
    ${SOURCES}
)

add_library(Labeling SHARED
    ${SOURCES}
)

include_directories(
    src/gco
    lib
    lib/glm
    lib/eigen   
)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}" ${CMAKE_MODULE_PATH})
find_package(Mathematica)
if(${Mathematica_WolframLibrary_FOUND})
    include_directories(${Mathematica_WolframLibrary_INCLUDE_DIR})
else(
	message(ERROR "Mathematica not found")
)
endif()

set(CMAKE_CXX_FLAGS
    "-g -pipe -Wall -I/opt/local/include -std=c++11 -stdlib=libc++"
)
set(CMAKE_C_FLAGS
    "-g -pipe -Wall -I/opt/local/include"
)

if(APPLE)
    message("\nEntering configuration for APPLE computers:\n")
    set_target_properties(Labeling label
        PROPERTIES
        #COMPILE_FLAGS "-g -pipe -Wall -I/opt/local/include"
        LINK_FLAGS "-v -lstdc++ -lpthread  -L/opt/X11/lib -lX11"
     )
endif()


