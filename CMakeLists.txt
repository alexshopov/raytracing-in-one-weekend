cmake_minimum_required(VERSION 3.0.0)
project(raytracing-in-one-weekend VERSION 0.1.0 LANGUAGES C CXX)

# configure for C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

add_executable(raytracing-in-one-weekend main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
