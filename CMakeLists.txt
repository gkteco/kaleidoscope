cmake_minimum_required(VERSION 3.15)

project(kalec)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(kalec src/main.cpp src/parser.cpp src/lexer.cpp)

target_include_directories(kalec PUBLIC include)