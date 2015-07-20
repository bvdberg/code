cmake_minimum_required(VERSION 2.8)
PROJECT(shared-lib)
SET(CMAKE_BUILD_TYPE Release)
#SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_VERBOSE_MAKEFILE false)
SET(CMAKE_VERBOSE_MAKEFILE true)
ADD_DEFINITIONS(-Wall -Wextra)

ADD_EXECUTABLE(foo
  main.c
)
TARGET_LINK_LIBRARIES(foo mylib )


ADD_LIBRARY(mylib SHARED
  mod1.c
)
#TARGET_LINK_LIBRARIES(chronos sqlite3 pthread lua)

#SUBDIRS(sqlite lua test)

