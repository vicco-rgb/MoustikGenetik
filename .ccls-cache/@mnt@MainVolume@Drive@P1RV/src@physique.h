#pragma once
#include <math.h>
#include <vector>

void PhysicEngine();
float mPiPi(float);

void operator+=(Coord &, Coord const&); //surcharge opérateurs
void operator-=(Coord &, Coord const&);
Coord operator-(Coord const&, Coord const&);
Coord operator+(Coord const&, Coord const&);
Coord operator*(float const&, Coord const&);

void commande(float);
void chuteLibre(float);
void piedSol();
void cassure(float);

void rotJambe(float,int);
void gravCuisse(float, int);
void gravTibia(float);

void anglesPhysiques();
