cmake_minimum_required(VERSION 3.20)
project(exp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GSL REQUIRED)
link_libraries(GSL::gsl)

add_executable(exp
        TestExp.cpp
)