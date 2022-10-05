cmake_minimum_required(VERSION 3.13)
project(net)

file(GLOB_RECURSE SRC "src/*.cpp")

add_executable(net ${SRC})