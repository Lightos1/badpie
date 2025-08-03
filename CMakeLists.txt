cmake_minimum_required(VERSION 3.31)
project(badpie C)

set(CMAKE_C_STANDARD 11)

# Add libnx include path for IntelliSense
include_directories("C:/devkitPro/libnx/include")

add_executable(badpie source/main.c)
