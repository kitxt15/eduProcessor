cmake_minimum_required(VERSION 3.16)

project(eduVirtualMachine LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Keep the project root on the include path for every target and subdirectory,
# so headers such as colors.h can be included by name from anywhere in the tree.
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(assembler asm.cpp)
add_executable(processor processor.cpp video.cpp)
add_executable(disassembler disasm.c)

find_package(raylib CONFIG QUIET)
if(TARGET raylib)
    target_link_libraries(processor PRIVATE raylib)
endif()
