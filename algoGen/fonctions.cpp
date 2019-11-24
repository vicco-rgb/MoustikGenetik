# include <iostream>
# include <cmath>
# include <ctime>
# include <cstring>
# include <fstream>
# include "fonctions.h"

using namespace std;

const int popSize = 3;
const int prob = 0.5;
const int nVar = 8;

struct chromosome {
  float gene[nVar];
  float fitness;
};

struct chromosome pop[popSize];

void initPop(string filename) {

  fstream fichier;

  fichier.open("population.txt",ios::in) ;
  for (int i; i<popSize; i++){
    for (int j; j<nVar; j++){
      fichier >> pop[i].gene[j]; // lecture de séparateur à séparateur
      cout << pop[i].gene[j];
    }
    pop[i].fitness=0;
    cout << endl;
  }
  fichier.close();
}
