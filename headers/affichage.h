#pragma once
#ifdef __APPLE__ //OpenGL et GLUT
	#include <OpenGL/gl.h>
	#include <GLUT/GLUT.h>
#else
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif
#include <math.h>
#include <vector>
#include <iostream>

#include "physique.h"

using namespace std;

GLvoid affichage();
GLvoid drawQuadrillage(int, int, int, int);
GLvoid clavier(unsigned char, int, int);
GLvoid update(int);
void reshape(GLsizei, GLsizei);
