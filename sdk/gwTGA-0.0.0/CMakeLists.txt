cmake_minimum_required(VERSION 2.8)

project(gwTGA)

include_directories("${PROJECT_SOURCE_DIR}/include")

set(SRC_FILES src/gwTGA.cpp)

add_library(gwTGA STATIC ${SRC_FILES})
