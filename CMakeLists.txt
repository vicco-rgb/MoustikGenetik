cmake_minimum_required(VERSION 3.0)
project(moustik)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Box2D REQUIRED)

set(Box2D_SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}../Box2D_v2.3.0/Box2D")
include_directories(${Box2D_SOURCE_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-g") #permet de chopper des informations pour débugger.

#FICHIERS .H
include_directories(headers)
#FICHIERS .CPP
file(GLOB SOURCES "src/*.cpp")


add_executable(main.out ${SOURCES})

target_link_libraries(
  main.out
  ${OPENGL_LIBRARIES}
  ${GLUT_LIBRARIES}
  Box2D
)

# main.out: main.o moustik.o physique.o affichage.o terrain.o
# 	g++ -o main.out main.o physique.o affichage.o moustik.o -lGL -lGLU -lglut
# 	rm *.o
# physique.o: physique.cpp terrain.h physique.h moustik.h
# 	g++ -c physique.cpp
# moustik.o: moustik.cpp moustik.h
# 	g++ -c moustik.cpp
# main.o: main.cpp affichage.h moustik.h physique.h
# 	g++ -c main.cpp
# affichage.o: affichage.cpp affichage.h moustik.h physique.h terrain.h
# 	g++ -c affichage.cpp
# terrain.o: terrain.cpp terrain.h moustik.h
# 	g++ -c terrain.cpp
# clean:
# 	rm *.o
