cmake_minimum_required(VERSION 3.13)
project(net)

file(GLOB_RECURSE SRC "src/*.cpp" "lib/*.cpp")

add_executable(net ${SRC})

target_include_directories(net PRIVATE "lib/")

if(WIN32)
    target_link_libraries(net PRIVATE wsock32 ws2_32)
else()
    target_compile_options(net PRIVATE -Wall -pedantic)
endif()