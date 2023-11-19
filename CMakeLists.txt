cmake_minimum_required(VERSION 3.25)

# https://stackoverflow.com/questions/13703647/how-to-properly-add-include-directories-with-cmake

project(tetraOS C CXX ASM)
add_subdirectory(Kernel)
add_subdirectory(LibC)

