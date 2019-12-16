#include <iostream>
#include <math.h>
#include "physique.h"
#include "moustik.h"
using namespace std;

extern moustik cousin;
extern int com;
const float PI=2*acos(0);
const float dt=3e-5; //vitesse de la physique du jeu
const float epSol=1e-3;
const float tLim=PI/2;
extern bool chgmt;
int Ncasse=0;

void PhysicEngine(){
  //fonction qui gère l'appel aux autres
  if (cousin.getPosH1().y<terrain(cousin.getPosH1().x)+epSol | cousin.getPosH2().y<terrain(cousin.getPosH2().x)+epSol){ //fin du game!
    cout<<"Game Over"<<endl<<"your score : "<<cousin.getPosM().x<<endl;
    exit(0);
  }

  cassure(dt); //provoque la cassure lorsqu'il y a changement de jambe qui porte
  commande(dt);  //lève la jambe com

  if (cousin.getPosP1().y>terrain(cousin.getPosP1().x)+epSol & cousin.getPosP2().y>terrain(cousin.getPosP2().x)+epSol){       //chute libre
    chuteLibre(dt);

    gravCuisse1(dt); //Angles cuisse gauche
    gravCuisse2(dt);  //Angles cuisse droite
    gravTibia1(dt);   //Angles tibia gauche
    gravTibia2(dt);  //Angles tibia droit
  }

  if (com==2 & cousin.getPosP1().y<terrain(cousin.getPosP1().x)+epSol){ //rotation autour de jambe1
    gravTibia1(dt); //permet de faire claquer aussi
    rotJambe1(dt);  // rotation jambe1 fixe
    gravCuisse2(dt);
    gravTibia2(dt);
  }
  if (com==1 & cousin.getPosP2().y<terrain(cousin.getPosP2().x)+epSol){                 //rotation autour jambe 2
    gravTibia2(dt); //permet de faire claquer aussi
    rotJambe2(dt);  // rotation jambe2 fixe
    gravCuisse1(dt);
    gravTibia1(dt);
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
void operator+=(coord & point1, coord const& point2){
  // surcharge pour faire de la translation de coord
  point1.x+=point2.x;
  point1.y+=point2.y;
}
void operator-=(coord & point1, coord const& point2){
  // surcharge pour faire de la translation de coord
  point1.x-=point2.x;
  point1.y-=point2.y;
}
coord operator-(coord const& point1, coord const& point2){
  // surcharge pour faire de la translation de coord
  coord res;
  res.x=point1.x-point2.x;
  res.y=point1.y-point2.y;
  return res;
}
coord operator+(coord const& point1, coord const& point2){
  // surcharge pour faire de la translation de coord
  coord res;
  res.x=point1.x+point2.x;
  res.y=point1.y+point2.y;
  return res;
}
coord operator*(float const& scal, coord const& point){
  // surcharge pour faire de la translation de coord
  coord res=point;
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
  coord nPosM=cousin.getPosM(); //on construit la nouvelle coordonnée de M
  nPosM.y-=dt;
  cousin.setPosM(nPosM); //On fait tomber le moustik à proprement parler
}
void rotJambe1(float dt){
  coord p1init=cousin.getPosP1();
  coord p1m=cousin.getPosH1()-cousin.getPosP1();
  coord vecDep; //vecteur orthogonal à p1m
  vecDep.x=p1m.y; //qui pointe vers en haut à droite
  vecDep.y=-p1m.x;
  if (cousin.getC()+cousin.getA1()>tLim){  //angle limite lui permettant de ne pas tomber
    vecDep.x=-vecDep.x;
    vecDep.y=-vecDep.y; //il tombe en arrière

    cousin.setPosM(cousin.getPosM()-dt*vecDep);
    cousin.setA1(cousin.getA1()+dt);
  }else{
    cousin.setPosM(cousin.getPosM()+dt*vecDep);
    cousin.setA1(cousin.getA1()-dt);
  }
  p1init-=cousin.getPosP1();
  cousin.setPosM(cousin.getPosM()+p1init); //on remet P1 à son lieu initial.
}
void rotJambe2(float dt){
  coord p2init=cousin.getPosP2();
  coord p2m=cousin.getPosH2()-cousin.getPosP2();
  coord vecDep; //vecteur orthogonal à p2m
  vecDep.x=p2m.y;
  vecDep.y=-p2m.x;
  if (cousin.getC()+cousin.getD1()>tLim){  //angle limite lui permettant de ne pas tomber
    vecDep.x=-vecDep.x;
    vecDep.y=-vecDep.y; //il tombe en arrière
    cousin.setPosM(cousin.getPosM()-dt*vecDep);
    cousin.setD1(cousin.getD1()+dt);
  }else{
    cousin.setPosM(cousin.getPosM()+dt*vecDep);
    cousin.setD1(cousin.getD1()-dt);
  }
  p2init-=cousin.getPosP2();
  cousin.setPosM(cousin.getPosM()+p2init); //on remet P2 à son lieu initial.
}
void gravCuisse1(float dt){
  cousin.setA1(cousin.getA1()-10*(cousin.getA1()+cousin.getC())*dt);
}
void gravCuisse2(float dt){
  cousin.setD1(cousin.getD1()-10*(cousin.getD1()+cousin.getC())*dt);
}
void gravTibia1(float dt){
  if (com==1) { //alors c'est la gravité
    cousin.setA2(-10*dt*(cousin.getA1()+cousin.getC())+(1-10*dt)*cousin.getA2());
  } else if (com==2){ //alors on fait claquer.
    cousin.setA2(cousin.getA2()*0.5);
  }
}
void gravTibia2(float dt){
  if (com==1) { //alors on fait claquer
    cousin.setD2(cousin.getD2()*0.5);
  } else if (com==2){ //alors c'est la gravité.
    cousin.setD2(-10*dt*(cousin.getD1()+cousin.getC())+(1-10*dt)*cousin.getD2());
  }
}

void piedSol(){
  //traite les problèmes de passage à travers le sol. Le problème est traité différement selon si on s'appuie sur la jambe ou non.
  coord posP1=cousin.getPosP1();
  float hsol1=terrain(posP1.x);
  if (posP1.y<=hsol1 & com==2){ //la jambe 1 porte
    // coord vecDep=cousin.getPosP1();
    // vecDep.x=0;
    // cousin.setPosM(cousin.getPosM()-vecDep);
  } else if (posP1.y<=hsol1 & com==1){
    if (cousin.getC()+cousin.getA1()+cousin.getA2()<0){ //tibia à replier vers l'arrière
      cousin.setA2(-acos(cousin.getPosG1().y/cousin.getL()));
    }else{          //tibia à replier vers l'avant
      cousin.setA2(acos(cousin.getPosG1().y/cousin.getL()));
    }
  }
  coord posP2=cousin.getPosP2();
  float hsol2=terrain(posP2.x);
  if (posP2.y<=hsol2 & com==1){ //la jambe 2 porte
    // coord vecDep=cousin.getPosP2();
    // vecDep.x=0;
    // cousin.setPosM(cousin.getPosM()-vecDep);
  } else if (posP2.y<=hsol2 & com==2){
    if (cousin.getC()+cousin.getD1()+cousin.getD2()<0){ //tibia à replier vers l'arrière
      cousin.setD2(-acos(cousin.getPosG2().y/cousin.getL()));
    }else{                //tibia à replier vers l'avant
      cousin.setA2(acos(cousin.getPosG2().y/cousin.getL()));
    }
  }
}
float terrain(float x){
  //fonction qui renvoie pour toute valeur de x, la valeur de la hauteur du sol : y=terrain(x)
  if (x<5){
    return 0.0;
  } else {
    return 0.3*(x-5);
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
  if (chgmt){
    Ncasse=0;
  }
  if (Ncasse<50){
    if (com==1){ //il faut commencer par plier la jambe1
      cousin.setA1(cousin.getA1()+5*dt);
      cousin.setA2(cousin.getA2()-5*dt);
    } else if (com==2){ //il faut commencer par plier la jambe2
      cousin.setD1(cousin.getD1()+5*dt);
      cousin.setD2(cousin.getD2()-5*dt);
    }
    Ncasse++;
  }
}
