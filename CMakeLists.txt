cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_CXX_STANDARD 20)

project(Chip8)

add_subdirectory(Libraries)
add_subdirectory(Emulator)
add_subdirectory(Sandbox)