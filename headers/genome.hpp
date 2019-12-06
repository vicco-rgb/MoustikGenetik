#pragma once
#include <stdio.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <iostream>
using namespace std;

class Genome {
private:
  int fitness;
  vector<int> seq; //correspond à la durée pendant laquelle le moustik est resté sur la même jambe.
  float tauxMutation;
public:
  Genome(vector<int>);
  Genome();
  ~Genome(){};
  vector<int> getRelativeSeq();
  vector<int> getAbsoluteSeq();
  int getFitness();
  Genome* cross(Genome*);
  Genome* mutation();
  bool betterThan(Genome*);
};

class Population {
private:
  vector<Genome*> genomes;
  int generation;
public:
  Population();
  Population(vector<Genome*>, int);
  Population(Population*);
  ~Population();

  vector<Genome*> getGenomes();
  void addGenome(Genome*);
  int getGeneration();

  Population bests(int);
  Population reproduction(Population);
  Population mutateGroup(Population);
  Population getChildren(int);
};

//surcharge de l'opérateur cout
ostream& operator<<(ostream&, Population);
ostream& operator<<(ostream&, Genome);

//fonction pour faire des tests
Population testInit();
