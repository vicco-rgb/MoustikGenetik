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
	Forme* ptrHead;
	Forme* ptrLegL;
	Forme* ptrLegR;
	b2RevoluteJointDef rotuleL;
	b2RevoluteJointDef rotuleR;
	int com;
public:
	Moustik(b2World* ptrWorld, Coord pos){
		com = 0;
		//définition des formes
		ptrHead = new Forme(ptrWorld, pos, 0.25, 0.25, 0); //tête dynamique de 0.5x0.5
		ptrLegL = new Forme(ptrWorld, pos+Coord(-0.25,-0.75), 0.05, 0.5, 0); //jambe dynamique de 0.1x1
		ptrLegR = new Forme(ptrWorld, pos+Coord(0.25,-0.75), 0.05, 0.5, 0); //jambe dynamique de 0.1x1
		//définition de la rotuleL et rotuleR
		rotuleL.Initialize(ptrHead->getBody(), ptrLegL->getBody(), coord2bvec(ptrHead->getHL()));
		rotuleR.Initialize(ptrHead->getBody(), ptrLegR->getBody(), coord2bvec(ptrHead->getHR()));
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
		cout<<com<<endl;
		if (com==1){
			rotuleL.enableMotor = true;
			rotuleL.motorSpeed = M_PI/2;
			rotuleL.maxMotorTorque = 1000; //how powerful is the motor ?
			rotuleR.enableMotor = false;
		} else if (com==2){
			rotuleR.enableMotor = true;
			rotuleR.motorSpeed = M_PI/2;
			rotuleR.maxMotorTorque = 1000; //how powerful is the motor ?
			rotuleL.enableMotor = false;
		}
	}
	Coord getPos(){
		return ptrHead->getPos();
	}
	GLvoid drawOpenGL(){
		ptrHead->drawOpenGL();
		ptrLegL->drawOpenGL();
		ptrLegR->drawOpenGL();
	}
};

/*
		VARIABLES GLOBALES #########################################################
*/

b2Vec2 gravity(0.0f, -10.0f);
b2World* ptrWorld= new b2World(gravity);

Moustik cousin(ptrWorld, Coord(0.0,2.0));

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
	glutPostRedisplay();
}
GLvoid clavier(unsigned char touche, int x, int y) {
	switch(touche) {
		case 's':
		cousin.commande();
		break; //on ne peut commander qu'une seule jambe a la fois.
		case 'g':
		grid=!grid;
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
