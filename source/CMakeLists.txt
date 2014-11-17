cmake_minimum_required (VERSION 2.6)
project (Strategy)

include(../cmake/macros.cmake)

#set_property(GLOBAL PROPERTY USE_FOLDERS On)

SUBDIRLIST(SUBDIRS ${CMAKE_SOURCE_DIR})

FOREACH(subdir ${SUBDIRS})
    ADD_SUBDIRECTORY(${subdir})
ENDFOREACH()

file(GLOB_RECURSE INCS "*.h")
file(GLOB_RECURSE CPPS "*.cpp")

# Additional places where to look for Find*.cmake scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/")

# get subdirectories in sdk folder
SUBDIRLIST(SDK_SUBDIRS "${CMAKE_SOURCE_DIR}/../sdk")

# create a list of sdk subdirs (including sdk folder)
SET(SDK_SUBDIRS_CONCAT "${CMAKE_SOURCE_DIR}/../sdk")
FOREACH(subdir ${SDK_SUBDIRS})
    set(SDK_SUBDIRS_CONCAT ${SDK_SUBDIRS_CONCAT} "${CMAKE_SOURCE_DIR}/../sdk/${subdir}")
ENDFOREACH()
    
print_list_contents("SDK Subdirectories:" SDK_SUBDIRS_CONCAT)

# Additional places where to look for libraries
set(CMAKE_PREFIX_PATH ${SDK_SUBDIRS_CONCAT} CACHE PATH "Paths to dependencies")

IF(MINGW)
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
  endif(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_CXX_STANDARD_LIBRARIES -lstdc++)
  set(CMAKE_CXX_FLAGS "-std=c++11 -m64 -fpermissive")
  set(CMAKE_CXX_FLAGS_RELEASE "-fno-exceptions -fno-rtti -ffast-math -ffunction-sections -fomit-frame-pointer -mavx -ftree-vectorize -O3 -fwhole-program -flto")
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
ENDIF()

#find_package(OpenGL)
find_package(GLM)


################################################################################
# Set all projects to output into single folder (required for shared libs).
################################################################################
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

################################################################################
# glew
################################################################################
#set(GLEW_PATH_TO_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/../sdk/glew" CACHE PATH "Path to directory containing glew source code.")
#set(GLEW_BUILD_SHARED OFF CACHE BOOL "Build glew as shared or static library.")
#if(NOT IS_DIRECTORY ${GLEW_PATH_TO_SOURCE})
#  message(FATAL_ERROR "Incorrect path to source of GLEW: ${GLEW_PATH_TO_SOURCE}")
#endif(NOT IS_DIRECTORY ${GLEW_PATH_TO_SOURCE})
#if(NOT GLEW_BUILD_SHARED)
#  add_definitions(-DGLEW_STATIC)
#endif(NOT GLEW_BUILD_SHARED)
#include_directories(${GLEW_PATH_TO_SOURCE}) # so we can adress glew includes using GL/* format
#add_subdirectory(${GLEW_PATH_TO_SOURCE} ${CMAKE_CURRENT_BINARY_DIR}/glew)

################################################################################
# SDL
################################################################################
Find_Package (SDL2 REQUIRED)

# Workaround for the non-working REQUIRED flag
if (NOT SDL2_FOUND)
  MESSAGE (FATAL_ERROR "SDL2 not found!")
else()
  MESSAGE("SDL2 Include Dir: " ${SDL2_INCLUDE_DIR})
  MESSAGE("SDL2 Library: " ${SDL2_LIBRARY})  
endif (NOT SDL2_FOUND)
   
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${GLM_INCLUDE_DIRS})

#include_directories(${GLEW_INCLUDE_DIRS})
#include_directories(${OPENGL_INCLUDE_DIRS})

add_executable(Strategy WIN32 ${CPPS} ${INCS})
#add_dependencies(Strategy glew)
#target_link_libraries(Strategy glew ${OPENGL_LIBRARIES})
target_link_libraries (Strategy ${SDL2_LIBRARY})

# Set target directory for copying. Can refer to target which hasn't been added yet.
set(COPY_TARGET_DIR $<TARGET_FILE_DIR:Strategy>)
post_build_make_dir_link(Strategy ${PROJECT_SOURCE_DIR}/../assets  ${COPY_TARGET_DIR}/assets)
post_build_make_dir_link(Strategy ${PROJECT_SOURCE_DIR}/GLSL       ${COPY_TARGET_DIR}/shaders)
if(MSVC)
    post_build_make_dir_link(Strategy ${PROJECT_SOURCE_DIR}/../assets  ${PROJECT_BINARY_DIR}/assets)
    post_build_make_dir_link(Strategy ${PROJECT_SOURCE_DIR}/GLSL       ${PROJECT_BINARY_DIR}/shaders)
endif(MSVC)

# Get last entry in SDL libraries list
LIST(GET SDL2_LIBRARY -1 SDL2_LIBRARY_LAST)
get_filename_component(SDL2_LIB_DIR "${SDL2_LIBRARY_LAST}" DIRECTORY)

add_custom_command(TARGET Strategy POST_BUILD   
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        "${SDL2_LIB_DIR}/SDL2.dll"    
        $<TARGET_FILE_DIR:Strategy>)                
  
