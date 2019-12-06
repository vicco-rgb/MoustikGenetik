//
//  main.cpp
//  ds2
//
//  Created by Chloe1 Leric on 11/5/19.
//  Copyright © 2019 Chloe1 Leric. All rights reserved.
//

#include <iostream>
#include <cmath>
#include <ctime>
#include <vector>
#include "genome.hpp"
using namespace std;

int main() {
    srand(time(NULL));
    vector<int> seq1{0,2,5,2,1};
    vector<int> seq2{3,6,8,1,4};
    vector<int> seq3{1,1,1,4,2};
    vector<int> seq4{3,4,2,9,5};
    genome *s1 = new genome(seq1);
    genome *s2 = new genome(seq2);
    genome *s3 = new genome(seq3);
    genome *s4 = new genome(seq4);
    vector<genome*> p = {s1,s2,s3,s4};
    population pop(p);
    pop.affichage();
    pop.generation(3);
    return 0;
}
