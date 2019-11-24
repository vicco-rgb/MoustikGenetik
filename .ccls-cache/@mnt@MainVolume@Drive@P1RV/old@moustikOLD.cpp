#include <math.h>
#include <iostream>
#include "moustik.h"

Moustik cousin(1,0.4,0.4); //L, H, T

Moustik::Moustik(float l, float h, float t){
	T=t;
	L=l;
	H=h;
	M.x=T/2;
	M.y=2*L+H/2;
	a1=0.0;
	a2=0.0;
	c=0.0;
	d1=0.0;
	d2=0.0;
	diag=sqrt(H*H/4+T*T/4);
	b=atan(H/T);
}
Moustik::Moustik(){
	T=0.5;
	L=1.0;
	H=0.5;
	M.x=T/2;
	M.y=2*L+H/2;
	a1=0.0;
	a2=0.0;
	c=0.0;
	d1=0.0;
	d2=0.0;
	diag=sqrt(H*H/4+T*T/4);
	b=atan(H/T);
}
Moustik::~Moustik(){
	//do nothing
}

Coord Moustik::getPosM() const {
	return M;
}
void Moustik::setPosM(Coord tete){
	M.x=tete.x;
	M.y=tete.y;
}
float Moustik::getL() const {
	return L;
}
float Moustik::getH() const {
	return H;
}
float Moustik::getT() const {
	return T;
}

float Moustik::getA1() const {
	return a1;
}
float Moustik::getA2() const {
	return a2;
}
float Moustik::getC() const {
	return c;
}
float Moustik::getD1() const {
	return d1;
}
float Moustik::getD2() const {
	return d2;
}

void Moustik::setA1(float a1){
	this->a1=a1;
}
void Moustik::setA2(float a2){
	this->a2=a2;
}
void Moustik::setC(float c){
	this->c=c;
}
void Moustik::setD1(float d1){
	this->d1=d1;
}
void Moustik::setD2(float d2){
	this->d2=d2;
}

Coord Moustik::getPosT1() const {
	Coord posT1;
	posT1.x = M.x - diag*cos(b-c);
	posT1.y = M.y + diag*sin(b-c);
	return posT1;
}
Coord Moustik::getPosT2() const {
	Coord posT2;
	posT2.x = M.x + diag*cos(b+c);
	posT2.y = M.y + diag*sin(b+c);
	return posT2;
}
Coord Moustik::getPosH1() const {
	Coord posH1;
	posH1.x = M.x - diag*cos(c+b);
	posH1.y = M.y - diag*sin(c+b);
	return posH1;
}
Coord Moustik::getPosH2() const {
	Coord posH2;
	posH2.x = M.x + diag*cos(b-c);
	posH2.y = M.y - diag*sin(b-c);
	return posH2;
}
Coord Moustik::getPosG1() const {
	Coord posG1=getPosH1();
	posG1.x += L*sin(a1+c);
	posG1.y -= L*cos(a1+c);
	return posG1;
}
Coord Moustik::getPosG2() const {
	Coord posG2=getPosH2();
	posG2.x += L*sin(d1+c);
	posG2.y -= L*cos(d1+c);
	return posG2;
}
Coord Moustik::getPosP1() const {
	Coord posP1=getPosG1();
	posP1.x += L*sin(a1+a2+c);
	posP1.y -= L*cos(a1+a2+c);
	return posP1;
}
Coord Moustik::getPosP2() const {
	Coord posP2=getPosG2();
	posP2.x += L*sin(d1+d2+c);
	posP2.y -= L*cos(d1+d2+c);
	return posP2;
}
std::ostream& operator<<(std::ostream&show, Coord const & point){
    //on surcharge opérateur <<
    show<<point.x <<";"<< point.y <<std::endl;
		return show;
}
