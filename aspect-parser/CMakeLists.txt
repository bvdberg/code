ADD_DEFINITIONS(-Wall -Wextra)
SET(CMAKE_BUILD_TYPE Release)
#SET(CMAKE_BUILD_TYPE Debug)


ADD_EXECUTABLE(parser
  main.cpp
)
TARGET_LINK_LIBRARIES(parser aspectparser)


ADD_LIBRARY(aspectparser STATIC
  AspectParser.cpp
)

SUBDIRS(test)

