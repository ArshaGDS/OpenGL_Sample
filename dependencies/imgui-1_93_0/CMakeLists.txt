cmake_minimum_required(VERSION 3.20)
project(ImGui VERSION 1.93.0 LANGUAGES CXX)

add_library(ImGui STATIC
        imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_tables.cpp
        imgui_widgets.cpp
        imgui_impl_opengl3.h
        imgui_impl_opengl3.cpp
        imgui_impl_glfw.h
        imgui_impl_glfw.cpp
        imgui_impl_opengl3_loader.h
)
add_library(ImGui::ImGui ALIAS ImGui)

target_include_directories(ImGui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(ImGui PUBLIC cxx_std_20)
set_target_properties(ImGui PROPERTIES
        CXX_EXTENSIONS OFF
        POSITION_INDEPENDENT_CODE ON
)

if(MSVC)
    target_compile_options(ImGui PRIVATE /W4 /utf-8)
    target_compile_definitions(ImGui PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(ImGui PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()

target_link_libraries(ImGui PRIVATE glfw)