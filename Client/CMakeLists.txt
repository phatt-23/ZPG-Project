cmake_minimum_required(VERSION 3.30)

project(Client)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# By default CONFIG (which is set by -DCMAKE_BUILD_TYPE=DEBUG
# or -DCMAKE_BUILD_TYPE=RELEASE) is set to DEBUG
add_compile_options(
        -Wall
        -Wextra
        -Wconversion
        -pedantic
        # $<$<CONFIG:DEBUG>:-g3>
        # $<$<CONFIG:DEBUG>:-Og>
        # $<$<CONFIG:RELEASE>:-O3>
)

if (NOT DISABLE_ASAN AND $<CONFIG:DEBUG>)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
endif()

file(GLOB_RECURSE client_sources "src/**.cpp")
file(GLOB_RECURSE client_headers "src/**.h")

add_executable(Client
        ${client_sources}
        ${client_headers}
)

target_include_directories(Client PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(Client PRIVATE ZPGine::ZPGine)

target_include_directories(Client PRIVATE
        ${CMAKE_SOURCE_DIR}/ZPGine/
        ${CMAKE_SOURCE_DIR}/ZPGine/src
)

set(WORKING_DIRECTORY "../")
