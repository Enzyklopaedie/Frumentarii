cmake_minimum_required(VERSION 3.10)
project(frumentarii)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include-Ordner
include_directories(include src)

# Executable
add_executable(frumentarii
    src/main.cpp
    src/arp.cpp
    src/network.cpp
)
