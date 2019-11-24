# include <iostream>
# include <cmath>
# include <ctime>
# include <cstring>
# include <fstream>
//# include "fonctions.h"

using namespace std;

const int popSize = 8;
const float prob = 0.7;
const int nVar = 8;
const int selec = round(popSize*prob);

struct chromosome {
  float gene[nVar];
  float fitness;
};

struct chromosome population[popSize];
struct chromosome newpopulation[2*selec];

void initPop(string filename);
void evaluate();
void elitist();
void coupleCroisement();
void croisement(int, int);

void initPop(string filename) {

  fstream fichier;

  fichier.open("population.txt",ios::in) ;
    for (int i = 0; i<popSize; i++){
      for (int j = 0; j<nVar; j++){
        fichier >> population[i].gene[j]; // lecture de séparateur à séparateur
        cout << population[i].gene[j]<< " ";
      }
      population[i].fitness=0;
      cout << endl;
    }
  fichier.close();
}
void evaluate() {
  for (int i = 0; i<popSize; i++) {
    for (int j=0; j<nVar; j++) {
      population[i].fitness += population[i].gene[j];
    }
    cout << population[i].fitness << endl;
  }
  cout<< endl;
}
void elitist() {
  int max;
  chromosome temp;
  for (int i =0; i<popSize-1; i++) {
    max = i;
    for (int j =i+1; j<popSize; j++){
      if (population[j].fitness>population[max].fitness) {
        max = j;
      }
    }
    if (max != i) {
      temp.fitness = population[i].fitness;
      for (int k=0; k<nVar; k++){
        temp.gene[k]=population[i].gene[k];
      }
      population[i]=population[max];
      population[max]=temp;
    }
  }
  for (int l=round(popSize*prob)-1; l<popSize; l++){
    population[l].fitness=0;
    for (int m=0; m<nVar;m++){
      population[l].gene[m]=0;
    }
  }
}
void coupleCroisement(){
  int partner=0;
  for (int i=0; i<selec;i++){
    if (population[i].fitness!=0){
      while (partner=i & population[partner].fitness==0){
        partner=rand()%selec-1;
      }
      newpopulation[i].fitness=population[i].fitness;
      newpopulation[partner].fitness=population[partner].fitness;
      population[i].fitness=0;
      population[partner].fitness=0;
      for (int j=0; j<nVar; j++){
        newpopulation[i].gene[j]=population[i].gene[j];
        newpopulation[partner].gene[j]=population[partner].gene[j];
        population[i].gene[j]=0;
        population[partner].gene[j]=0;
      }
      croisement(i,partner);
    }
  }
}
void croisement(int a, int b){
  int enjambement = 4;
  for (int i=0;i<enjambement-1;i++){
    newpopulation[a+selec].gene[i]=newpopulation[a].gene[i];
    newpopulation[b+selec].gene[i]=newpopulation[b].gene[i];
  }
  for(int j=enjambement;j<nVar;j++){
    newpopulation[a+selec].gene[j]=newpopulation[b].gene[j];
    newpopulation[b+selec].gene[j]=newpopulation[a].gene[j];
  }

}






int main() {
  initPop("population.txt");
  evaluate();
  elitist();
  coupleCroisement();
  for (int k =0;k<2*selec;k++){
    for (int m=0;m<nVar;m++){
      cout<<newpopulation[k].gene[m]<<" ";
    }
    cout<<endl;
  }
  return 0;
}
