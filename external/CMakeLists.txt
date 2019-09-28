cmake_minimum_required(VERSION 3.12.0)

add_library(tiny_png_out TinyPngOut-cpp/TinyPngOut.cpp TinyPngOut-cpp/TinyPngOut.hpp)
target_include_directories(tiny_png_out INTERFACE TinyPngOut-cpp)

add_executable(tiny_png_simple_example TinyPngOut-cpp/SimplePng.cpp)
target_link_libraries(tiny_png_simple_example PRIVATE tiny_png_out)
