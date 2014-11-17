cmake_minimum_required(VERSION 2.8)

project(glew C)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})
include_directories("${PROJECT_SOURCE_DIR}/include")
if(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif(UNIX)

if(WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DVC_EXTRALEAN)
    add_definitions(-DWIN32)
endif(WIN32)

set(GLEW_BUILD_SHARED OFF CACHE BOOL "Build glew as shared or static library.")
if(GLEW_BUILD_SHARED)
    set(GLEW_LIB_TYPE SHARED)
    add_definitions(-DGLEW_BUILD)
else(GLEW_BUILD_SHARED)
    set(GLEW_LIB_TYPE STATIC)
    add_definitions(-DGLEW_STATIC)
endif(GLEW_BUILD_SHARED)

set(GLEW_FILES src/glew.c)
if(MSVC)
    set(GLEW_FILES ${GLEW_FILES}
                   include/GL/glew.h
                   include/GL/glxew.h
                   include/GL/wglew.h)
endif(MSVC)

add_library(glew ${GLEW_LIB_TYPE} ${GLEW_FILES})
target_link_libraries(glew ${OPENGL_LIBRARIES})
