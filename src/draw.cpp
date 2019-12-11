#include "draw.hpp"
#include "moustik.hpp"
#include "genome.hpp"

/*
VARIABLES GLOBALES #############################################################
*/

float abscisse=0;
extern bool channel; //par défaut, mode de jeu
extern bool grid;
//REAL WORLD
extern Moustik* cousin; //humain
extern Forme* ground;
//IAWORLD
extern Forme* groundIAs;
MoustikIA* cousinIA;
extern Population* HomoSapiens; //les premiers génomes

/*
FONCTIONS OPENGL ###############################################################
*/

void reshape(GLsizei width, GLsizei height) {
	// GLsizei for non-negative integer
	// Compute aspect ratio of the new window
	if (height == 0){height = 1;}                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);
	// Set the aspect ratio of the clipping area to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset the projection matrix
	if (width >= height) { //ordre : left, right, bottom, top
		gluOrtho2D(abscisse-2.0* aspect, abscisse+4.0 * aspect, -1.0, 5.0); // aspect >= 1, set the height from -1 to 1, with larger width
	} else {
		gluOrtho2D(abscisse-2.0, abscisse+4.0, -1.0 / aspect, 5.0 / aspect);// aspect < 1, set the width to -1 to 1, with larger height
	}
}
GLvoid drawQuadrillage(int x1,int x2, int y1, int y2){
	for (int i=y1;i<y2;i++){  //horizontales
		glBegin(GL_QUADS);
		glColor3f(0.2f,0.2f,0.2f);
		glVertex2f(x1, i);
		glColor3f(0.2f,0.2f,0.2f);
		glVertex2f(x1, i-0.05);
		glColor3f(0.2f,0.2f,0.2f);
		glVertex2f(x2, i-0.05);
		glColor3f(0.2f,0.2f,0.2f);
		glVertex2f(x2, i);
		glEnd();
		glFlush();
	}
	for (int j=x1;j<x2;j++){ //verticales
		glBegin(GL_QUADS);
		glColor3f(0.2f,0.2f,0.2f);
		glVertex2f(j-0.05, y2);
		glColor3f(0.2f,0.2f,0.2f);
		glVertex2f(j-0.05, y1);
		glColor3f(0.2f,0.2f,0.2f);
		glVertex2f(j, y1);
		glColor3f(0.2f,0.2f,0.2f);
		glVertex2f(j, y2);
		glEnd();
		glFlush();
	}
}
GLvoid affichage(){
	glClear(GL_COLOR_BUFFER_BIT); 	// Effacement du frame buffer

	if (channel) { //vrai monde
		cousin->drawOpenGL();
		ground->drawOpenGL();
	} else {
		groundIAs->drawOpenGL(0.7f,0.7f,1.0f);
		cousinIA->drawOpenGL();
		//faire passer les moustiks les uns après les autres.
	}
	if (grid){ //affichage de la grille
		drawQuadrillage(floor(abscisse)-2,floor(abscisse)+5,-2,3);
	}
	glLoadIdentity();
	gluOrtho2D(abscisse-2.0, abscisse+4.0, -1.0, 3.0);
	glutSwapBuffers();
}
GLvoid update(int fps){
	int dt=floor(1000/fps); //dt=16ms pour 60fps
	glutTimerFunc(dt, update, fps);
	//le jeu se met en pause lorsque l'on change de channel
	if (channel){
<<<<<<< HEAD
		//on s'interesse au monde utilisateur
=======
		//on s'interesse au monde IA ou au monde jeu?
>>>>>>> e9bcecaa55a8efbe14dc0d5d8aee48c8bbf6c431
		cousin->upAge();
		cousin->getWorld()->Step((float32)1/fps, (int32)8, (int32)3);
		cousin->undertaker(); //est-ce que il est mort ?
		cousin->updateFitness();
		abscisse=cousin->getAbs();
	} else {
<<<<<<< HEAD
		//on s'intéresse au monde jeu
=======
>>>>>>> e9bcecaa55a8efbe14dc0d5d8aee48c8bbf6c431
		cousinIA->upAge();
		cousinIA->getWorld()->Step((float32)1/fps, (int32)8, (int32)3);
		cousinIA=HomoSapiens->playLive();
		cousinIA->play();
		cousinIA->undertaker();
		cousinIA->updateFitness();
		abscisse=cousinIA->getAbs();
	}
	glutPostRedisplay();
}
GLvoid clavier(unsigned char touche, int x, int y) {
	switch(touche) {
		case 's':
		cousin->commande();
		break; //on ne peut commander qu'une seule jambe a la fois.
		case 'g':
		grid=!grid;
		break;
		case 'i':
		cousin->reset();
		break;
		case 'c':
		channel=!channel;
		break;
		case 'q' : // quitter
		case 27 : //escape ou "q"
		exit(0);
		break;
	}
}
