#pragma once

#ifdef __APPLE__ //OpenGL et GLUT
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif
#include <cmath> // pour cos, sin, tan et M_PI
#include <iostream> //pour écrire et lire dans un terminal
#include <fstream> //pour écrire dans un fichier texte
#include <string> //pour convertir int 2 string
#include <ctime> //pour avoir accès à l'horloge et aux nombres alatoires
#include <vector>
#include <algorithm> //pour avoir des fonctions utiles avec vector
#include <Box2D/Box2D.h> //physique
using namespace std;

class Genome;

struct Coord {
  float x;
  float y;
  Coord();
  Coord(float, float);
};
void operator+=(Coord &, Coord const&);
void operator-=(Coord &, Coord const&);
Coord operator-(Coord const&, Coord const&);
Coord operator+(Coord const&, Coord const&);
Coord operator*(float const&, Coord const&);
ostream& operator<<(ostream& , const Coord);
b2Vec2 coord2bvec(Coord);

class Forme {
private:
  b2Body* body;
  float width;
  float height;
  int type; //0, 1 ou 2 = dynamique, statique, kinématique
  float diag; //demi-diagonale de la boite
  float angleIn; //angle intérieur = atan(height/width)
public:
	Forme(b2World*, Coord, float, float, int);
	Forme(b2World*, Coord, int);
	~Forme();
	float getX();
	float getY();
	Coord getPos();
	b2Body* getBody();
	float getAngle();
	float getAngleIn();
	float getDiag();
	Coord getHL();
	Coord getHR();
	Coord getTL();
	Coord getTR();
	GLvoid drawOpenGL();
	GLvoid drawOpenGL(float, float, float);
};

class Moustik {
protected:
	Forme* ptrHead;
	Forme* ptrLegL;
	Forme* ptrLegR;
	b2RevoluteJoint* rotuleL;
	b2RevoluteJoint* rotuleR;
	int com;
	bool dead;
	float angleMax;
	string controlType;
	Genome* genome;
	bool seqWritten;
  int age;
public:
  //constructeurs
	Moustik(b2World*, Coord);
	~Moustik();
	void commande();
	Coord getPos();
  bool isDead();
  int getAge();
	Genome* getGenome(); //on pourrait rendre cette fonction virtuelle
  void setGenome(Genome*);
  void upAge();
  b2World* getWorld();
	virtual bool undertaker();
	void updateFitness();
	void reset();
	float getAbs();
	string getType();
	GLvoid drawOpenGL();
	void writeGenome(bool);
};

class MoustikIA : public Moustik {
protected:
	string id; //de la forme generation/numero
public:
	MoustikIA(b2World*, Coord, Genome*, string);
	MoustikIA(b2World*, Coord, vector<int>, string);
  void setID(string);
  string getID();
  void activation(bool);
	void play();
	virtual bool undertaker();
};
