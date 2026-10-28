cmake_minimum_required(VERSION 3.20)
project(OpenGL_Sample VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS "CMake")

set(OUTPUT_DIR_BASE "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR_BASE}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR_BASE}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR_BASE}/lib")

add_subdirectory(dependencies)

file(GLOB_RECURSE Render_Source CONFIGURE_DEPENDS
        "src/*.cpp"
        "src/*.h"
)

list(FILTER Render_Source EXCLUDE REGEX "src/main\\.cpp$")

add_executable(OpenGL_Sample
        src/main.cpp
        ${Render_Source}
)

target_include_directories(OpenGL_Sample PRIVATE
        "${PROJECT_SOURCE_DIR}/src"
        "${PROJECT_SOURCE_DIR}/dependencies/glew-2.3.1/include"
        "${PROJECT_SOURCE_DIR}/dependencies/glm/glm"
        "${PROJECT_SOURCE_DIR}/dependencies/stb_image"
)

target_link_libraries(OpenGL_Sample PRIVATE glfw OpenGL::GL glew_s stb_image glm)