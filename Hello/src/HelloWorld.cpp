#ifdef __APPLE__ //OpenGL et GLUT
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#include <cmath>
#include <Box2D/Box2D.h>
#include <stdio.h>
#include <iostream>
using namespace std;

/*
		STRUCTURES #################################################################
*/

struct Coord {
	float x;
	float y;
	Coord(){}
	Coord(float x, float y){
		this->x=x;
		this->y=y;
	}
};
void operator+=(Coord & point1, Coord const& point2){
  // surcharge pour faire de la translation de Coord
  point1.x+=point2.x;
  point1.y+=point2.y;
}
void operator-=(Coord & point1, Coord const& point2){
  // surcharge pour faire de la translation de Coord
  point1.x-=point2.x;
  point1.y-=point2.y;
}
Coord operator-(Coord const& point1, Coord const& point2){
  // surcharge pour faire de la translation de Coord
  Coord res;
  res.x=point1.x-point2.x;
  res.y=point1.y-point2.y;
  return res;
}
Coord operator+(Coord const& point1, Coord const& point2){
  // surcharge pour faire de la translation de Coord
  Coord res;
  res.x=point1.x+point2.x;
  res.y=point1.y+point2.y;
  return res;
}
Coord operator*(float const& scal, Coord const& point){
  // surcharge pour faire de la translation de Coord
  Coord res=point;
  res.x*=scal;
  res.y*=scal;
  return res;
}
ostream& operator<<(ostream& os, const Coord pos){
    os << pos.x << '\t' << pos.y << endl;
    return os;
}
b2Vec2 coord2bvec(Coord pos){
	b2Vec2 cpos(pos.x, pos.y);
	return cpos;
}

//body, width, height, type, diag, angleIn
//getX, getY, getPos, getBody, getAngle, getAngleIn, getDiag, getHL, getHR drawOpenGL
class Forme {
private:
  b2Body* body;
  float width;
  float height;
  int type; //0, 1 ou 2 = dynamique, statique, kinématique
  float diag; //demi-diagonale de la boite
  float angleIn; //angle intérieur = atan(height/width)
public:
	// Forme(){
	// 	b2BodyDef bodyDef;
	// 	ptrWorld->CreateBody(&bodyDef);
	// }
	Forme(){}
	Forme(b2World* ptrWorld, Coord pos, float w, float h, int type){
	  b2BodyDef bodyDef;

	  this->type=type;
		if (type==0){ //dynamique
			bodyDef.type=b2_dynamicBody;
		} else if (type==1){
			bodyDef.type=b2_staticBody;
		} else if (type==2){
			bodyDef.type=b2_kinematicBody;
		}
		bodyDef.position.Set(pos.x, pos.y);

		body = ptrWorld->CreateBody(&bodyDef);
		//Box
		b2PolygonShape collisionBox;
		collisionBox.SetAsBox(w, h);
	  //Fixtures
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &collisionBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.3f;
		body->CreateFixture(&fixtureDef);

	  width=w;
	  height=h;
	  diag=sqrt(w*w+h*h);
	  angleIn=atan(height/width);
	}
	Forme(b2World* ptrWorld, Coord pos, int type){
	  Forme(ptrWorld, pos, 1.0, 1.0, type);
	}
	~Forme(){}
	float getX(){
		return body->GetPosition().x;
	}
	float getY(){
		return body->GetPosition().y;
	}
	Coord getPos(){
		Coord pos(body->GetPosition().x, body->GetPosition().y);
		return pos;
	}
	b2Body* getBody(){
		return body;
	}
	float getAngle(){
		return body->GetAngle();
	}
	float getAngleIn(){
		return angleIn;
	}
	float getDiag(){
		return diag;
	}
	Coord getHL(){
		Coord pos(getX()-diag*cos(getAngle()+angleIn), getY()-diag*sin(getAngle()+angleIn));
		return pos;
	}
	Coord getHR(){
		Coord pos(getX()+diag*cos(angleIn-getAngle()), getY()-diag*sin(angleIn-getAngle()));
		return pos;
	}
	GLvoid drawOpenGL(){
		glBegin(GL_QUADS);
		glColor3f(1.0f, 0.9f, 0.8f);
		glVertex2f(getX()-diag*cos(angleIn-getAngle()), getY()+diag*sin(angleIn-getAngle()));
		glColor3f(1.0f, 0.8f, 0.9f);
		glVertex2f(getX()-diag*cos(getAngle()+angleIn), getY()-diag*sin(getAngle()+angleIn));
		glColor3f(0.9f, 0.9f, 0.8f);
		glVertex2f(getX()+diag*cos(angleIn-getAngle()), getY()-diag*sin(angleIn-getAngle()));
		glColor3f(0.8f, 1.0f, 1.0f);
		glVertex2f(getX()+diag*cos(angleIn+getAngle()), getY()+diag*sin(angleIn+getAngle()));
		glEnd();
		glFlush();
	}
};
//head, legL, legR, rotuleL, rotuleR, com
//commande, getPos, drawOpenGL
class Moustik {
private:
	Forme head;
	Forme legL;
	Forme legR;
	b2RevoluteJointDef rotuleL;
	b2RevoluteJointDef rotuleR;
	int com;
public:
	Moustik(b2World* ptrWorld, Coord pos){
		com = 0;
		//définition des formes
		Forme head(ptrWorld, pos, 0.25, 0.25, 0); //tête dynamique de 0.5x0.5
		Forme legL(ptrWorld, pos, 0.05, 0.5, 0); //jambe dynamique de 0.1x1
		Forme legR(ptrWorld, pos, 0.05, 0.5, 0); //jambe dynamique de 0.1x1
		//définition de la rotuleL et rotuleR
		rotuleL.Initialize(head.getBody(), legL.getBody(), coord2bvec(head.getHL()));
		rotuleR.Initialize(head.getBody(), legR.getBody(), coord2bvec(head.getHR()));
		//angles limites rotules
		float amax=2*M_PI/3;
		rotuleL.enableLimit = true;
		rotuleL.lowerAngle = -amax;
		rotuleL.upperAngle = amax;
		rotuleR.enableLimit = true;
		rotuleR.lowerAngle = -amax;
		rotuleR.upperAngle = amax;
		//créer le joint
		ptrWorld->CreateJoint(&rotuleL);
		ptrWorld->CreateJoint(&rotuleR);
	}
	~Moustik(){}
	void commande(){
		com=(com+1)%2;
		// Turn on the motor.
		//rjd.enableMotor = true;
 		// How fast is the motor?
		//rjd.motorSpeed = PI*2;
		// How powerful is the motor?
	}
	Coord getPos(){
		return head.getPos();
	}
	GLvoid drawOpenGL(){
		head.drawOpenGL();
		legL.drawOpenGL();
		legR.drawOpenGL();
	}
};

/*
		VARIABLES GLOBALES #########################################################
*/

b2Vec2 gravity(0.0f, -10.0f);
b2World world(gravity);
b2World* ptrWorld=&world;

//MOUSTIK-----------------------------------------
Coord pos(0.0,2.0);
Forme head(ptrWorld, pos, 0.25, 0.25, 0); //tête dynamique de 0.5x0.5
Forme legL(ptrWorld, pos, 0.05, 0.5, 0); //jambe dynamique de 0.1x1
Forme legR(ptrWorld, pos, 0.05, 0.5, 0); //jambe dynamique de 0.1x1

//FIN MOUSTIK ----------------------

Coord posGround(0.0,-1.0);
Forme ground(ptrWorld, posGround, 10.0, 1.0, 1);


/*
		FONCTIONS D'AFFICHAGE ######################################################
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
		gluOrtho2D(-2.0* aspect, 4.0 * aspect, -1.0, 5.0); // aspect >= 1, set the height from -1 to 1, with larger width
	} else {
		gluOrtho2D(-2.0, 4.0, -1.0 / aspect, 5.0 / aspect);// aspect < 1, set the width to -1 to 1, with larger height
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

	// cousin.drawOpenGL();
	head.drawOpenGL();
	legL.drawOpenGL();
	legR.drawOpenGL();
	ground.drawOpenGL();
	drawQuadrillage(-2,5,-2,5);

	glLoadIdentity();
	gluOrtho2D(-2.0, 4.0, -1.0, 3.0);
	glutSwapBuffers();
}
GLvoid update(int fps){
	glutTimerFunc(fps, update, 0);
	// Prepare for simulation. Typically we use a time step of 1/60 of a
	// second (60Hz) and 10 iterations. This provides a high quality simulation
	// in most game scenarios.
	ptrWorld->Step(1.0f/30000, 6, 2);
	glutPostRedisplay();
}
GLvoid clavier(unsigned char touche, int x, int y) {
	switch(touche) {
		case 's':
		break; //on ne peut commander qu'une seule jambe a la fois.
		case 'i':
		break;
		case 'q' : // quitter
		case 27 : //escape ou "q"
			exit(0);
		break;
	}
}

/*
		MAIN #######################################################################
*/

int main(int argc, char** argv){
	B2_NOT_USED(argc);
	B2_NOT_USED(argv);

	//définition de la rotuleL et rotuleR
	b2RevoluteJointDef* rotuleL = new b2RevoluteJointDef();
	b2RevoluteJointDef* rotuleR = new b2RevoluteJointDef();
	rotuleL->Initialize(head.getBody(), legL.getBody(), coord2bvec(head.getHL()));
	rotuleR->Initialize(head.getBody(), legR.getBody(), coord2bvec(head.getHR()));
	//angles limites rotules
	float amax=2*M_PI/3;
	rotuleL->enableLimit = true;
	rotuleL->lowerAngle = -amax;
	rotuleL->upperAngle = amax;
	rotuleR->enableLimit = true;
	rotuleR->lowerAngle = -amax;
	rotuleR->upperAngle = amax;
	//créer le joint
	ptrWorld->CreateJoint(rotuleL);
	ptrWorld->CreateJoint(rotuleR);

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
	glutTimerFunc((unsigned int)100/6, update, 0);
	glutMainLoop(); // Lancement de la boucle infinie GLUT

	return 0;
}
