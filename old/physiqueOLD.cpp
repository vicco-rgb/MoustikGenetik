#include "physique.h"

extern Moustik cousin;
extern Terrain sol;
extern int com;
const float PI=2*acos(0);
const float dt=5e-5; //vitesse de la physique du jeu
const float epSol=1e-3;
const float tLim=PI/6;
extern bool chgmt;
int Ncasse=0;

void PhysicEngine(){
  //fonction qui gère l'appel aux autres
  if (cousin.getPosH1().y<sol.getAltitude(cousin.getPosH1().x)+epSol | cousin.getPosH2().y<sol.getAltitude(cousin.getPosH2().x)+epSol){ //fin du game!
    cout<<"Game Over"<<endl<<"your score : "<<cousin.getPosM().x<<endl;
    exit(0);
  }

  // cassure(dt); //provoque la cassure lorsqu'il y a changement de jambe qui porte
  commande(dt);  //lève la jambe com

  if (cousin.getPosP1().y>sol.getAltitude(cousin.getPosP1().x)+epSol & cousin.getPosP2().y>sol.getAltitude(cousin.getPosP2().x)+epSol){       //chute libre
    chuteLibre(dt);

    gravCuisse(dt,1); //Angles cuisse gauche
    gravCuisse(dt,2);  //Angles cuisse droite
    gravTibia(dt,1);  //Angles tibias
    gravTibia(dt,2);
  }

  if (com==2 & cousin.getPosP1().y<sol.getAltitude(cousin.getPosP1().x)+epSol){ //rotation autour de jambe1
    gravTibia(dt,1);
    rotJambe(dt,1);  // rotation jambe1 fixe
    gravCuisse(dt,2);
    gravTibia(dt,2);
  }
  if (com==1 & cousin.getPosP2().y<sol.getAltitude(cousin.getPosP2().x)+epSol){                 //rotation autour jambe 2
    gravTibia(dt,1);
    rotJambe(dt,2);  // rotation jambe2 fixe
    gravCuisse(dt,1);
    gravTibia(dt,2);
  }

  piedSol();
  anglesPhysiques();   //on ne garde que des angles entre -pi et pi pour ne pas avoir de problèmes type tibia-hélicoptère.}
}
float mPiPi(float x){
  // fonction récursive de modulo
  if (x<=PI & x>=-PI){
    return x;
  } else if (x<=-PI){ //x trop petit
    return mPiPi(x+2*PI);
  } else if (x>=PI){ //x trop grand
    return mPiPi(x-2*PI);
  } else {
    return 0.0;
  }
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

void commande(float dt){
  float forceCom=6; //correspond a la force que l'utilisatuer a pour relever la jambe.
  if (com==1){
    //on ne peut faire monter la jambe que si le genou ne touche pas le sol
    if (cousin.getPosG1().y>0){
      cousin.setA1(cousin.getA1()+dt*forceCom);
    }
  }
  if (com==2){
    if (cousin.getPosG2().y>0){
      cousin.setD1(cousin.getD1()+dt*forceCom);
    }
  }
}
void chuteLibre(float dt){
  //le moustique est en l'air. donc acceleration =g
  float posMInit=cousin.getPosM().y;// calcul de la nouvelle vitesse du moustique
  Coord nPosM=cousin.getPosM(); //on construit la nouvelle Coordonnée de M
  nPosM.y-=dt;
  cousin.setPosM(nPosM); //On fait tomber le moustik à proprement parler
}
void rotJambe(float dt, int nJambe){
  Coord pInit, ph;
  float angleJambe;
  //VARIABLES
  if (nJambe==1){
    pInit=cousin.getPosP1();
    ph=cousin.getPosH1()-cousin.getPosP1();
    angleJambe=cousin.getA1();
  } else if (nJambe==2){
    pInit=cousin.getPosP2();
    ph=cousin.getPosH2()-cousin.getPosP2();
    angleJambe=cousin.getD1();
  }
  Coord vecDep;
  vecDep.x=ph.y; //qui pointe vers en haut à droite
  vecDep.y=-ph.x;

  //CALCULS
  int inv=1;
  if (angleJambe>tLim+cousin.getC()){  //Le moustik tombe en arrière
    vecDep-=vecDep;
    inv=-1;
  }
  if (nJambe==1) cousin.setA1(angleJambe+inv*dt);
  if (nJambe==2) cousin.setD1(angleJambe+inv*dt);
  cousin.setPosM(cousin.getPosM()+dt*vecDep);

  //ON REPLACE LES PIEDS
  if (nJambe==1) pInit-=cousin.getPosP1();
  if (nJambe==2) pInit-=cousin.getPosP2();
  cousin.setPosM(cousin.getPosM()+pInit); //on remet Pi à son lieu initial.
}
void gravCuisse(float dt, int nCuisse){
  float coeff=10;
  if (nCuisse==1) cousin.setA1(cousin.getA1()-coeff*(cousin.getA1()+cousin.getC())*dt);
  if (nCuisse==2) cousin.setD1(cousin.getD1()-coeff*(cousin.getD1()+cousin.getC())*dt);
}
void gravTibia(float dt, int nTibia){
  float coeffClaque=(1-1e4*dt);
  if (nTibia==1){
    if (com==1) { //alors 1=gravité 2=claque
      cousin.setA2(-10*dt*(cousin.getA1()+cousin.getC())+(1-10*dt)*cousin.getA2());
    } else if (com==2){ //alors l'inverse
    cousin.setA2(cousin.getA2()*coeffClaque);
  }
} else {
  if (com==1) { //alors 1=gravité 2=claque
    cousin.setD2(cousin.getD2()*coeffClaque);
  } else if (com==2){ //alors l'inverse
  cousin.setD2(-10*dt*(cousin.getD1()+cousin.getC())+(1-10*dt)*cousin.getD2());
}
}
}
void piedSol(){
  //traite les problèmes de passage à travers le sol.
  //Distinction de cas suivant la jambe porteuse.
  //PIED1
  Coord posP1=cousin.getPosP1();
  float hsol1=sol.getAltitude(posP1.x);
  if (posP1.y<=hsol1 & com==2){ //la jambe 1 porte
    Coord vecDep;
    vecDep.x=0;
    vecDep.y=hsol1-posP1.y;
    cousin.setPosM(cousin.getPosM()+vecDep); //on translate vers le haut tout le moustik.
  } else if (posP1.y<=hsol1 & com==1){ //la jambe ne porte pas et on plie le tibia vers l'arrière
  cousin.setA2(-acos(cousin.getPosG1().y/cousin.getL()));
}

//PIED2
Coord posP2=cousin.getPosP2();
float hsol2=sol.getAltitude(posP2.x);
if (posP2.y<=hsol2 & com==1){ //la jambe2 porte
  Coord vecDep;
  vecDep.x=0;
  vecDep.y=hsol2-posP2.y;
  cousin.setPosM(cousin.getPosM()+vecDep);
} else if (posP2.y<=hsol2 & com==2){ //la jambe2 ne porte pas et on replie le tibia vers l'arrière
cousin.setD2(-acos(cousin.getPosG2().y/cousin.getL()));
}
}
void anglesPhysiques(){
  cousin.setA1(mPiPi(cousin.getA1()));
  cousin.setA2(mPiPi(cousin.getA2()));
  cousin.setD1(mPiPi(cousin.getD1()));
  cousin.setD2(mPiPi(cousin.getD2()));
  if (cousin.getA2()>0){
    cousin.setA2(0); //le tibia ne peut pas remonter au-dessus du genoux.
  }
  if (cousin.getD2()>0){
    cousin.setD2(0); //le tibia ne peut pas remonter au-dessus du genoux.
  }
  // if (cousin.getA1()>PI/2){
  //   cousin.setA1(PI/2);
  // }
  // if (cousin.getD1()>PI/2){
  //   cousin.setD1(PI/2);
  // }
}

void cassure(float dt){
  float coeffCasse=dt;
  if (chgmt){
    Ncasse=0;
  }
  if (Ncasse<100){
    if (com==1){ //il faut commencer par plier la jambe1
      cousin.setA1(cousin.getA1()+coeffCasse);
      cousin.setA2(cousin.getA2()-coeffCasse);
    } else if (com==2){ //il faut commencer par plier la jambe2
      cousin.setD1(cousin.getD1()+coeffCasse);
      cousin.setD2(cousin.getD2()-coeffCasse);
    }
    Ncasse++;
  }
}
