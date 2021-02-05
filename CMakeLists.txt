cmake_minimum_required(VERSION 3.15)
include(ExternalProject)
project(3DGraphicsExamples CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

set(3rdParty ${PROJECT_SOURCE_DIR}/src/3rdParty)
include_directories(${PROJECT_SOURCE_DIR}/src/3rdParty/include ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE glad_SOURCES "${PROJECT_SOURCE_DIR}/src/3rdParty/src/glad/*.c")
list(APPEND 3rdParty_SOURCES ${glad_SOURCES})


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${3rdParty}/src/glfw3)


add_library(glad OBJECT ${glad_SOURCES})

find_package(OpenGL REQUIRED)

link_libraries(OpenGL::GL glfw)
add_subdirectory(src/Application)

link_libraries(application)
add_subdirectory(src/Exercises/Triangle)
add_subdirectory(src/Exercises/Colors)
add_subdirectory(src/Exercises/Indices)
add_subdirectory(src/Exercises/Uniforms)
add_subdirectory(src/Exercises/PVM)
add_subdirectory(src/Exercises/Pyramid)
add_subdirectory(src/Exercises/BackfaceCulling)
add_subdirectory(src/Exercises/Resize)
add_subdirectory(src/Exercises/Zoom)
add_subdirectory(src/Exercises/CameraMove)
add_subdirectory(src/Exercises/PyramidAnimation)
add_subdirectory(src/Exercises/PyramidTextured)
add_subdirectory(src/Exercises/PhongDiffuseLightning)
