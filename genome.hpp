//
//  genome.hpp
//  ds2
//
//  Created by Chloe1 Leric on 11/5/19.
//  Copyright © 2019 Chloe1 Leric. All rights reserved.
//

#pragma once
#include <stdio.h>
#include <vector>

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
