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
  vector<int> seq;
  float totMutation;
public:
  vector<int> getSeq();
  int getFitness();
  Genome(vector<int>);
  Genome();
  ~Genome(){};
  virtual Genome* cross(Genome*);
  virtual Genome* mutation();
  virtual int compare(Genome*);
};

class Population {
private:
  vector<Genome*> pop;
public:
  Population();
  Population(vector<Genome*>);
  vector<Genome*> getPopulation();
  vector<Genome*> best(int);
  vector<Genome*> reproduction(vector<Genome*>,int);
  vector<Genome*> mutateGroup(vector<Genome*>);
  vector<Genome*> generation(int);
};

//surcharge de l'opérateur cout
ostream& operator<<(ostream&, const Population);
ostream& operator<<(ostream&, const vector<Genome*>);
