cmake_minimum_required(VERSION 3.20)

project(M-Engine
    VERSION 0.1.0
    DESCRIPTION "A custom 3D game engine built with C++ and OpenGL"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_library(glad STATIC
    third_party/glad/src/gl.c
)

target_include_directories(glad PUBLIC
    third_party/glad/include
)

add_executable(M-Engine
    samples/main.cpp
)

target_link_libraries(M-Engine PRIVATE
    glad
    glfw
    glm::glm
    OpenGL::GL
)

target_compile_options(M-Engine PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic>
)