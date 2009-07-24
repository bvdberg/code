PROJECT(Chronos-engine)
SET(CMAKE_BUILD_TYPE Release)
#SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_VERBOSE_MAKEFILE false)
ADD_DEFINITIONS(-Wall -Wextra)

ADD_EXECUTABLE(test
  main.cpp
)
TARGET_LINK_LIBRARIES(test normal)


ADD_LIBRARY(normal SHARED
    normal.cpp
)

ADD_LIBRARY(override SHARED
    override.cpp
)

