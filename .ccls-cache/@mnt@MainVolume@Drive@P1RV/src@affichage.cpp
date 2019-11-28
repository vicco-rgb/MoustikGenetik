//INCLUDES
#include "affichage.h"

//VARIABLES
extern int previousTime;
extern int currentTime;
extern int elapsedTime;
extern unsigned int fps;
extern int com;

extern float groundHeight;
extern float groundWidth;
extern float bodyWidth;
extern float bodyHeight;
extern b2Body* body;
extern b2Body* groundBody;
extern b2World* ptrWorld;


int nTotFrames=0;
GLvoid drawQuadrillage(int x1,int x2, int y1, int y2){
	for (int i=y1;i<y2;i++){  //horiontales
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
		gluOrtho2D(-2.0* aspect, 4.0 * aspect, -1.0, 3.0);
	} else {
		// aspect < 1, set the width to -1 to 1, with larger height
		gluOrtho2D(-2.0, 4.0, -1.0 / aspect, 3.0 / aspect);
	}
}
GLvoid affichage(){
	glClear(GL_COLOR_BUFFER_BIT); 	// Effacement du frame buffer
	drawQuadrillage(-2,5,-2,5);


	glBegin(GL_QUADS); //sol
	glColor3f(1.0f, 0.5f, 0.5f);
	glVertex2f(groundBody->GetLocalCenter().x-groundWidth, groundBody->GetLocalCenter().y+groundHeight);
	glColor3f(1.0f, 0.5f, 0.5f);
	glVertex2f(groundBody->GetLocalCenter().x-groundWidth, groundBody->GetLocalCenter().y-groundHeight);
	glColor3f(1.0f, 0.5f, 0.5f);
	glVertex2f(groundBody->GetLocalCenter().x+groundWidth, groundBody->GetLocalCenter().y-groundHeight);
	glColor3f(1.0f, 0.5f, 0.5f);
	glVertex2f(groundBody->GetLocalCenter().x+groundWidth, groundBody->GetLocalCenter().y+groundHeight);
	glEnd();
	glFlush();

	glBegin(GL_QUADS); //carre
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(body->GetLocalCenter().x-bodyWidth, body->GetLocalCenter().y+bodyHeight);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(body->GetLocalCenter().x-bodyWidth, body->GetLocalCenter().y-bodyHeight);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(body->GetLocalCenter().x+bodyWidth, body->GetLocalCenter().y-bodyHeight);
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex2f(body->GetLocalCenter().x+bodyWidth, body->GetLocalCenter().y+bodyHeight);
	glEnd();
	glFlush();

	glLoadIdentity();
	gluOrtho2D(-2.0, 4.0, -1.0, 3.0);
	glutSwapBuffers();
}
GLvoid update(int fps){
	glutTimerFunc((unsigned int)1000/fps, update, (float)fps); // setups the timer to be called again
	PhysicEngine();
	previousTime = currentTime;	// Get the time when the previous frame was rendered
	currentTime = glutGet(GLUT_ELAPSED_TIME);	// Get the current time (in milliseconds)
	elapsedTime = currentTime - previousTime; //calculate elapsed time
	glutPostRedisplay();
	nTotFrames++;
}
GLvoid clavier(unsigned char touche, int x, int y) {
	// Suivant les touches pressees, nous aurons un comportement different de l'application
	// ESCAPE ou 'q' : fermera l'application
	switch(touche) {
		case 's':
		com=(com%2)+1;
		break; //on ne peut commander qu'une seule jambe a la fois.
		case 'i':
		break;
		case 'q' : // quitter
		case 27 :
		cout<<nTotFrames;
		exit(0);
		break;
	}
	// Demande a GLUT de reafficher la scene
	// glutPostRedisplay();
}
