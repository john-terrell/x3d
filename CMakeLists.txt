cmake_minimum_required(VERSION 3.2)

project(x3d CXX)

find_package(Glut REQUIRED)
find_package(OpenGL REQUIRED)

set(SRC_FILES
    include/x3d/color.h
    include/x3d/element.h
    include/x3d/hemicube.h
    include/x3d/ieee_fp.h
    include/x3d/opengl.h
    include/x3d/point3.h
    include/x3d/vector3.h
    include/x3d/x3d.h
    samples/cornell_box/main.cpp
    samples/cornell_box/radiosity.cpp
    samples/cornell_box/radiosity.h
    samples/cornell_box/window.cpp
    samples/cornell_box/window.h
    source/element.cpp
    source/hemicube.cpp
    )

include_directories(include)

add_executable(x3d ${SRC_FILES})

target_link_libraries(x3d ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})
