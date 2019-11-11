# CMakeList.txt : CMake project for WindStream, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(SOURCES Test.c)

add_executable(WindTest ${SOURCES})