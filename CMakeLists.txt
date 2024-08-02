cmake_minimum_required(VERSION 3.22)

project(risc-v-simulate)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

include_directories(utility)

file(GLOB sources RELATIVE ${PROJECT_SOURCE_DIR} src/*.cpp)

add_executable(code ${sources})
#add_definitions(-DDEBUG)