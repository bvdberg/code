cmake_minimum_required(VERSION 2.8)
PROJECT(mytool)
include_directories(".")

SET(CMAKE_CXX_COMPILER "clang++-3.5")

SET(CMAKE_BUILD_TYPE Release)
#SET(CMAKE_BUILD_TYPE Debug)

SET(CMAKE_VERBOSE_MAKEFILE true)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O1 -g -fsanitize=address -fno-omit-frame-pointer -Wall -Wextra -Wshadow -Wno-unused-parameter -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS")
SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -O1 -g -fsanitize=address -fno-omit-frame-pointer -Wall -Wextra -Wshadow -Wno-unused-parameter -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -fsanitize=address")


ADD_EXECUTABLE(mytool
    main.cpp
    otherfile.cpp
)

