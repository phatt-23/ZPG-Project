cmake_minimum_required(VERSION 3.30)

project(ImGui)

file(GLOB sources *.cpp *.h)

add_library(ImGui STATIC
        ${sources}
        backends/imgui_impl_glfw.h
        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_opengl3.h
        backends/imgui_impl_opengl3.cpp
        backends/imgui_impl_opengl3_loader.h
)

target_include_directories(ImGui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/backends
)