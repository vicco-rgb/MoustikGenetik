
/*
		INCLUDES ###################################################################
*/

#ifdef __APPLE__ //OpenGL et GLUT
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#include <cmath> //pour M_PI, cos, sin et tan.
#include <Box2D/Box2D.h> //pour la physique
#include <stdio.h>
#include <iostream> //pour écrire et lire dans un terminal
#include <fstream> //pour écrire dans un fichier texte
#include <vector>
#include <ctime> //pour avoir accès à l'horloge et aux nombres alatoires
#include <algorithm> //pour avoir des fonctions utiles avec vector
#include "genome.hpp"
#include "draw.hpp"
#include "moustik.hpp"
using namespace std;

/*
		VARIABLES GLOBALES #########################################################
*/

bool channel=true; //par défaut, mode de jeu
bool grid=false;
//REAL WORLD
b2World* ptrWorld= new b2World(b2Vec2(0.0f,-9.81f));
Moustik cousin(ptrWorld, Coord(0.0,3.0));
Forme ground(ptrWorld, Coord(0.0,-1.0), 10.0, 1.0, 1);
int nFrame=0;
//IAWORLD
b2World* ptrWorldIAs= new b2World(b2Vec2(0.0f,-9.81f));
vector<int> seq {1,20,50,60,70};
Genome genome(seq);
MoustikIA cousinIA(ptrWorldIAs, Coord(0.0,3.0), genome, 0);
Forme groundIAs(ptrWorldIAs, Coord(0.0,-1.0), 10.0, 1.0, 1);
int nFrameIAs=0;


/*
		MAIN #######################################################################
*/

int main(int argc, char** argv){
	B2_NOT_USED(argc);
	B2_NOT_USED(argv);
	int fps=60;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);	// Choix du mode d'affichage (ici RVB)
	glutInitWindowPosition(0,0); // Position initiale de la fenetre GLUT
	glutInitWindowSize(1000,600); // Taille initiale de la fenetre GLUT
	glutCreateWindow("DaddyLongLeg - Algo Genetique"); // Creation de la fenetre GLUT
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);	// Définition de la couleur d'effacement du framebuffer //le fond est noir
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	// Définition des fonctions de callbacks
	glutDisplayFunc(affichage);
	glutKeyboardFunc(clavier);
	glutReshapeFunc(reshape);
	glutTimerFunc(floor(1000/fps), update, fps);
	glutMainLoop(); // Lancement de la boucle infinie GLUT

	return 0;
}
