#include "physique.h"

extern int com;
extern unsigned int fps;

float bodyWidth=0.5f;
float bodyHeight=0.5f;
float groundWidth=10.0f;
float groundHeight=0.5f;

b2Body* body;
b2Body* groundBody;
b2World* ptrWorld;
void BigBang(){
  //DEFINITION MONDE
  b2Vec2 gravity(0.0f, 0.0f);
  b2World world(gravity);
  ptrWorld = &world;

  //DEFINITION SOL
  b2BodyDef groundBodyDef;
  groundBodyDef.type = b2_staticBody;
  groundBodyDef.position.Set(0.0f,-groundHeight);
  groundBody = world.CreateBody(&groundBodyDef); //renvoie un pointeur
  b2PolygonShape groundBox;
  groundBox.SetAsBox(groundWidth, groundHeight); //2*groundWith units wide and 2*groundHeight units tall;
  groundBody->CreateFixture(&groundBox, 0.0f);

  //DEFINITION CUBE
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(1.0f, 4.0f);
  bodyDef.angle = 0;
  body = world.CreateBody(&bodyDef); //renvoie un pointeur
  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(bodyWidth, bodyHeight);
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 0.0f;
  fixtureDef.friction = 0.3f;
  body->CreateFixture(&fixtureDef);
}
void PhysicEngine(){
  float32 timeStep = fps;
  int32 velocityIterations = 6;
  int32 positionIterations = 2;
  ptrWorld->Step(timeStep, velocityIterations, positionIterations);
}
