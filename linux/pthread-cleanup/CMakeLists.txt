SET(CMAKE_EXE_LINKER_FLAGS -Wall)
ADD_DEFINITIONS(-Wall)

ADD_EXECUTABLE(test
  main.cpp
  Manager.cpp
)

TARGET_LINK_LIBRARIES(test
  pthread
)

