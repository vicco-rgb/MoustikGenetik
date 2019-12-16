#pragma once
#include "moustik.h"
#include <math.h>

void PhysicEngine();
float mPiPi(float);

void operator+=(coord &, coord const&); //surcharge opérateurs
void operator-=(coord &, coord const&);
coord operator-(coord const&, coord const&);
coord operator+(coord const&, coord const&);
coord operator*(float const&, coord const&);

void commande(float);
void chuteLibre(float);
void piedSol();
float terrain(float);
void cassure(float);

void rotJambe1(float);
void rotJambe2(float);

void gravCuisse1(float); //Angles cuisse gauche
void gravCuisse2(float);  //Angles cuisse droite
void gravTibia1(float);   //Angles tibia gauche
void gravTibia2(float);  //Angles tibia droit

void anglesPhysiques();
