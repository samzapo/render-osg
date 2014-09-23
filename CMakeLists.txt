cmake_minimum_required(VERSION 2.8.3)

project(render-osg)

include (CheckIncludeFiles)
include (CheckLibraryExists)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMakeModules)
set (PROJECT_SRC_DIR .)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_executable(${PROJECT_NAME} render.cpp)
target_link_libraries(${PROJECT_NAME} osg osgDB osgGA osgViewer OpenThreads)

install (TARGETS ${PROJECT_NAME} DESTINATION bin)
