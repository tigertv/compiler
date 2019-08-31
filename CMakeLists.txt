cmake_minimum_required (VERSION 2.6)

project (COMPILER)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -pedantic)

add_subdirectory(src)
