cmake_minimum_required(VERSION 3.13)
project(net)

file(GLOB_RECURSE SRC "src/*.cpp")

add_executable(net ${SRC})

if(WIN32)
    target_link_libraries(net PRIVATE wsock32 ws2_32)
endif()