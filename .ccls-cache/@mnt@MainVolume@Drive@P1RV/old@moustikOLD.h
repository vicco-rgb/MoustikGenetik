#pragma once
#ifdef __APPLE__ //OpenGL
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

struct Coord {
	float x;
	float y;
};
std::ostream& operator<<(std::ostream&s, Coord const&);
class Moustik {
private:
	Coord M; // Coordonnées centre de masse de la tete
	float a1, a2, b, c, d1, d2; // angles
	float L,H,T; // longueur jambe, hauteur tete, largeur tete
	float diag; //demi diagonale de la tête
public:
	Moustik();
	Moustik(float,float,float);
	~Moustik();

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

	Coord getPosM() const; //centre masse
	void setPosM(Coord);
	Coord getPosT1() const; //tête
	Coord getPosT2() const;
	Coord getPosH1() const; //hanches
	Coord getPosH2() const;
	Coord getPosG1() const; //genoux
	Coord getPosG2() const;
	Coord getPosP1() const; //pieds
	Coord getPosP2() const;
};
