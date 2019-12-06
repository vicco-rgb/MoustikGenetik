#pragma once
#include <stdio.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <iostream>
using namespace std;

class genome {
private:
    int fitness;
    vector<int> seq;
    float totMutation;
public:
    vector<int> getSeq();
    int getFitness();
    genome(vector<int>);
    genome();
    ~genome(){};
    virtual genome* cross(genome*);
    virtual genome* mutation();
    virtual int compare(genome*);
};

class population {
private:
    vector<genome*> pop;
public:
    population(vector<genome*>);
    vector<genome*> best(int);
    vector<genome*> reproduction(vector<genome*>,int);
    vector<genome*> mutateGroup(vector<genome*>);
    vector<genome*> generation(int);
    void affichage();
};

void affichageVec(vector<genome*> pop);
