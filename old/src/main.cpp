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
#include "moustik.h"
using namespace std;

int previousTime;
int currentTime;
int elapsedTime;

unsigned int fps = 25;

extern moustik cousin;
int com=0;	//correspond au numéro de la jambe qui se lève

int main (int argc, char *argv[]){

	float w = 1000;
	float h = 600;

	glutInit(&argc, argv); // Initialisation de GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);	// Choix du mode d'affichage (ici RVB)
	glutInitWindowPosition(0,0); // Position initiale de la fenetre GLUT
	glutInitWindowSize(w,h); // Taille initiale de la fenetre GLUT
	glutCreateWindow("Premier Exemple"); // Creation de la fenetre GLUT
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);	// Définition de la couleur d'effacement du framebuffer //le fond est noir
	// Définition des fonctions de callbacks
	glutDisplayFunc(affichage);
	glutKeyboardFunc(clavier);
	glutKeyboardUpFunc(clavierUp);
	glutReshapeFunc(reshape);

	glutTimerFunc(1000/fps, update, 0);
	currentTime = glutGet(GLUT_ELAPSED_TIME);

	glutMainLoop(); // Lancement de la boucle infinie GLUT
	return 0;
}
