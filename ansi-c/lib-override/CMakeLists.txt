PROJECT(Chronos-engine)
SET(VERBOSE true)
SET(CMAKE_BUILD_TYPE Release)
#SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_VERBOSE_MAKEFILE true)
ADD_DEFINITIONS(-Wall -Wextra)

ADD_EXECUTABLE(test
  main.cpp
)
TARGET_LINK_LIBRARIES(test lib1)


ADD_LIBRARY(lib1 SHARED
    lib1.cpp
)

ADD_LIBRARY(override SHARED
    override.cpp
)

