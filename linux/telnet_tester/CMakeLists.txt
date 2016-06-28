cmake_minimum_required(VERSION 2.8)

ADD_DEFINITIONS(-Wall -Wextra -Wshadow -Wno-unused-parameter -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS)

include_directories(".")

ADD_EXECUTABLE(tt
    client.c
)

