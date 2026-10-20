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

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/engine/*.cpp"
)

add_library(m_engine STATIC
    ${ENGINE_SOURCES}
)

target_include_directories(m_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(m_engine PUBLIC
    glad
    glfw
    glm::glm
    OpenGL::GL
)

target_compile_options(m_engine PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic>
)

add_executable(M-Engine
    samples/main.cpp
)

target_link_libraries(M-Engine PRIVATE
    m_engine
)