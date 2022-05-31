cmake_minimum_required(VERSION 3.19)
project(du6 C)

set(CMAKE_C_STANDARD 99)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
add_executable(du6 main.c)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})