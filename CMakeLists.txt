cmake_minimum_required(VERSION 3.10)

# set the project name
project(bftoc VERSION 2.1)

# add the executable
add_executable(bftoc src/bftoc.cpp src/preprocessor.cpp src/preprocessor.h)