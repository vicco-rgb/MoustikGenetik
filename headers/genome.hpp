#pragma once
#include <stdio.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <iostream>
#include <istream> //permet de lire des fichiers
#include <string> //permet de convertir entier en string
using namespace std;

class MoustikIA;

class Genome {
private:
  float fitness; //vaut -1 quand la séquence n'a pas encore été testée
  vector<int> seq; //correspond à la durée pendant laquelle le moustik est resté sur la même jambe.
  float tauxMutation;
public:
  Genome(vector<int>, float); //sequence et fitness (mettre -1 par défaut.)
  Genome(int);
  Genome();
  ~Genome(){};
  vector<int> getRelativeSeq();
  vector<int> getAbsoluteSeq();
  float getFitness();
  void setFitness(float);
  void addAbsoluteDate(int);
  Genome* crossSplit(Genome*);
  Genome* crossAvg(Genome*);
  Genome* mutation();
  bool betterThan(Genome*);
};

class Population {
private:
  vector<MoustikIA*> moustiks;
  int generation;
public:
  Population();
  Population(vector<MoustikIA*>, int);
  Population(Population*);
  ~Population();

  vector<MoustikIA*> getMoustiks();
  void addMoustik(MoustikIA*);
  int getGeneration();
  void setGeneration(int);

  Population* bests(int);
  Population* reproduction(Population);
  Population* mutateGroup(Population);
  Population* getChildren(int);

  MoustikIA* playLive();
  void playOff();

  void writeGenomes();
  vector<Genome*> readGenomes(string);
};

//surcharge de l'opérateur cout
ostream& operator<<(ostream&, Population);
ostream& operator<<(ostream&, Genome);
ostream& operator<<(ostream&, vector<int>);
//fonction pour faire des tests
Population testInit();
