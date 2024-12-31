cmake_minimum_required(VERSION 3.10)

project(SynthesisSolution)

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)

# Include directories for header files
include_directories(include)

# Collect all source files
file(GLOB SOURCES "src/*.cpp")

# Define the executable
add_executable(synthesis ${SOURCES})

