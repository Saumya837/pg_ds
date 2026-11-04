cmake_minimum_required(VERSION 3.16)
project(bplustree CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(bplustree src/leaf.cpp)
target_include_directories(bplustree PUBLIC include)
target_compile_options(bplustree PRIVATE -Wall -Wextra -fsanitize=address,undefined)
target_link_options(bplustree PUBLIC -fsanitize=address,undefined)

add_executable(test_leaf tests/test_leaf.cpp)
target_link_libraries(test_leaf bplustree)