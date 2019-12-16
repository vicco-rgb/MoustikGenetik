#include <iostream>

// Fichiers d'en-têtes pour OpenGL et GLUT
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <math.h>
#include "moustik.h"

moustik cousin(1,0.4,0.4); //L, H, T

moustik::moustik(float l, float h, float t){
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
moustik::moustik(){
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
moustik::~moustik(){
	//do nothing
}

coord moustik::getPosM() const {
	return M;
}
void moustik::setPosM(coord tete){
	M.x=tete.x;
	M.y=tete.y;
}
float moustik::getL() const {
	return L;
}
float moustik::getH() const {
	return H;
}
float moustik::getT() const {
	return T;
}

float moustik::getA1() const {
	return a1;
}
float moustik::getA2() const {
	return a2;
}
float moustik::getC() const {
	return c;
}
float moustik::getD1() const {
	return d1;
}
float moustik::getD2() const {
	return d2;
}

void moustik::setA1(float a1){
	this->a1=a1;
}
void moustik::setA2(float a2){
	this->a2=a2;
}
void moustik::setC(float c){
	this->c=c;
}
void moustik::setD1(float d1){
	this->d1=d1;
}
void moustik::setD2(float d2){
	this->d2=d2;
}

coord moustik::getPosT1() const {
	coord posT1;
	posT1.x = M.x - diag*cos(b-c);
	posT1.y = M.y + diag*sin(b-c);
	return posT1;
}
coord moustik::getPosT2() const {
	coord posT2;
	posT2.x = M.x + diag*cos(b+c);
	posT2.y = M.y + diag*sin(b+c);
	return posT2;
}
coord moustik::getPosH1() const {
	coord posH1;
	posH1.x = M.x - diag*cos(c+b);
	posH1.y = M.y - diag*sin(c+b);
	return posH1;
}
coord moustik::getPosH2() const {
	coord posH2;
	posH2.x = M.x + diag*cos(b-c);
	posH2.y = M.y - diag*sin(b-c);
	return posH2;
}
coord moustik::getPosG1() const {
	coord posG1=getPosH1();
	posG1.x += L*sin(a1+c);
	posG1.y -= L*cos(a1+c);
	return posG1;
}
coord moustik::getPosG2() const {
	coord posG2=getPosH2();
	posG2.x += L*sin(d1+c);
	posG2.y -= L*cos(d1+c);
	return posG2;
}
coord moustik::getPosP1() const {
	coord posP1=getPosG1();
	posP1.x += L*sin(a1+a2+c);
	posP1.y -= L*cos(a1+a2+c);
	return posP1;
}
coord moustik::getPosP2() const {
	coord posP2=getPosG2();
	posP2.x += L*sin(d1+d2+c);
	posP2.y -= L*cos(d1+d2+c);
	return posP2;
}
