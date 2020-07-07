cmake_minimum_required(VERSION 3.16)
project(RISC_V_simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "")
add_executable(simulate src/main.cpp src/memory.h src/cpu.h src/typedef.h src/memory.cpp src/cpu.cpp src/constant.h)