#pragma once
#include <vector>
#include "moustik.h"

float moduloFloat(float,float);

class Terrain {
private:
  std::vector<Coord> defSol;
public:
  Terrain();
  Terrain(std::vector<float>, std::vector<float>);
  ~Terrain();
  std::vector<Coord> getCoord() ;
  float getPortionFloat(float)  ; //retourne un float entre 0 et defSol(defSol.size())->x
  float getPortionInt(float)  ; //retourne l'indice de defSol sur lequel se trouve le moustik
  float getAltitude(float)  ; //retourne l'altitude du sol en fonction de x.
  bool gotThrough(Coord) ; // retourne si un point est dans le sol ou non
};
