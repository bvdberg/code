cmake_minimum_required(VERSION 2.8)
PROJECT(example)
SET(CMAKE_BUILD_TYPE Release)
#SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_VERBOSE_MAKEFILE false)
ADD_DEFINITIONS(-Wall -Wextra -Wshadow -Wconversion -Wno-unused-parameter -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)

ADD_EXECUTABLE(udp-server
  UdpServer.cpp
)

ADD_EXECUTABLE(udp-client
  UdpClient.cpp
)

