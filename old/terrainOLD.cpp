#include <math.h>
#include <vector>
#include <iostream>
#include "moustik.h"
#include "terrain.h"

std::vector<float> Xs={0.0,1.0,2.0,3.0,4.0,5.0};
std::vector<float> Ys={-1.0,-1.1,-0.9,-0.7,-0.8};
Terrain sol(Xs,Ys);

float moduloFloat(float a, float b){
  if (b<=0) {
    return 0.0;
  } else if (a<0) { //-16.3%8.1 = - (16.3%8.1)+8.1=8.0
    return -moduloFloat(-a,b)+b;
  } else if (a>=0 & a<b){
    return a;
  } else {
    return moduloFloat(a-b,b);
  }
}

Terrain::Terrain(){
  std::vector<float> vecteurX;
  std::vector<float> vecteurY;
  for (float i=0;i<5;i+=1){
    vecteurX.push_back(i);
    vecteurY.push_back(0.0);
  }
  Terrain(vecteurX,vecteurY);
}
Terrain::Terrain(std::vector<float> Xs, std::vector<float> Ys){
  Coord temp;
  for (int i=0; i<Xs.size(); i++){
    temp.x=Xs[i];
    temp.y=Ys[i];
    defSol.push_back(temp);
  }
}
Terrain::~Terrain(){}
float Terrain::getPortionFloat(float x)  {
  //si le Terrain est défini sur 8.3m, pour un moustique à 16m, on cherchera l'altitude équivalente à 16%8,3m
  return moduloFloat(x,defSol[defSol.size()].x);
}
float Terrain::getPortionInt(float x) {
  return floor(getPortionFloat(x));
}
float Terrain::getAltitude(float x) {
  //Terrain(portion) + portionTerrain*pente
  int portion=getPortionInt(x);
  return defSol[portion].y+getPortionFloat(x)*(defSol[portion+1].y-defSol[portion].y)/(defSol[portion+1].x-defSol[portion].x);
}
bool Terrain::gotThrough(Coord position){
  return position.y>getAltitude(position.x);
}
std::vector<Coord> Terrain::getCoord() {
  return defSol;
}
