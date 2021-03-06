cmake_minimum_required(VERSION 3.5)

project(learnopengl VERSION 2.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lessonProgram
    main.cpp
    glad/src/glad.c glad/include/glad/glad.h
    glad/include/KHR/khrplatform.h
    stb/stb_image.h
    opengl_framework.cpp opengl_framework.h
    image_framework.cpp image_framework.h
    shader_program.cpp shader_program.h
    triangles_drawer.cpp triangles_drawer.h
    texture_applier.cpp texture_applier.h
    common_settings.h
    common_definitions.h
    camera_system.cpp camera_system.h
    examples/ch5_hello_triangle.cpp examples/ch5_hello_triangle.h
    examples/ch5_shaders.h
    examples/ch6_shaders_examples.cpp examples/ch6_shaders_examples.h
    examples/ch6_shaders.h
    examples/ch7_textures.cpp examples/ch7_textures.h
    examples/ch7_shaders.h
    examples/ch8_transformation.cpp examples/ch8_transformation.h
    examples/ch8_shaders.h
    examples/ch9_coord_system.cpp examples/ch9_coord_system.h
    examples/ch9_shaders.h
    examples/ch10_camera.cpp examples/ch10_camera.h
    examples/ch10_shaders.h
    examples/ch12_colors.cpp examples/ch12_colors.h
    examples/ch12_shaders.h
    examples/ch13_basic_lighting.cpp examples/ch13_basic_lighting.h
    examples/ch13_shaders.h
    examples/ch14_materials.cpp examples/ch14_materials.h
    examples/ch14_shaders.h
    examples/ch15_lighting_maps.cpp examples/ch15_lighting_maps.h
    examples/ch15_shaders.h
    )

target_include_directories(lessonProgram
    PRIVATE "."
    PRIVATE "glad/include"
    PRIVATE "D:/Soft/Programming/OpenGL/glfw-3.3.2/include")

add_library(GLFW_LIB STATIC IMPORTED)
set_property(TARGET GLFW_LIB PROPERTY IMPORTED_LOCATION "D:/Soft/Programming/OpenGL/glfw-build/src/libglfw3.a")

target_link_libraries(lessonProgram PRIVATE GLFW_LIB)
