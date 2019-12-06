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
//getX, getY, getPos, getBody, getAngle, getAngleIn, getDiag, getHL, getHR, getTL, getTR, drawOpenGLx2
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
	~Forme(){
		body->GetWorld()->DestroyBody( body );
	}
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
		return Coord(getX()-diag*cos(getAngle()+angleIn), getY()-diag*sin(getAngle()+angleIn));
	}
	Coord getHR(){
		return Coord(getX()+diag*cos(angleIn-getAngle()), getY()-diag*sin(angleIn-getAngle()));
	}
	Coord getTL(){
		return Coord(getX()-diag*cos(angleIn-getAngle()), getY()+diag*sin(angleIn-getAngle()));
	}
	Coord getTR(){
		return Coord(getX()+diag*cos(angleIn+getAngle()), getY()+diag*sin(angleIn+getAngle()));
	}
	GLvoid drawOpenGL(){
		//dessine le rectangle dans une couleur blanc pale par défaut
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
	GLvoid drawOpenGL(float r, float g, float b){
		//on peut définir ici la couleur d'affichage de la forme
		glBegin(GL_QUADS);
		glColor3f(r, g, b);
		glVertex2f(getX()-diag*cos(angleIn-getAngle()), getY()+diag*sin(angleIn-getAngle()));
		glColor3f(r, g, b);
		glVertex2f(getX()-diag*cos(getAngle()+angleIn), getY()-diag*sin(getAngle()+angleIn));
		glColor3f(r, g, b);
		glVertex2f(getX()+diag*cos(angleIn-getAngle()), getY()-diag*sin(angleIn-getAngle()));
		glColor3f(r, g, b);
		glVertex2f(getX()+diag*cos(angleIn+getAngle()), getY()+diag*sin(angleIn+getAngle()));
		glEnd();
		glFlush();
	}
};
//head, legL, legR, hipL, hipR, com
//commande, getPos, undertaker, updateScore, reset, drawOpenGL
class Moustik {
private:
	Forme* ptrHead;
	Forme* ptrThighL;
	Forme* ptrThighR;
	Forme* ptrTibiaL;
	Forme* ptrTibiaR;
	b2RevoluteJoint* hipL;
	b2RevoluteJoint* hipR;
	b2RevoluteJoint* kneeL;
	b2RevoluteJoint* kneeR;
	int com;
	bool dead;
	float score;
	float angleMaxThigh; //[-angleMaxThigh, angleMaxThigh]
	float angleTibia; //[angleTibia, 0]
public:
	Moustik(b2World* ptrWorld, Coord pos){
		com = 0;
		dead=false;
		score=0.0;
		angleMaxThigh = 5*M_PI/6;
		angleTibia = -3*M_PI/4;
		//définition des formes
		ptrHead = new Forme(ptrWorld, pos, 0.25, 0.25, 0); //tête dynamique de 0.5x0.5
		ptrThighL = new Forme(ptrWorld, pos+Coord(-0.25,-0.75), 0.05, 0.5, 0); //cuisse dynamique de 0.1x1
		ptrThighR = new Forme(ptrWorld, pos+Coord(0.25,-0.75), 0.05, 0.5, 0);
		ptrTibiaL = new Forme(ptrWorld, pos+Coord(-0.25, -1.75), 0.05, 0.5, 0);
		ptrTibiaR = new Forme(ptrWorld, pos+Coord(0.25, -1.75), 0.05, 0.5, 0);
		//définition de la hipL et hipR
		b2RevoluteJointDef defhipL;
		b2RevoluteJointDef defhipR;
		b2RevoluteJointDef defkneeL;
		b2RevoluteJointDef defkneeR;
		defhipL.Initialize(ptrHead->getBody(), ptrThighL->getBody(), coord2bvec(ptrHead->getHL()));
		defhipR.Initialize(ptrHead->getBody(), ptrThighR->getBody(), coord2bvec(ptrHead->getHR()));
		defkneeL.Initialize(ptrThighL->getBody(), ptrTibiaL->getBody(), coord2bvec(ptrThighL->getPos()+Coord(0,-0.5)));
		defkneeR.Initialize(ptrThighR->getBody(), ptrTibiaR->getBody(), coord2bvec(ptrThighR->getPos()+Coord(0,-0.5)));
		//les jambes passent a travers la tête.
		defhipL.collideConnected = false;
		defhipR.collideConnected = false;
		defkneeL.collideConnected = false;
		defkneeR.collideConnected = false;
		//angles limites rotules hanches
		defhipL.enableLimit = true;
		defhipL.lowerAngle = -angleMaxThigh;
		defhipL.upperAngle = angleMaxThigh;
		defhipR.enableLimit = true;
		defhipR.lowerAngle = -angleMaxThigh;
		defhipR.upperAngle = angleMaxThigh;
		//angles limites rotules genoux
		defkneeL.enableLimit = true;
		defkneeL.lowerAngle = angleTibia;
		defkneeL.upperAngle = 0;
		defkneeR.enableLimit = true;
		defkneeR.lowerAngle = angleTibia;
		defkneeR.upperAngle = 0;
		//motorisation rotules.
		defhipL.enableMotor = false;
		defhipR.enableMotor = false;
		defkneeL.enableMotor = false;
		defkneeR.enableMotor = false;
		//créer le joint
		hipL = (b2RevoluteJoint*) ptrWorld->CreateJoint( &defhipL );
		hipR = (b2RevoluteJoint*) ptrWorld->CreateJoint( &defhipR );
		kneeL = (b2RevoluteJoint*) ptrWorld->CreateJoint( &defkneeL );
		kneeR = (b2RevoluteJoint*) ptrWorld->CreateJoint( &defkneeR );
	}
	~Moustik(){}
	void commande(b2World* ptrWorld){
		com=1+com%2;
		float minTorque=5;
		float maxTorque=50;
		if (dead) { //on tue tout.
			hipL->EnableMotor(false);
			hipR->EnableMotor(false);
		}	else if (com==1){
			hipL->EnableMotor(true);
  		hipL->SetMotorSpeed(M_PI/2); //1/4 de tour par seconde
  		hipL->SetMaxMotorTorque(maxTorque);
			hipR->EnableMotor(true);
  		hipR->SetMotorSpeed(-M_PI/2);
  		hipR->SetMaxMotorTorque(minTorque);
		} else if (com==2){
			hipR->EnableMotor(true);
  		hipR->SetMotorSpeed(M_PI/2);
  		hipR->SetMaxMotorTorque(maxTorque);
			hipL->EnableMotor(true);
  		hipL->SetMotorSpeed(-M_PI/2);
  		hipL->SetMaxMotorTorque(minTorque);
		}
	}
	Coord getPos(){
		return ptrHead->getPos();
	}
	void undertaker(){
		//permet de tester si le moustik et mort
		float limit=2e-2;
		if (ptrHead->getHL().y<limit| ptrHead->getHR().y<limit| ptrHead->getTL().y<limit| ptrHead->getTR().y<limit){
			dead=true;
			ptrHead->getBody()->SetActive(false);
			ptrThighL->getBody()->SetActive(false);
			ptrThighR->getBody()->SetActive(false);
		}
	}
	void updateScore(){
		score=max(ptrHead->getPos().x, score);
	}
	void reset(b2World* ptrWorld){
		delete ptrHead;
		delete ptrThighL;
		delete ptrThighR;
		delete ptrTibiaL;
		delete ptrTibiaR;
		//cette fonction ressemble au constructeur mais ne réinitialise pas le score, ni angleMaxThigh.
		com = 0;
		dead = false;
		//definition of bodies
		Coord pos(0, 3);
		ptrHead = new Forme(ptrWorld, pos, 0.25, 0.25, 0); //tête dynamique de 0.5x0.5
		ptrThighL = new Forme(ptrWorld, pos+Coord(-0.25,-0.75), 0.05, 0.5, 0); //jambe dynamique de 0.1x1
		ptrThighR = new Forme(ptrWorld, pos+Coord(0.25,-0.75), 0.05, 0.5, 0);
		ptrTibiaL = new Forme(ptrWorld, pos+Coord(-0.25, -1.75), 0.05, 0.5, 0);
		ptrTibiaR = new Forme(ptrWorld, pos+Coord(0.25, -1.75), 0.05, 0.5, 0);
		//définition des rotules
		b2RevoluteJointDef defhipL;
		b2RevoluteJointDef defhipR;
		b2RevoluteJointDef defkneeL;
		b2RevoluteJointDef defkneeR;
		defhipL.Initialize(ptrHead->getBody(), ptrThighL->getBody(), coord2bvec(ptrHead->getHL()));
		defhipR.Initialize(ptrHead->getBody(), ptrThighR->getBody(), coord2bvec(ptrHead->getHR()));
		defkneeL.Initialize(ptrThighL->getBody(), ptrTibiaL->getBody(), coord2bvec(ptrThighL->getPos()+Coord(0,-0.5)));
		defkneeR.Initialize(ptrThighR->getBody(), ptrTibiaR->getBody(), coord2bvec(ptrThighR->getPos()+Coord(0,-0.5)));
		//les jambes passent a travers la tête.
		defhipL.collideConnected = false;
		defhipR.collideConnected = false;
		defkneeL.collideConnected = false;
		defkneeR.collideConnected = false;
		//angles limites rotules
		defhipL.enableLimit = true;
		defhipL.lowerAngle = -angleMaxThigh;
		defhipL.upperAngle = angleMaxThigh;
		defhipR.enableLimit = true;
		defhipR.lowerAngle = -angleMaxThigh;
		defhipR.upperAngle = angleMaxThigh;
		//motorisation rotules
		defhipL.enableMotor = false;
		defhipR.enableMotor = false;
		defkneeL.enableMotor = false;
		defkneeR.enableMotor = false;
		//créer le joint
		hipL = (b2RevoluteJoint*) ptrWorld->CreateJoint( &defhipL );
		hipR = (b2RevoluteJoint*) ptrWorld->CreateJoint( &defhipR );
		kneeL = (b2RevoluteJoint*) ptrWorld->CreateJoint( &defkneeL );
		kneeR = (b2RevoluteJoint*) ptrWorld->CreateJoint( &defkneeR );
	}
	GLvoid drawOpenGL(){
		if (dead) { //si mort, il devient rouge
			ptrHead->drawOpenGL(1.0f, 0.7f, 0.7f);
			ptrThighL->drawOpenGL(1.0f, 0.7f, 0.7f);
			ptrThighR->drawOpenGL(1.0f, 0.7f, 0.7f);
			ptrTibiaL->drawOpenGL(1.0f, 0.7f, 0.7f);
			ptrTibiaR->drawOpenGL(1.0f, 0.7f, 0.7f);
		} else { //il est vivant
			ptrHead->drawOpenGL();
			ptrTibiaL->drawOpenGL();
			ptrTibiaR->drawOpenGL();
			if (com==1){
				ptrThighL->drawOpenGL(0.7f, 1.0f, 0.7f);
				ptrThighR->drawOpenGL();
			} else if (com==2){
				ptrThighL->drawOpenGL();
				ptrThighR->drawOpenGL(0.7f, 1.0f, 0.7f);
			} else if (com==0){
				ptrThighL->drawOpenGL();
				ptrThighR->drawOpenGL();
			}
		}
	}
};

/*
		VARIABLES GLOBALES #########################################################
*/

b2Vec2 gravity(0.0f, -10.0f);
b2World* ptrWorld= new b2World(gravity);

Moustik cousin(ptrWorld, Coord(0.0, 3.0));

Forme ground(ptrWorld, Coord(0.0,-1.0), 10.0, 1.0, 1);
bool grid=false;

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
	cousin.drawOpenGL();
	ground.drawOpenGL();
	if (grid){
		drawQuadrillage(-2,5,-2,5);
	}
	glLoadIdentity();
	gluOrtho2D(-2.0, 4.0, -1.0, 3.0);
	glutSwapBuffers();
}
GLvoid update(int fps){
	int dt=floor(1000/fps); //dt=16ms pour 60fps
	glutTimerFunc(dt, update, fps);

	ptrWorld->Step((float32)1/fps, (int32)8, (int32)3);
	cousin.undertaker(); //est-ce que il est mort ?
	cousin.updateScore();

	glutPostRedisplay();
}
GLvoid clavier(unsigned char touche, int x, int y) {
	switch(touche) {
		case 's':
		cousin.commande(ptrWorld);
		break; //on ne peut commander qu'une seule jambe a la fois.
		case 'g':
		grid=!grid;
		break;
		case 'i':
		cousin.reset(ptrWorld);
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
