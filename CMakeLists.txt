cmake_minimum_required(VERSION 3.5)

project(learnopengl VERSION 2.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lessonProgram
    main.cpp
    glad/src/glad.c glad/include/glad/glad.h
    glad/include/KHR/khrplatform.h
    opengl_framework.cpp opengl_framework.h
    shader_program.cpp shader_program.h
    triangles_drawer.cpp triangles_drawer.h
    shaders/simple_triangle.h
    )

target_include_directories(lessonProgram
    PRIVATE "glad/include"
    PRIVATE "D:/Soft/Programming/OpenGL/glfw-3.3.2/include")

add_library(GLFW_LIB STATIC IMPORTED)
set_property(TARGET GLFW_LIB PROPERTY IMPORTED_LOCATION "D:/Soft/Programming/OpenGL/glfw-build/src/libglfw3.a")

target_link_libraries(lessonProgram PRIVATE GLFW_LIB)
