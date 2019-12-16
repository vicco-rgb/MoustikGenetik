#pragma once
#ifdef __APPLE__ //OpenGL
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

struct coord {
	float x;
	float y;
};

class moustik {
private:
	coord M; // coordonnées centre de masse de la tete
	float a1, a2, b, c, d1, d2; // angles
	float L,H,T; // longueur jambe, hauteur tete, largeur tete
	float diag; //demi diagonale de la tête
public:
	moustik();
	moustik(float,float,float);
	~moustik();

	float getL() const;
	float getH() const;
	float getT() const;

	float getA1() const;
	float getA2() const;
	float getC() const;
	float getD1() const;
	float getD2() const;

	void setA1(float);
	void setA2(float);
	void setC(float);
	void setD1(float);
	void setD2(float);

	coord getPosM() const; //centre masse
	void setPosM(coord);
	coord getPosT1() const; //tête
	coord getPosT2() const;
	coord getPosH1() const; //hanches
	coord getPosH2() const;
	coord getPosG1() const; //genoux
	coord getPosG2() const;
	coord getPosP1() const; //pieds
	coord getPosP2() const;
};
