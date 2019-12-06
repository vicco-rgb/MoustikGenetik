#include "draw.hpp"

/*
		VARIABLES GLOBALES #########################################################
*/

extern bool channel; //par défaut, mode de jeu
extern bool grid;
//REAL WORLD
extern b2World* ptrWorld;
extern Moustik cousin;
extern Forme ground;
extern int nFrame;
//IAWORLD
extern b2World* ptrWorldIAs;
extern MoustikIA cousinIA;
extern Forme groundIAs;
extern int nFrameIAs;

/*
		FONCTIONS OPENGL ###########################################################
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
		gluOrtho2D(cousin.getAbs()-2.0* aspect, cousin.getAbs()+4.0 * aspect, -1.0, 5.0); // aspect >= 1, set the height from -1 to 1, with larger width
	} else {
		gluOrtho2D(cousin.getAbs()-2.0, cousin.getAbs()+4.0, -1.0 / aspect, 5.0 / aspect);// aspect < 1, set the width to -1 to 1, with larger height
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
		cousin.drawOpenGL();
		ground.drawOpenGL();
	} else {
		cousinIA.drawOpenGL();
		groundIAs.drawOpenGL();
	}

	if (grid){ //affichage de la grille
		drawQuadrillage(floor(cousin.getAbs())-2,floor(cousin.getAbs())+5,-2,3);
	}

	glLoadIdentity();
	gluOrtho2D(cousin.getAbs()-2.0, cousin.getAbs()+4.0, -1.0, 3.0);
	glutSwapBuffers();
}
GLvoid update(int fps){
	int dt=floor(1000/fps); //dt=16ms pour 60fps
	glutTimerFunc(dt, update, fps);
	//le jeu se met en pause lorsque l'on change de channel
	if (channel){ //on s'interesse au monde IA ou au monde jeu?
		nFrame++;
		ptrWorld->Step((float32)1/fps, (int32)8, (int32)3);
		cousin.undertaker(nFrame); //est-ce que il est mort ?
		cousin.updateScore();
	} else {
		nFrameIAs++;
		ptrWorldIAs->Step((float32)1/fps, (int32)8, (int32)3);
		cousinIA.play(ptrWorldIAs, nFrameIAs);
		cousinIA.updateScore();
		cousinIA.undertaker(nFrameIAs);
	}
	glutPostRedisplay();
}
GLvoid clavier(unsigned char touche, int x, int y) {
	switch(touche) {
		case 's':
		cousin.commande(ptrWorld, nFrame);
		break; //on ne peut commander qu'une seule jambe a la fois.
		case 'g':
		grid=!grid;
		break;
		case 'i':
		cousin.reset(ptrWorld);
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
