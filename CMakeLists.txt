cmake_minimum_required(VERSION 3.10)

# Project name and version
project(HelloWorld VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(assembler
    src/main.cpp
    src/assembler.cpp
    src/emulator.cpp
)

enable_testing()
add_subdirectory(tests)