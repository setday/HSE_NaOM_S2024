cmake_minimum_required(VERSION 3.20)
project(exp)

set(CMAKE_CXX_STANDARD 23)

set(GSL_INCLUDE_DIR "../GSL/include/")
set(GSL_LIB_DIR "../GSL/lib/")

include_directories(${GSL_INCLUDE_DIR})
link_directories(${GSL_LIB_DIR})

add_executable(exp TestExp.cpp)
target_link_libraries(exp -lgsl -lgslcblas)
