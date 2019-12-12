#include "draw.hpp"
#include "moustik.hpp"
#include "genome.hpp"

/*
VARIABLES GLOBALES #############################################################
*/

float abscisse=0;
extern int fps;
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
		glBegin(GL_LINES);
		glColor3f(0.2f,0.4f,0.2f);
		glVertex2f(x1, i);
		glColor3f(0.2f,0.4f,0.2f);
		glVertex2f(x2, i);
		glEnd();
		glLineWidth(3.0f);
		glFlush();
	}
	for (int j=x1;j<x2;j++){ //verticales
		glBegin(GL_LINES);
		glColor3f(0.2f,0.4f,0.2f);
		glVertex2f(j, y1);
		glColor3f(0.2f,0.4f,0.2f);
		glVertex2f(j, y2);
		glEnd();
		glLineWidth(3.0f);
		glFlush();
	}
}
GLvoid affichage(){
	glClear(GL_COLOR_BUFFER_BIT); 	// Effacement du frame buffer

	if (grid){ //affichage de la grille
		drawQuadrillage(floor(abscisse)-2,floor(abscisse)+5,-2,3);
	}
	if (channel) { //vrai monde
		cousin->drawOpenGL();
		ground->drawOpenGL();
	} else {
		groundIAs->drawOpenGL(0.7f,0.7f,1.0f);
		cousinIA->drawOpenGL();
		float fitness = cousinIA->getGenome()->getFitness();
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.6f, 0.6f);
		glVertex2f(fitness, -1.0);
		glColor3f(1.0f, 0.6f, 0.6f);
		glVertex2f(fitness, 3.0);
		glEnd();
		glLineWidth(3.0f);
		glFlush();
		//faire passer les moustiks les uns après les autres.
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
		//on s'interesse au monde utilisateur
		cousin->upAge();
		cousin->getWorld()->Step((float32)1/fps, (int32)8, (int32)3);
		cousin->undertaker(); //est-ce que il est mort ?
		cousin->updateFitness();
		abscisse=cousin->getAbs();
	} else {
		//on s'intéresse au monde jeu
		MoustikIA* lastMoustik = HomoSapiens->getMoustiks().back();
		if (cousinIA == lastMoustik && lastMoustik->isDead()){
        //si le dernier moustik est mort
        HomoSapiens->writeGenomes();
        cout<<"####### end simulation of generation "+to_string(HomoSapiens->getGeneration())+" #######"<<endl;
				//on passe à la génération suivante ou on arrête la simulation?
        cout<<"0-quit \t 1-generation suivante : ";
        int choix=1;
        //cin >> choix;
        if (choix==1){
          //on prend la moitiée des meilleurs moustiks.
					cout<<"upGeneration"<<endl;
          HomoSapiens = HomoSapiens->getChildren(HomoSapiens->getMoustiks().size()/2);
        } else {
          exit(0);
        }

		}

		cousinIA=HomoSapiens->playLive();
		cousinIA->upAge();
		cousinIA->getWorld()->Step((float32)1/fps, (int32)8, (int32)3);
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
