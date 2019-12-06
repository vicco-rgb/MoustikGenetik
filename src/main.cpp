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
#include <string> //pour convertir int 2 string
#include <vector>
#include <ctime> //pour avoir accès à l'horloge et aux nombres alatoires
#include <algorithm> //pour avoir des fonctions utiles avec vector
#include "genome.hpp"
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
//head, legL, legR, rotuleL, rotuleR, com
//commande, getPos, undertaker, updateScore, reset, getAbs, isIA, drawOpenGL, writeSeqDown
class Moustik {
protected:
	Forme* ptrHead;
	Forme* ptrLegL;
	Forme* ptrLegR;
	b2RevoluteJoint* rotuleL;
	b2RevoluteJoint* rotuleR;
	int com;
	bool dead;
	float score;
	float angleMax;
	string controlType;
	vector<int> sequence;
	bool seqWritten;
public:
	Moustik(b2World* ptrWorld, Coord pos){
		com = 0;
		dead=false;
		score=0.0;
		angleMax = 5*M_PI/6;
		controlType="human";
		ofstream outfile;
		outfile.open("../src/mySequence.txt", ios_base::ate); //écrase et créée un nouveau fichier
		outfile.close();
		seqWritten=false;
		//définition des formes
		ptrHead = new Forme(ptrWorld, pos, 0.25, 0.25, 0); //tête dynamique de 0.5x0.5
		ptrLegL = new Forme(ptrWorld, pos+Coord(-0.25,-0.75), 0.05, 0.5, 0); //jambe dynamique de 0.1x1
		ptrLegR = new Forme(ptrWorld, pos+Coord(0.25,-0.75), 0.05, 0.5, 0); //jambe dynamique de 0.1x1
		//définition de la rotuleL et rotuleR
		b2RevoluteJointDef defRotuleL;
		b2RevoluteJointDef defRotuleR;
		defRotuleL.Initialize(ptrHead->getBody(), ptrLegL->getBody(), coord2bvec(ptrHead->getHL()));
		defRotuleR.Initialize(ptrHead->getBody(), ptrLegR->getBody(), coord2bvec(ptrHead->getHR()));
		//les jambes passent a travers la tête.
		defRotuleL.collideConnected = false;
		defRotuleR.collideConnected = false;
		//angles limites rotules
		defRotuleL.enableMotor = false;
		defRotuleR.enableMotor = false;
		defRotuleL.enableLimit = true;
		defRotuleL.lowerAngle = -angleMax;
		defRotuleL.upperAngle = angleMax;
		defRotuleR.enableLimit = true;
		defRotuleR.lowerAngle = -angleMax;
		defRotuleR.upperAngle = angleMax;
		//créer le joint
		rotuleL = (b2RevoluteJoint*) ptrWorld->CreateJoint( &defRotuleL );
		rotuleR = (b2RevoluteJoint*) ptrWorld->CreateJoint( &defRotuleR );
	}
	~Moustik(){}
	void commande(b2World* ptrWorld, int nFrame){
		sequence.push_back(nFrame);
		com=1+com%2;
		float minTorque=5;
		float maxTorque=50;
		if (dead) { //on tue tout.
			rotuleL->EnableMotor(false);
			rotuleR->EnableMotor(false);
		}	else if (com==1){
			rotuleL->EnableMotor(true);
  		rotuleL->SetMotorSpeed(M_PI/2); //1/4 de tour par seconde
  		rotuleL->SetMaxMotorTorque(maxTorque);
			rotuleR->EnableMotor(true);
  		rotuleR->SetMotorSpeed(-M_PI/2);
  		rotuleR->SetMaxMotorTorque(minTorque);
		} else if (com==2){
			rotuleR->EnableMotor(true);
  		rotuleR->SetMotorSpeed(M_PI/2);
  		rotuleR->SetMaxMotorTorque(maxTorque);
			rotuleL->EnableMotor(true);
  		rotuleL->SetMotorSpeed(-M_PI/2);
  		rotuleL->SetMaxMotorTorque(minTorque);
		}
	}
	Coord getPos(){
		return ptrHead->getPos();
	}
	virtual void undertaker(int nFrame){
		//permet de tester si le moustik et mort
		float limit=2e-2;
		if (ptrHead->getHL().y<limit| ptrHead->getHR().y<limit| ptrHead->getTL().y<limit| ptrHead->getTR().y<limit){
			ptrHead->getBody()->SetActive(false);
			ptrLegL->getBody()->SetActive(false);
			ptrLegR->getBody()->SetActive(false);
			dead=true;
			if (!seqWritten){
				writeSeqDown(nFrame);
			}
		}
	}
	void updateScore(){
		score=max(ptrHead->getPos().x, score);
	}
	void reset(b2World* ptrWorld){
		seqWritten=false;
		delete ptrHead;
		delete ptrLegL;
		delete ptrLegR;
		//cette fonction ressemble au constructeur mais ne réinitialise pas le score, ni angleMax.
		com = 0;
		dead = false;
		//definition of bodies
		Coord pos(0, 2);
		ptrHead = new Forme(ptrWorld, pos, 0.25, 0.25, 0); //tête dynamique de 0.5x0.5
		ptrLegL = new Forme(ptrWorld, pos+Coord(-0.25,-0.75), 0.05, 0.5, 0); //jambe dynamique de 0.1x1
		ptrLegR = new Forme(ptrWorld, pos+Coord(0.25,-0.75), 0.05, 0.5, 0); //jambe dynamique de 0.1x1
		//définition de la rotuleL et rotuleR
		b2RevoluteJointDef defRotuleL;
		b2RevoluteJointDef defRotuleR;
		defRotuleL.Initialize(ptrHead->getBody(), ptrLegL->getBody(), coord2bvec(ptrHead->getHL()));
		defRotuleR.Initialize(ptrHead->getBody(), ptrLegR->getBody(), coord2bvec(ptrHead->getHR()));
		//les jambes passent a travers la tête.
		defRotuleL.collideConnected = false;
		defRotuleR.collideConnected = false;
		//angles limites rotules
		defRotuleL.enableMotor = false;
		defRotuleR.enableMotor = false;
		defRotuleL.enableLimit = true;
		defRotuleL.lowerAngle = -angleMax;
		defRotuleL.upperAngle = angleMax;
		defRotuleR.enableLimit = true;
		defRotuleR.lowerAngle = -angleMax;
		defRotuleR.upperAngle = angleMax;
		//créer le joint
		rotuleL = (b2RevoluteJoint*) ptrWorld->CreateJoint( &defRotuleL );
		rotuleR = (b2RevoluteJoint*) ptrWorld->CreateJoint( &defRotuleR );
	}
	float getAbs(){
		return ptrHead->getPos().x;
	}
	string type(){
		return controlType;
	}
	GLvoid drawOpenGL(){
		if (dead) { //si mort, il devient rouge
			ptrHead->drawOpenGL(1.0f, 0.7f, 0.7f);
			ptrLegL->drawOpenGL(1.0f, 0.7f, 0.7f);
			ptrLegR->drawOpenGL(1.0f, 0.7f, 0.7f);
		} else { //il est vivant
			ptrHead->drawOpenGL();
			if (com==1){
				ptrLegL->drawOpenGL(0.7f, 1.0f, 0.7f);
				ptrLegR->drawOpenGL();
			} else if (com==2){
				ptrLegL->drawOpenGL();
				ptrLegR->drawOpenGL(0.7f, 1.0f, 0.7f);
			} else if (com==0){
				ptrLegL->drawOpenGL();
				ptrLegR->drawOpenGL();
			}
		}
	}
	void writeSeqDown(int nFrame){
		//on écrit la séquence de jeu dans un fichier texte.
		ofstream outfile;
		outfile.open("../src/mySequence.txt", ios_base::app); //pour rajouter
		for (int i=0;i<sequence.size();i++){
				outfile<<sequence[i]<<"\t";
		}
		outfile<<"\n"<<nFrame<<"\n"; //correspond à fitness
		outfile.close();
		seqWritten=true;
	}
};
//ptrGenome
//play
class MoustikIA : public Moustik {
protected:
	Genome* ptrGenome;
	int id;
public:
	MoustikIA(b2World* ptrWorld, Coord pos, Genome genome) : Moustik(ptrWorld, pos){
		controlType="IA";
		ptrGenome=&genome;
	}
	void play(b2World* ptrWorld, int nFrame){
		//fonction appelée a toutes les frames
		vector<int> seq=ptrGenome->getAbsoluteSeq();
		if (count(seq.begin(), seq.end(), nFrame)==1){
			commande(ptrWorld, nFrame);
		}
	}
	virtual void undertaker(){
		//permet de tester si le moustik et mort
		float limit=2e-2;
		if (ptrHead->getHL().y<limit| ptrHead->getHR().y<limit| ptrHead->getTL().y<limit| ptrHead->getTR().y<limit){
			dead=true;
			ptrHead->getBody()->SetActive(false);
			ptrLegL->getBody()->SetActive(false);
			ptrLegR->getBody()->SetActive(false);
			//on écrit la séquence de jeu dans un fichier texte.
			ofstream outfile;
			string filename="seqIA-"+to_string(id)+".txt";
			outfile.open(filename);
			for (int i=0;i<sequence.size();i++){
					outfile<<sequence[i]<<"-";
			}
			outfile.close();
		}
	}
};

/*
		VARIABLES GLOBALES #########################################################
*/

b2World* ptrWorld= new b2World(b2Vec2(0.0f,-9.81f));

Moustik cousin(ptrWorld, Coord(0.0,2.0));

Forme ground(ptrWorld, Coord(0.0,-1.0), 10.0, 1.0, 1);
bool grid=false;
int nFrame=0;

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

	// cousin.drawOpenGL();
	cousin.drawOpenGL();
	ground.drawOpenGL();
	if (grid){
		drawQuadrillage(floor(cousin.getAbs())-2,floor(cousin.getAbs())+5,-2,3);
	}
	glLoadIdentity();
	gluOrtho2D(cousin.getAbs()-2.0, cousin.getAbs()+4.0, -1.0, 3.0);
	glutSwapBuffers();
}
GLvoid update(int fps){
	nFrame++;
	int dt=floor(1000/fps); //dt=16ms pour 60fps
	glutTimerFunc(dt, update, fps);

	ptrWorld->Step((float32)1/fps, (int32)8, (int32)3);
	cousin.undertaker(nFrame); //est-ce que il est mort ?
	cousin.updateScore();

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

	// Population popInit = testInit();
	// Population children = popInit.getChildren(3);
	// cout<<popInit<<endl<<"####### CHILDREN ##########"<<endl;
  // cout<<children;

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
