//INCLUDES
#include "affichage.h"

//VARIABLES
GLfloat deplacement;

extern int previousTime;
extern int currentTime;
extern int elapsedTime;

extern unsigned int fps;

extern Moustik cousin;	//variable globale definie dans un autre cpp
extern int com;
extern Terrain sol;

extern b2Vec2 positionBODY;
extern float32 angleBODY;
extern b2Vec2 positionGROUND;
extern float32 angleGROUND;
extern b2World* ptrWorld;

bool chgmt=false;

//FONCTIONS
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
	if (width >= height) {
		// aspect >= 1, set the height from -1 to 1, with larger width
		gluOrtho2D((cousin.getPosM().x-2.0)* aspect, (cousin.getPosM().x+4.0) * aspect, -1.0, 3.0);
	} else {
		// aspect < 1, set the width to -1 to 1, with larger height
		gluOrtho2D((cousin.getPosM().x-2.0), (cousin.getPosM().x+4.0), -1.0 / aspect, 3.0 / aspect);
	}
}
void pointPlateformes(Coord *ptA,Coord *ptB,Coord *ptC,Coord *ptD){
	//ne fonctionne pas lorsque le moustik est dans des positions négatives
	vector<Coord> defCoordSol=sol.getCoord();
	int indiceSolPrec=sol.getPortionInt(cousin.getPosM().x);
	int nCycles=floor(cousin.getPosM().x/defCoordSol.back().x); //combien de fois le moustik a parcouru le parcours sol ?

	*ptB=defCoordSol[indiceSolPrec]; //POINT B
	ptB->x+=nCycles*defCoordSol.back().x;
	if (indiceSolPrec==0){ //POINT A
		*ptA=defCoordSol.back();
		ptA->x=ptB->x-1;
	} else {
		*ptA=defCoordSol[indiceSolPrec-1];
		ptA->x+=nCycles*defCoordSol.back().x;
	}
	if (indiceSolPrec+1>defCoordSol.size()){// POINT C & D
		*ptC=defCoordSol.front(); //C dépasse, donc D dépasse
		ptC->x+=ptB->x+1;
		*ptD=defCoordSol[1];
		ptD->x+=ptC->x;
	}else{ //le point C ne dépasse pas
		*ptC=defCoordSol[indiceSolPrec+1];
		ptC->x+=nCycles*defCoordSol.back().x;
		if(indiceSolPrec+2>defCoordSol.size()){ //le point D dépasse
			*ptD=defCoordSol.front();
			ptD->x=ptC->x+1;
		} else { //le point D ne dépasse pas
			*ptD=defCoordSol[indiceSolPrec+2];
			ptD->x+=nCycles*defCoordSol.back().x;
		}
	}
	// cout<<"ptA : "<<*ptA<<"ptB : "<<*ptB<<"ptC : "<<*ptC<<"ptD : "<<*ptD<<endl;
}
GLvoid affichage(){
	// Effacement du frame buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// AFFICHAGE SOL : interpolation linéaire des plateformes de coordonnées ptA,ptB,ptC,ptD->
	Coord ptA,ptB,ptC,ptD;
	pointPlateformes(&ptA,&ptB,&ptC,&ptD);
	glBegin(GL_QUADS); //portion précédente
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(ptA.x, ptA.y);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(ptA.x, ptA.y-0.3);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(ptB.x, ptB.y-0.3);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(ptB.x, ptB.y);
	glEnd();
	glFlush();
	glBegin(GL_QUADS); //portion actuelle
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(ptB.x, ptB.y);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(ptB.x, ptB.y-0.3);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(ptC.x, ptC.y-0.3);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(ptC.x, ptC.y);
	glEnd();
	glFlush();
	glBegin(GL_QUADS); //portion future
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(ptC.x, ptC.y);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(ptC.x, ptC.y-0.3);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(ptD.x, ptD.y-0.3);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(ptD.x, ptD.y);
	glEnd();
	glFlush();


	glBegin(GL_QUADS);	//TETE
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(cousin.getPosH1().x,cousin.getPosH1().y);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(cousin.getPosH2().x,cousin.getPosH2().y);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(cousin.getPosT2().x,cousin.getPosT2().y);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(cousin.getPosT1().x,cousin.getPosT1().y);
	glEnd();
	glFlush();

	glBegin(GL_LINES); //CUISSE 1
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(cousin.getPosH1().x,cousin.getPosH1().y);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(cousin.getPosG1().x,cousin.getPosG1().y);
	glEnd();
	glLineWidth(2.0)
	glFlush();

	glBegin(GL_LINES); //CUISSE 2
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(cousin.getPosH2().x,cousin.getPosH2().y);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(cousin.getPosG2().x,cousin.getPosG2().y);
	glEnd();
	glLineWidth(2.0)
	glFlush();

	glBegin(GL_LINES); //TIBIA 1
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(cousin.getPosG1().x,cousin.getPosG1().y);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(cousin.getPosP1().x,cousin.getPosP1().y);
	glEnd();
	glLineWidth(2.0)
	glFlush();

	glBegin(GL_LINES); //TIBIA 2
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(cousin.getPosG2().x,cousin.getPosG2().y);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(cousin.getPosP2().x,cousin.getPosP2().y);
	glEnd();
	glLineWidth(2.0)
	glFlush();

	glLoadIdentity();
	gluOrtho2D(-2.0, 4.0, -1.0, 3.0);
	glutSwapBuffers();
}
GLvoid update(int fps){
	// setups the timer to be called again
	glutTimerFunc(fps, update, 0);
	PhysicEngine();

	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	ptrWorld->Step(timeStep, velocityIterations, positionIterations);
	// Get the time when the previous frame was rendered
	previousTime = currentTime;

	// Get the current time (in milliseconds) and calculate the elapsed time
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	elapsedTime = currentTime - previousTime;
	deplacement = 0.01*elapsedTime;
	glutPostRedisplay();
}
GLvoid clavier(unsigned char touche, int x, int y) {
	// Suivant les touches pressees, nous aurons un comportement different de l'application
	// ESCAPE ou 'q' : fermera l'application
	switch(touche) {
		case 's':
		com=(com%2)+1;
		chgmt=true;
		break; //on ne peut commander qu'une seule jambe a la fois.
		case 'i':
		Coord nM;
		nM.x=cousin.getT()/2;
		nM.y=2*cousin.getL()+cousin.getH()/2;
		cousin.setPosM(nM);
		cousin.setA1(0.0);
		cousin.setA2(0.0);
		cousin.setC(0.0);
		cousin.setD1(0.0);
		cousin.setD2(0.0);
		com=0;
		break;
		case 'q' : // quitter
		case 27 :
		exit(0);
		break;
	}
	// Demande a GLUT de reafficher la scene
	glutPostRedisplay();
}

GLvoid clavierUp(unsigned char key, int x, int y){}
