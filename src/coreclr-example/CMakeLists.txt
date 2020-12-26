cmake_minimum_required(VERSION 3.10)
project(coreclr-example LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/dist)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(coreclr-example main.cpp)

target_compile_features(coreclr-example PUBLIC cxx_std_17)

target_link_libraries(coreclr-example PRIVATE coreclr)

target_include_directories(coreclr-example PRIVATE ../../include)

# Build dotnet project
add_custom_command(TARGET coreclr-example PRE_BUILD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dotnet/
        COMMAND dotnet build
        --nologo
        -o $<TARGET_FILE_DIR:coreclr-example>/plugins/)

add_custom_command(TARGET coreclr-example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/runtime/ $<TARGET_FILE_DIR:coreclr-example>/runtime/)