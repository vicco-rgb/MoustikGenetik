cmake_minimum_required(VERSION 2.8)
project(oldmoustik)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-g") #permet de chopper des informations pour débugger.

include_directories(headers)
file(GLOB SRCS src/*.cpp)
add_executable(exec ${SRCS})

target_link_libraries (exec ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
