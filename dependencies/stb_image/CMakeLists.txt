cmake_minimum_required(VERSION 3.20)
project(stb_image LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(stb_image STATIC
        stb_image.cpp
        stb_image.h
)

target_include_directories(stb_image PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)