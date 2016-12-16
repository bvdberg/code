cmake_minimum_required(VERSION 2.8)
PROJECT(TLV_Demo)

SET(CMAKE_BUILD_TYPE Release)
#SET(CMAKE_BUILD_TYPE Debug)

#ADD_DEFINITIONS(-Wall -Wextra -Wshadow -Wconversion -Wno-unused-parameter -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)
ADD_DEFINITIONS(-O2 -Wall -Wextra -Wshadow -Wno-unused-parameter -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)

include_directories(".")
include_directories("lib")

ADD_EXECUTABLE(tlv
    utils/Logger.cpp
    utils/StringBuilder.cpp
    utils/Utils.cpp
    main.cpp
)
