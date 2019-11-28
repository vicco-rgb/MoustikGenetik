#include "Hello2.h"

Forme::Forme(b2World* ptrWorld, Coord pos, int type){
  Forme(ptrWorld, pos, 1.0, 1.0, type);
}
Forme::Forme(b2World* ptrWorld, Coord pos, float w, float h, int type){
  b2BodyDef bodyDef;

  if (type==0){
    bodyDef.type = b2_dynamicBody;
    this->type="dynamique";
  } else if (type==1) {
    bodyDef.type = b2_staticBody;
    this->type="statique";
  } else if (type==2) {
    bodyDef.type = b2_kinematicBody;
    this->type="kinématique";
  }

	bodyDef.position.Set(pos.x, pos.y);

	body = world.CreateBody(&bodyDef);
  //dynamicbox taille 1.0 1.0 par défaut
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(w, h);
  //Fixtures
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);

  width=w;
  height=h;
  diag=sqrt(w*w+h*h);
  angleIn=atan(height/width);
}
~Forme::Forme(){}
