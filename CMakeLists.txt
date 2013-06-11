cmake_minimum_required(VERSION 2.8.4)
project(Labeling)

include_directories(
    src
    src/gco
    lib
    lib/glm
    lib/eigen
)

set(SOURCES 
    src/libraryLink.c
    src/c_wrapper.cpp
    src/labeling.cpp
    src/connectedComponents.cpp
    src/componentMeasurements.cpp
    src/gco/graph.cpp
    src/gco/maxflow.cpp
    src/gco/GCoptimization.cpp
    src/gco/LinkedBlockList.cpp
)

add_executable(label
    src/main.cpp
    src/libraryLink.c
    src/c_wrapper.cpp
    src/labeling.cpp
    src/connectedComponents.cpp
    src/componentsMeasurements.cpp
    src/gco/graph.cpp
    src/gco/maxflow.cpp
    src/gco/GCoptimization.cpp
    src/gco/LinkedBlockList.cpp
)

add_library(Labeling SHARED
    src/libraryLink.c
    src/c_wrapper.cpp
    src/labeling.cpp
    src/connectedComponents.cpp
    src/componentsMeasurements.cpp
    src/gco/graph.cpp
    src/gco/maxflow.cpp
    src/gco/GCoptimization.cpp
    src/gco/LinkedBlockList.cpp
)

if(APPLE)
    message("\nEntering configuration for APPLE computers:\n")
    set_target_properties(Labeling label
        PROPERTIES
        COMPILE_FLAGS "-g -pipe -Wall"
        LINK_FLAGS "-v -lstdc++"
     )
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}" ${CMAKE_MODULE_PATH})
find_package(Mathematica)
if(${Mathematica_FOUND})
    include_directories("/Applications/Mathematica.app/SystemFiles/IncludeFiles/C" "/Applications/Mathematica.app/SystemFiles/Links/MathLink/DeveloperKit/CompilerAdditions/")
endif()