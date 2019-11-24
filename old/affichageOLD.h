#pragma once
#include <math.h>
#include <vector>
#include <iostream>
#include "moustik.h"
#ifdef __APPLE__ //OpenGL et GLUT
	#include <OpenGL/gl.h>
	#include <GLUT/GLUT.h>
#else
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif
#include "physique.h"
#include "terrain.h"

using namespace std;

struct Coord;
void pointPlateformes(Coord *,Coord *,Coord *,Coord *);
GLvoid affichage();
GLvoid clavier(unsigned char, int, int);
void clavierUp(unsigned char, int x, int y);
GLvoid update(int);
void reshape(GLsizei, GLsizei);
GLvoid souris(int, int, int, int);
