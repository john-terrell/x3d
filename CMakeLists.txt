cmake_minimum_required(VERSION 3.2)

project(x3d CXX)

find_package(Glut REQUIRED)
find_package(OpenGL REQUIRED)

set (CMAKE_CXX_STANDARD 11)

set(SRC_FILES
    source/color.h
    source/element.cpp
    source/element.h
    source/hemicube.cpp
    source/hemicube.h
    source/main.cpp
    source/opengl.h
    source/point3.h
    source/radiosity.cpp
    source/radiosity.h
    source/vector3.h
    source/window.cpp
    source/window.h
    source/x3d.h
    )

include_directories(include)

add_executable(x3d ${SRC_FILES})

target_link_libraries(x3d ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
