//MAINFILE
#ifdef __APPLE__ //OpenGL
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <iostream>
#include <math.h>
#include "affichage.h"
#include "physique.h"
using namespace std;

int previousTime;
int currentTime;
int elapsedTime;

extern b2Body* body;

unsigned int fps = 60;

int com=0;	//correspond au numéro de la jambe qui se lève

int main (int argc, char *argv[]){

	float w = 1000;
	float h = 600;

	glutInit(&argc, argv); // Initialisation de GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);	// Choix du mode d'affichage (ici RVB)
	glutInitWindowPosition(0,0); // Position initiale de la fenetre GLUT
	glutInitWindowSize(w,h); // Taille initiale de la fenetre GLUT
	glutCreateWindow("DaddyLongLeg - Algo Genetique"); // Creation de la fenetre GLUT
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);	//le fond est noir

	// Définition des fonctions de callbacks
	glutDisplayFunc(affichage);
	glutKeyboardFunc(clavier);
	glutReshapeFunc(reshape);
	glutTimerFunc((unsigned int)1000/fps, update, (float)fps);
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	BigBang();
	glutMainLoop(); // Lancement de la boucle infinie GLUT
	return 0;
}
