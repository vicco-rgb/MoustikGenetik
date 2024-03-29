#pragma once
#ifdef __APPLE__ //OpenGL
	#include <OpenGL/gl.h>
	#include <GLUT/GLUT.h>
#else
	#include <GL/gl.h>
	#include <GL/glut.h>
#endif

GLvoid affichage();
GLvoid clavier(unsigned char, int, int);
void clavierUp(unsigned char, int x, int y);
GLvoid update(int);
void reshape(GLsizei, GLsizei);
GLvoid souris(int, int, int, int);
