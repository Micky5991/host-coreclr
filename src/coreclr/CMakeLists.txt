cmake_minimum_required(VERSION 3.10)
project(host-coreclr VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(coreclr core_clr.cpp)

target_include_directories(coreclr PUBLIC ../../include)
target_include_directories(coreclr PRIVATE include/)
target_compile_features(coreclr PUBLIC cxx_std_17)

