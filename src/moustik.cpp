#include "moustik.hpp"
#include "genome.hpp"


//STRUCTURES ###################################################################

Coord::Coord(){
  x=0;
  y=0;
}
Coord::Coord(float x, float y){
  this->x=x;
  this->y=y;
}
void operator+=(Coord & point1, Coord const& point2){
  // surcharge pour faire de la translation de Coord
  point1.x+=point2.x;
  point1.y+=point2.y;
}
void operator-=(Coord & point1, Coord const& point2){
  // surcharge pour faire de la translation de Coord
  point1.x-=point2.x;
  point1.y-=point2.y;
}
Coord operator-(Coord const& point1, Coord const& point2){
  // surcharge pour faire de la translation de Coord
  Coord res;
  res.x=point1.x-point2.x;
  res.y=point1.y-point2.y;
  return res;
}
Coord operator+(Coord const& point1, Coord const& point2){
  // surcharge pour faire de la translation de Coord
  Coord res;
  res.x=point1.x+point2.x;
  res.y=point1.y+point2.y;
  return res;
}
Coord operator*(float const& scal, Coord const& point){
  // surcharge pour faire de la translation de Coord
  Coord res=point;
  res.x*=scal;
  res.y*=scal;
  return res;
}
ostream& operator<<(ostream& os, const Coord pos){
  os << pos.x << '\t' << pos.y << endl;
  return os;
}
b2Vec2 coord2bvec(Coord pos){
  b2Vec2 cpos(pos.x, pos.y);
  return cpos;
}

//FORME
//constructeurs
Forme::Forme(b2World* ptrWorld, Coord pos, float w, float h, int type){
  b2BodyDef bodyDef;

  this->type=type;
  if (type==0){ //dynamique
    bodyDef.type=b2_dynamicBody;
  } else if (type==1){
    bodyDef.type=b2_staticBody;
  } else if (type==2){
    bodyDef.type=b2_kinematicBody;
  }
  bodyDef.position.Set(pos.x, pos.y);

  body = ptrWorld->CreateBody(&bodyDef);
  //Box
  b2PolygonShape collisionBox;
  collisionBox.SetAsBox(w, h);
  //Fixtures
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &collisionBox;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;
  body->CreateFixture(&fixtureDef);

  width=w;
  height=h;
  diag=sqrt(w*w+h*h);
  angleIn=atan(height/width);
}
Forme::Forme(b2World* ptrWorld, Coord pos, int type){
  Forme(ptrWorld, pos, 1.0, 1.0, type);
}
Forme::~Forme(){
  body->GetWorld()->DestroyBody( body );
}
//get-set
float Forme::getX(){
  return body->GetPosition().x;
}
float Forme::getY(){
  return body->GetPosition().y;
}
Coord Forme::getPos(){
  Coord pos(body->GetPosition().x, body->GetPosition().y);
  return pos;
}
b2Body* Forme::getBody(){
  return body;
}
float Forme::getAngle(){
  return body->GetAngle();
}
float Forme::getAngleIn(){
  return angleIn;
}
float Forme::getDiag(){
  return diag;
}
Coord Forme::getHL(){
  return Coord(getX()-diag*cos(getAngle()+angleIn), getY()-diag*sin(getAngle()+angleIn));
}
Coord Forme::getHR(){
  return Coord(getX()+diag*cos(angleIn-getAngle()), getY()-diag*sin(angleIn-getAngle()));
}
Coord Forme::getTL(){
  return Coord(getX()-diag*cos(angleIn-getAngle()), getY()+diag*sin(angleIn-getAngle()));
}
Coord Forme::getTR(){
  return Coord(getX()+diag*cos(angleIn+getAngle()), getY()+diag*sin(angleIn+getAngle()));
}
//draw
GLvoid Forme::drawOpenGL(){
  //dessine le rectangle dans une couleur blanc pale par défaut
  glBegin(GL_QUADS);
  glColor3f(1.0f, 0.9f, 0.8f);
  glVertex2f(getX()-diag*cos(angleIn-getAngle()), getY()+diag*sin(angleIn-getAngle()));
  glColor3f(1.0f, 0.8f, 0.9f);
  glVertex2f(getX()-diag*cos(getAngle()+angleIn), getY()-diag*sin(getAngle()+angleIn));
  glColor3f(0.9f, 0.9f, 0.8f);
  glVertex2f(getX()+diag*cos(angleIn-getAngle()), getY()-diag*sin(angleIn-getAngle()));
  glColor3f(0.8f, 1.0f, 1.0f);
  glVertex2f(getX()+diag*cos(angleIn+getAngle()), getY()+diag*sin(angleIn+getAngle()));
  glEnd();
  glFlush();
}
GLvoid Forme::drawOpenGL(float r, float g, float b){
  //on peut définir ici la couleur d'affichage de la forme
  glBegin(GL_QUADS);
  glColor3f(r, g, b);
  glVertex2f(getX()-diag*cos(angleIn-getAngle()), getY()+diag*sin(angleIn-getAngle()));
  glColor3f(r, g, b);
  glVertex2f(getX()-diag*cos(getAngle()+angleIn), getY()-diag*sin(getAngle()+angleIn));
  glColor3f(r, g, b);
  glVertex2f(getX()+diag*cos(angleIn-getAngle()), getY()-diag*sin(angleIn-getAngle()));
  glColor3f(r, g, b);
  glVertex2f(getX()+diag*cos(angleIn+getAngle()), getY()+diag*sin(angleIn+getAngle()));
  glEnd();
  glFlush();
}

//MOUSTIK
//constructeurs
Moustik::Moustik(b2World* ptrWorld, Coord pos){
  age=0;
  com = 0;
  dead=false;
  angleMax = M_PI/2;
  controlType="human";
  seqWritten=false;
  genome= new Genome();
  //définition des formes
  ptrHead = new Forme(ptrWorld, pos, 0.25, 0.25, 0); //tête dynamique de 0.5x0.5, densité 1
  ptrLegL = new Forme(ptrWorld, pos+Coord(-0.25,-1.05), 0.05, 0.8, 0); //jambe dynamique de 0.1x1, densité 1
  ptrLegR = new Forme(ptrWorld, pos+Coord(0.25,-1.05), 0.05, 0.8, 0); //jambe dynamique de 0.1x1, densité 1.5
  //définition de la rotuleL et rotuleR
  b2RevoluteJointDef defRotuleL;
  b2RevoluteJointDef defRotuleR;
  defRotuleL.Initialize(ptrHead->getBody(), ptrLegL->getBody(), coord2bvec(ptrHead->getHL()));
  defRotuleR.Initialize(ptrHead->getBody(), ptrLegR->getBody(), coord2bvec(ptrHead->getHR()));
  //les jambes passent a travers la tête.
  defRotuleL.collideConnected = false;
  defRotuleR.collideConnected = false;
  //angles limites rotules
  defRotuleL.enableMotor = false;
  defRotuleR.enableMotor = false;
  defRotuleL.enableLimit = true;
  defRotuleL.lowerAngle = -angleMax;
  defRotuleL.upperAngle = angleMax;
  defRotuleR.enableLimit = true;
  defRotuleR.lowerAngle = -angleMax;
  defRotuleR.upperAngle = angleMax;
  //créer le joint
  rotuleL = (b2RevoluteJoint*) ptrWorld->CreateJoint( &defRotuleL );
  rotuleR = (b2RevoluteJoint*) ptrWorld->CreateJoint( &defRotuleR );
}
Moustik::~Moustik(){}
//get-set
float Moustik::getAbs(){
  return ptrHead->getPos().x;
}
string Moustik::getType(){
  return controlType;
}
bool Moustik::isDead(){
  return dead;
}
int Moustik::getAge(){
  return age;
}
void Moustik::upAge(){
  age++;
}
b2World* Moustik::getWorld(){
  return ptrHead->getBody()->GetWorld();
}
Genome* Moustik::getGenome(){
  return genome;
}
void Moustik::setGenome(Genome* genome){
  this->genome=genome;
}
//jeu
void Moustik::reset(){
  seqWritten=false;
  b2World* leWorld = ptrHead->getBody()->GetWorld();
  delete ptrHead;
  delete ptrLegL;
  delete ptrLegR;
  delete genome;

  com = 0;
  dead = false;
  genome=new Genome();

  //definition of bodies
  Coord pos(0, 2);
  ptrHead = new Forme(leWorld, pos, 0.25, 0.25, 0); //tête dynamique de 0.5x0.5
  ptrLegL = new Forme(leWorld, pos+Coord(-0.25,-1.05), 0.05, 0.8, 0); //jambe dynamique de 0.1x1
  ptrLegR = new Forme(leWorld, pos+Coord(0.25,-1.05), 0.05, 0.8, 0); //jambe dynamique de 0.1x1
  //définition de la rotuleL et rotuleR
  b2RevoluteJointDef defRotuleL;
  b2RevoluteJointDef defRotuleR;
  defRotuleL.Initialize(ptrHead->getBody(), ptrLegL->getBody(), coord2bvec(ptrHead->getHL()));
  defRotuleR.Initialize(ptrHead->getBody(), ptrLegR->getBody(), coord2bvec(ptrHead->getHR()));
  //les jambes passent a travers la tête.
  defRotuleL.collideConnected = false;
  defRotuleR.collideConnected = false;
  //angles limites rotules
  defRotuleL.enableMotor = false;
  defRotuleR.enableMotor = false;
  defRotuleL.enableLimit = true;
  defRotuleL.lowerAngle = -angleMax;
  defRotuleL.upperAngle = angleMax;
  defRotuleR.enableLimit = true;
  defRotuleR.lowerAngle = -angleMax;
  defRotuleR.upperAngle = angleMax;
  //créer le joint
  rotuleL = (b2RevoluteJoint*) getWorld()->CreateJoint( &defRotuleL );
  rotuleR = (b2RevoluteJoint*) getWorld()->CreateJoint( &defRotuleR );
}
void Moustik::commande(){
  genome->addAbsoluteDate(age);
  com=1+com%2;
  float upTorque=3;
  float downTorque=3;
  if (dead) { //on tue tout.
    rotuleL->EnableMotor(false);
    rotuleR->EnableMotor(false);
  }	else if (com==1){
    //la jambe L monte
    rotuleL->EnableMotor(true);
    rotuleL->SetMotorSpeed(M_PI/4); //1/4 de tour par seconde
    rotuleL->SetMaxMotorTorque(upTorque);
    rotuleR->EnableMotor(true);
    rotuleR->SetMotorSpeed(-M_PI/7);
    rotuleR->SetMaxMotorTorque(downTorque);
  } else if (com==2){
    //la jambe R monte
    rotuleR->EnableMotor(true);
    rotuleR->SetMotorSpeed(M_PI/4);
    rotuleR->SetMaxMotorTorque(upTorque);
    rotuleL->EnableMotor(true);
    rotuleL->SetMotorSpeed(-M_PI/7);
    rotuleL->SetMaxMotorTorque(downTorque);
  }
}
Coord Moustik::getPos(){
  return ptrHead->getPos();
}
bool Moustik::undertaker(){
  //permet de tester si le moustik et mort
  float limit=7e-2;
  if (ptrHead->getHL().y<limit| ptrHead->getHR().y<limit| ptrHead->getTL().y<limit| ptrHead->getTR().y<limit){
    ptrHead->getBody()->SetActive(false);
    ptrLegL->getBody()->SetActive(false);
    ptrLegR->getBody()->SetActive(false);
    dead=true;
    if (!seqWritten){
      writeGenome(false);
    }
  }
  return dead;
}
void Moustik::updateFitness(){
  genome->setFitness( max(getAbs(), genome->getFitness()) );
}
GLvoid Moustik::drawOpenGL(){
  if (dead) { //si mort, il devient rouge
    ptrHead->drawOpenGL(1.0f, 0.7f, 0.7f);
    ptrLegL->drawOpenGL(1.0f, 0.7f, 0.7f);
    ptrLegR->drawOpenGL(1.0f, 0.7f, 0.7f);
  } else { //il est vivant
    ptrHead->drawOpenGL();
    if (com==1){
      ptrLegL->drawOpenGL(0.7f, 1.0f, 0.7f);
      ptrLegR->drawOpenGL();
    } else if (com==2){
      ptrLegL->drawOpenGL();
      ptrLegR->drawOpenGL(0.7f, 1.0f, 0.7f);
    } else if (com==0){
      ptrLegL->drawOpenGL();
      ptrLegR->drawOpenGL();
    }
  }
}
void Moustik::writeGenome(bool erase){
  //on écrit la séquence de jeu dans un fichier texte.
  vector<int> sequence=genome->getRelativeSeq();
  float fitness = genome->getFitness();
  ofstream outfile;
  if (erase) { // on écrase le fichier actuel
    outfile.open("../sequences/"+controlType+".txt", ios_base::ate);
  } else { //on ajoute au fichier actuel
    outfile.open("../sequences/"+controlType+".txt", ios_base::app);
  }
  for (int i=0;i<sequence.size();i++){
    outfile<<sequence[i]<<"_";
  }
  outfile<<"\n"<<fitness<<"\n"; //correspond à fitness
  outfile.close();
  seqWritten=true;
}

//MOUSTIKIA
//constructeurs
MoustikIA::MoustikIA(b2World* ptrWorld, Coord pos, Genome* genome, string id) : Moustik(ptrWorld, pos){
  this->id=id;
  controlType="IA";
  this->genome=genome;
}
MoustikIA::MoustikIA(b2World* ptrWorld, Coord pos, vector<int> seq, string id) : Moustik(ptrWorld, pos){
  this->id=id;
  controlType="IA";
  genome = new Genome(seq, -1);
}
//get-set
void MoustikIA::setID(string newID){
  id=newID;
}
string MoustikIA::getID(){
  return id;
}
//jeu
void MoustikIA::activation(bool activate){
  ptrHead->getBody()->SetActive(activate);
  ptrLegL->getBody()->SetActive(activate);
  ptrLegR->getBody()->SetActive(activate);
}
void MoustikIA::play(){
  //fonction appelée a toutes les frames
  vector<int> sequence = genome->getAbsoluteSeq();
  if (count(sequence.begin(), sequence.end(), age%sequence.back())==1){
    //la séquence se joue en boucle grâce au modulo.
    commande();
  }
}
bool MoustikIA::undertaker(){
  //permet de tester si le moustik et mort
  float limit=7e-2;
  if (ptrHead->getHL().y<limit| ptrHead->getHR().y<limit| ptrHead->getTL().y<limit| ptrHead->getTR().y<limit){
    dead=true;
    ptrHead->getBody()->SetActive(false);
    ptrLegL->getBody()->SetActive(false);
    ptrLegR->getBody()->SetActive(false);
  }
  return dead;
}
