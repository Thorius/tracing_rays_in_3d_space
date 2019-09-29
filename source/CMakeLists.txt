cmake_minimum_required(VERSION 3.12.0)

# Mathematics library
add_library(mathy INTERFACE)
# Sources: mathy/vector3.hpp
target_include_directories(mathy INTERFACE mathy)

# Ray-tracing library
add_library(tracey INTERFACE)
# Sources: TODO
target_include_directories(tracey INTERFACE tracey)
target_link_libraries(tracey INTERFACE mathy)

# The actual renderer
add_executable(renderer driver.cpp)
target_link_libraries(renderer PRIVATE mathy tracey tiny_png_out)
target_compile_features(renderer PRIVATE cxx_std_17)
