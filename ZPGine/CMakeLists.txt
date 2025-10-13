cmake_minimum_required(VERSION 3.30)
project(ZPGine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE engine_headers "src/**.h")
file(GLOB_RECURSE engine_sources "src/**.cpp")

add_library(ZPGine STATIC
        ${engine_headers}
        ${engine_sources}
        ZPGine.h
)

target_include_directories(ZPGine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor
)

target_precompile_headers(ZPGine PUBLIC
        src/Core/PCH.h
)

target_compile_definitions(ZPGine
        PUBLIC $<$<CONFIG:DEBUG>:ZPG_ENABLE_ASSERTS>
        PUBLIC $<$<CONFIG:DEBUG>:ZPG_ENABLE_LOGGING>
        PUBLIC $<$<CONFIG:DEBUG>:ZPG_OPENGL_CALL_ERROR_CHECK>
        # PUBLIC ZPG_ENABLE_PROFILING
        PUBLIC ZPG_FORWARD_EVENT_TO_MEMBER_FN_USE_LAMBDAS
        PUBLIC GLFW_INCLUDE_NONE
        GLM_ENABLE_EXPERIMENTAL
)


find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)
target_link_libraries(ZPGine PUBLIC glfw)

find_package(glm REQUIRED)
target_link_libraries(ZPGine PUBLIC glm::glm)

add_subdirectory(vendor/glad)
target_link_libraries(ZPGine PUBLIC glad)

add_subdirectory(vendor/spdlog)
target_link_libraries(ZPGine PUBLIC spdlog::spdlog)

add_subdirectory(vendor/imgui)
target_link_libraries(ZPGine PUBLIC ImGui)

add_subdirectory(vendor/stb_image)
target_link_libraries(ZPGine PUBLIC Stb_Image)

target_link_libraries(ZPGine PUBLIC assimp)

# boost c++ library

if(UNIX AND NOT APPLE)
    target_link_options(ZPGine PUBLIC -rdynamic)
endif()

# can be referenced both ways
add_library(ZPGine::ZPGine ALIAS ZPGine)




