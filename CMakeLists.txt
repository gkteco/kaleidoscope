cmake_minimum_required(VERSION 3.10)
project(kalec)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Set the C++ standard you wish to use (you'll need at least C++11)
set(CMAKE_CXX_STANDARD 20)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

# Include LLVM directories
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Add your source files
add_executable(kalec src/llvm_wrapper.cpp src/main.cpp src/lexer.cpp src/parser.cpp src/ast.cpp)
target_include_directories(kalec PUBLIC "${PROJECT_BINARY_DIR}" include/)


# Link against LLVM libraries
llvm_map_components_to_libnames(llvm_libs support core native orcjit irreader)
target_link_libraries(kalec ${llvm_libs})