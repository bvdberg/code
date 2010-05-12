cmake_minimum_required(VERSION 2.8)
PROJECT(Binary-include)
SET(CMAKE_BUILD_TYPE Release)
#SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_VERBOSE_MAKEFILE false)
ADD_DEFINITIONS(-Wall -Wextra)

add_executable(MakeTable maketable.c)

add_custom_command (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/file.h
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/MakeTable ${CMAKE_SOURCE_DIR}/file.bin ${CMAKE_CURRENT_BINARY_DIR}/file.h
    DEPENDS MakeTable
)
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

ADD_EXECUTABLE (test
  main.c
  ${CMAKE_CURRENT_BINARY_DIR}/file.h
)


  
