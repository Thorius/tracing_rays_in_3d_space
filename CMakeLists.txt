cmake_minimum_required(VERSION 3.12.0)

# Always create a compile if the generator supports it.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(tracing_rays_in_3d_space)

add_subdirectory(external)

add_subdirectory(source)
