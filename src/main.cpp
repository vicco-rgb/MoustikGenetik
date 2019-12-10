
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
int fps=60;
//REAL WORLD
b2World* ptrWorld;
Moustik* cousin;
Forme* ground;
int nFrame;
//IAWORLD
b2World* ptrWorldIAs;
Forme* groundIAs;
Population* HomoSapiens; //les premiers génomes
int nFrameIAs;

/*
		INIT #######################################################################
*/

void Init(){

	//HUMAN WORLD
	ptrWorld = new b2World(b2Vec2(0.0f,-9.81f));
	cousin = new Moustik(ptrWorld, Coord(0.0,3.0));
	ground = new Forme(ptrWorld, Coord(0.0,-1.0), 10.0, 1.0, 1);
	nFrame=0;

	//IA WORLD
	ptrWorldIAs = new b2World(b2Vec2(0.0f,-9.81f));
	groundIAs = new Forme(ptrWorldIAs, Coord(0.0,-1.0), 100.0, 1.0, 1); //! peut-être trop court !
	HomoSapiens = new Population();
	for (int i=0; i<50; i++){
		Genome* genomeIA = new Genome(40);
		MoustikIA* cousinIA = new MoustikIA(ptrWorldIAs, Coord(0.0,3.0), genomeIA, to_string(i));
		cousinIA->activation(false); //on rend le moustik inactif
		HomoSapiens->addMoustik(cousinIA);
	}
	Genome* affgen=HomoSapiens->getMoustiks()[0]->getGenome();

}

/*
		MAIN #######################################################################
*/

int main(int argc, char** argv){
	B2_NOT_USED(argc);
	B2_NOT_USED(argv);
	srand(time(0));

	Init(); 	//définition des variables.

	HomoSapiens->playOff();
	HomoSapiens->writeGenomes();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);	// Choix du mode d'affichage (ici RVB)
	glutInitWindowPosition(0,0); // Position initiale de la fenetre GLUT
	glutInitWindowSize(1000,600); // Taille initiale de la fenetre GLUT
	glutCreateWindow("DaddyLongLeg - Algo Genetique"); // Creation de la fenetre GLUT
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);	// Fond noir.
	int currentTime = glutGet(GLUT_ELAPSED_TIME);

	glutDisplayFunc(affichage);// Définition des fonctions de callbacks
	glutKeyboardFunc(clavier);
	glutReshapeFunc(reshape);
	glutTimerFunc(floor(1000/fps), update, fps);
	glutMainLoop(); // Lancement de la boucle infinie GLUT

	return 0;
}
