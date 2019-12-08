#pragma once
#ifdef __APPLE__ //OpenGL et GLUT
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#include <Box2D/Box2D.h>
#include <stdio.h> //permet d'interagir avec le clavier et la souris

/*
		FONCTIONS OPENGL ###########################################################
*/

void reshape(GLsizei, GLsizei);
GLvoid drawQuadrillage(int, int, int, int);
GLvoid affichage();
GLvoid update(int);
GLvoid clavier(unsigned char, int, int);
