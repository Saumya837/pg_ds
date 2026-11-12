cmake_minimum_required(VERSION 3.16)
project(extsort CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(extsort src/page_io.cpp)
target_include_directories(extsort PUBLIC include)
target_compile_options(extsort PRIVATE -Wall -Wextra -fsanitize=address,undefined)
target_link_options(extsort PUBLIC -fsanitize=address,undefined)

add_executable(gen tools/gen.cpp)
