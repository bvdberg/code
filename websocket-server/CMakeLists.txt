cmake_minimum_required(VERSION 2.8)
PROJECT(ws_server)
SET(CMAKE_BUILD_TYPE Release)
#SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_VERBOSE_MAKEFILE false)
#ADD_DEFINITIONS(-Wall -Wextra -Wshadow)
ADD_DEFINITIONS(-Wall -Wextra -Wshadow -Wno-unused -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)

#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

ADD_EXECUTABLE(ws_server
    base64.cpp
    sha-1.cpp
    WebSocketServer.cpp
    main.cpp
)

