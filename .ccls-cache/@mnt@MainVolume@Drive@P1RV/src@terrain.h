#pragma once
#include <vector>
#include "moustik.h"

class Terrain {
private:
  std::vector<Coord> defSol;
public:
  Terrain();
  Terrain(std::vector<float>, std::vector<float>);
  ~Terrain();
  std::vector<Coord> getCoord() ;
  float getPortionFloat(float) const ; //retourne un float entre 0 et defSol(defSol.size())->x
  float getPortionInt(float) const ; //retourne l'indice de defSol sur lequel se trouve le moustik
  float getAltitude(float) const ; //retourne l'altitude du sol en fonction de x.
};
