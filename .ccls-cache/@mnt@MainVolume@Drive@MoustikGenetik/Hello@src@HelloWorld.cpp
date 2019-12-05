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
// #include "hello2.cpp"

using namespace std;
struct Coord {
	float x;
	float y;
};
class Forme {
private:
  b2Body* body;
  float width;
  float height;
  int type; //0, 1 ou 2 = dynamique, statique, kinématique
  float diag; //demi-diagonale de la boite
  float angleIn; //angle intérieur = atan(height/width)
public:
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
	float getAngle(){
		return body->GetAngle();
	}
	float getAngleIn(){
		return angleIn;
	}
	float getDiag(){
		return diag;
	}
};
class Moustik {
public:
	Forme head;
	Forme legL;
	Forme legR;
	int com;
private:
	
};
int main(int argc, char** argv){
	B2_NOT_USED(argc);
	B2_NOT_USED(argv);
	// Define the gravity vector.
	b2Vec2 gravity(0.0f, -10.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	b2World world(gravity);
	b2World* ptr_World=&world;

	Coord pos;
	pos.x=0.1;
	pos.y=1.0;
	Forme leNewBody(ptr_World, pos, 0.5, 0.5, 0);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -1.0f);
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 1.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	b2BodyDef bodyDef;	//HEAD
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	b2Body* body = world.CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.5f, 0.5f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);

	b2Body* leg = world.CreateBody(&bodyDef);
	dynamicBox.SetAsBox(0.1f, 0.5f);
	leg->CreateFixture(&fixtureDef);

	b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = body;
	revoluteJointDef.bodyB = leg;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.localAnchorA.Set(-0.5f,-0.5f);
	revoluteJointDef.localAnchorB.Set(0.05f,0.25f);
	b2RevoluteJoint* joint1 = (b2RevoluteJoint*)world.CreateJoint( &revoluteJointDef );


	// Prepare for simulation. Typically we use a time step of 1/60 of a
	// second (60Hz) and 10 iterations. This provides a high quality simulation
	// in most game scenarios.
	float32 timeStep = 1.0f / 3000.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);	// Choix du mode d'affichage (ici RVB)
	glutInitWindowPosition(0,0); // Position initiale de la fenetre GLUT
	glutInitWindowSize(1000,600); // Taille initiale de la fenetre GLUT
	glutCreateWindow("DaddyLongLeg - Algo Genetique"); // Creation de la fenetre GLUT
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);	// Définition de la couleur d'effacement du framebuffer //le fond est noir
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	while (true){

		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		ptr_World->Step(timeStep, velocityIterations, positionIterations);
		// Now print the position and angle of the body.
		b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();
		//ON AFFICHE

		float diag=sqrt(2)*0.5;
		float b=45;
		float c=body->GetAngle();
		glClear(GL_COLOR_BUFFER_BIT); 	// Effacement du frame buffer
		glBegin(GL_QUADS); //GROUND
		glColor3f(0.87f, 0.72f, 0.64f);
		glVertex2f(-10,0);
		glColor3f(0.87f, 0.72f, 0.64f);
		glVertex2f(-10,-1);
		glColor3f(0.87f, 0.72f, 0.64f);
		glVertex2f(10,-1);
		glColor3f(0.87f, 0.72f, 0.64f);
		glVertex2f(10,0);
		glEnd();
		glFlush();

		// glBegin(GL_QUADS); //carre
		// glColor3f(0.5f, 0.5f, 0.5f);
		// glVertex2f(body->GetPosition().x-diag*cos(b-c), body->GetPosition().y+diag*sin(b-c));
		// glColor3f(1.0f, 0.5f, 0.5f);
		// glVertex2f(body->GetPosition().x-diag*cos(c+b), body->GetPosition().y-diag*sin(c+b));
		// glColor3f(0.5f, 0.5f, 0.5f);
		// glVertex2f(body->GetPosition().x+diag*cos(b-c), body->GetPosition().y-diag*sin(b-c));
		// glColor3f(0.5f, 0.5f, 0.5f);
		// glVertex2f(body->GetPosition().x+diag*cos(b+c), body->GetPosition().y+diag*sin(b+c));
		// glEnd();
		// glFlush();
		//
		// diag=sqrt(2)*0.5;
		// b=atan(5);
		// c=body->GetAngle();
		// glBegin(GL_QUADS); //leg
		// glColor3f(0.5f, 0.5f, 0.5f);
		// glVertex2f(leg->GetPosition().x-diag*cos(b-c), leg->GetPosition().y+diag*sin(b-c));
		// glColor3f(0.5f, 0.5f, 0.5f);
		// glVertex2f(leg->GetPosition().x-diag*cos(c+b), leg->GetPosition().y-diag*sin(c+b));
		// glColor3f(0.5f, 0.5f, 0.5f);
		// glVertex2f(leg->GetPosition().x+diag*cos(b-c), leg->GetPosition().y-diag*sin(b-c));
		// glColor3f(0.5f, 1.0f, 0.5f);
		// glVertex2f(leg->GetPosition().x+diag*cos(b+c), leg->GetPosition().y+diag*sin(b+c));
		// glEnd();
		// glFlush();

		diag=leNewBody.getDiag();
		b=leNewBody.getAngleIn();
		c=leNewBody.getAngle();
		glBegin(GL_QUADS); //boiteTest
		glColor3f(0.5f, 0.4f, 0.4f);
		glVertex2f(leNewBody.getX()-diag*cos(b-c), leNewBody.getY()+diag*sin(b-c));
		glColor3f(0.4f, 0.4f, 0.4f);
		glVertex2f(leNewBody.getX()-diag*cos(c+b), leNewBody.getY()-diag*sin(c+b));
		glColor3f(0.4f, 0.4f, 0.4f);
		glVertex2f(leNewBody.getX()+diag*cos(b-c), leNewBody.getY()-diag*sin(b-c));
		glColor3f(0.4f, 1.0f, 0.5f);
		glVertex2f(leNewBody.getX()+diag*cos(b+c), leNewBody.getY()+diag*sin(b+c));
		glEnd();
		glFlush();

		glLoadIdentity();
		gluOrtho2D(-2.0, 4.0, -1.0, 3.0);
		glutSwapBuffers();
	}

	currentTime = glutGet(GLUT_ELAPSED_TIME);

	// When the world destructor is called, all bodies and joints are freed. This can
	// create orphaned pointers, so be careful about your world management.


	return 0;
}
