cmake_minimum_required(VERSION 2.8.7)

project(test)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

add_library(bar SHARED
    mod2.c
)
target_link_libraries(bar m)

add_library(foo STATIC
    mod1.c
)
target_link_libraries(foo bar)

add_executable(test
    main.c
)

target_link_libraries(test foo)

