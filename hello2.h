#pragma once
#include <Box2D/Box2D.h>
#include <cmath>

struct Coord {
	float x;
	float y;
};

class Forme {
private:
  b2Body* body;
  float width;
  float height;
  int type;
  float diag; //demi-diagonale de la boite
  float angleIn; //angle intérieur = atan(height/width)
public:
  Forme(b2World*, Coord, int); //position + statique/dynamique/kinematik?
  Forme(b2World*, Coord, float, float, int); //position puis taille
  ~Forme();
};
