#include "genome.hpp"

Population testInit(){
  srand(time(NULL));
  // vector<int> seq1{1,20,50,60,70};
  // vector<int> seq2{3,60,80,10,40};
  // vector<int> seq3{1,10,100,40,20};
  // vector<int> seq4{30,40,20,90,500};

  vector<int> seq1;
  vector<int> seq2;
  vector<int> seq3;
  vector<int> seq4;
  for (int i=0; i<5; i++){
    seq1.push_back(50);
    seq2.push_back(60);
    seq3.push_back(55);
    seq4.push_back(43);
  }
  Genome *s1 = new Genome(seq1);
  Genome *s2 = new Genome(seq2);
  Genome *s3 = new Genome(seq3);
  Genome *s4 = new Genome(seq4);

  vector<Genome*> listGenomes;
  listGenomes.push_back(s1);
  listGenomes.push_back(s2);
  listGenomes.push_back(s3);
  listGenomes.push_back(s4);

  Population pop(listGenomes,0);
  return pop;
}

/*
GENOME
*/

Genome::Genome(vector<int> seq) {
  tauxMutation=0.3;
  this->seq=seq;
  fitness=0;
}
Genome::Genome() {
  tauxMutation=0.3;
  seq={1};
  fitness=0;
  for (int i=0;i<seq.size();i++){
    fitness+=seq[i];
  }
}
vector<int> Genome::getRelativeSeq(){
  return seq;
}
vector<int> Genome::getAbsoluteSeq(){
  vector<int> absoluteSeq;
  absoluteSeq.push_back(seq[0]);
  for (int i=1; i<seq.size(); i++) {
    absoluteSeq.push_back(absoluteSeq[i-1]+seq[i]);
  }
  return absoluteSeq;
}
int Genome::getFitness(){
  return fitness;
}
Genome* Genome::cross(Genome* s2){
  Genome *fils = new Genome();
  int n = rand()%seq.size();
  for (int i=0;i<n;i++){
    fils->seq[i]=seq[i];
  }
  for (int j=n;j<seq.size();j++){
    fils->seq[j]=s2->seq[j];
  }
  fils->fitness=0;
  for (int i=0;i<fils->seq.size();i++){
    fils->fitness+=fils->seq[i];
  }
  return fils;
}
Genome* Genome::mutation(){
  Genome *mutated = new Genome();
  int n = rand()%100;
  if (n<tauxMutation*100){
    seq[rand()%seq.size()]=rand()%10;
  }
  mutated->seq=seq;
  mutated->fitness=0;
  for (int i=0;i<mutated->seq.size();i++){
    mutated->fitness+=mutated->seq[i];
  }
  return mutated;
}
bool Genome::betterThan(Genome* s1){
  return (fitness > s1->fitness);
}

/*
POPULATION
*/

//constructeurs-destructeurs
Population::Population(){
  generation=0;
}
Population::Population(vector<Genome*> genomes, int gen){
  this->genomes=genomes;
  generation=gen;
}
Population::Population(Population *pop){
  genomes = pop->getGenomes();
  generation = pop->getGeneration();
}
Population::~Population(){}
//getters
vector<Genome*> Population::getGenomes(){
  return genomes;
}
int Population::getGeneration(){
  return generation;
}
//reproduction
Population Population::bests(int n){
  int best=0;
  //on trie toute la liste des genomes selon leur fitness
  for(int i=0;i<genomes.size()-1;i++){
    Genome *tmp = new Genome();
    best=i;
    for(int j=i+1;j<genomes.size();j++){
      if (genomes[j]->betterThan(genomes[best])){
        best=j;
      }
      if (best!=i){
        *tmp=*genomes[i];
        *genomes[i]=*genomes[best];
        *genomes[best]=*tmp;
      }
    }
  }
  //on récupère les n premiers
  vector<Genome*> bestGens;
  for (vector<Genome*>::iterator it = genomes.begin(); *it != genomes[n]; ++it){
    bestGens.push_back(*it);
  }
  return Population(bestGens, generation);
}
Population Population::reproduction(Population pop, int n){
  vector<Genome*> gensCrossed = pop.getGenomes();
  for (int i=0;i<n;i++){
    int first=1+rand()%n;
    int sec=1+rand()%n;
    while (sec==first){
      sec=1+rand()%n;
    }
    gensCrossed[i]=pop.getGenomes()[first]->cross(pop.getGenomes()[sec]);
  }
  return Population(gensCrossed, pop.getGeneration());
}
Population Population::mutateGroup(Population pop){
  vector<Genome*> gensMutated = pop.getGenomes();
  for (int i=0; i<pop.getGenomes().size();i++){
    gensMutated[i]=pop.getGenomes()[i]->mutation();
  }
  return Population(gensMutated, pop.getGeneration());
}
Population Population::getChildren(int n){
  //cette fonction renvoie une population correspondant aux enfants de la population manipulée.
  //La population manipulée n'est pas écrasée
  Population bestPop = bests(n); //les n meilleurs
  Population newGeneration = reproduction(bestPop,n);
  newGeneration.getGenomes().insert(newGeneration.getGenomes().end(), bestPop.getGenomes().begin(), bestPop.getGenomes().end());
  newGeneration = mutateGroup(newGeneration);
  return newGeneration;
}

ostream& operator<<(ostream& os, Population pop){
  for (int i=0;i<pop.getGenomes().size();i++){
    //on affiche tous lse génomes les uns après les autres.
    cout<<"##### GENOME "<<i<<" #####"<<endl<<pop.getGenomes()[i];
  }
  cout<<endl;
  return os;
}
ostream& operator<<(ostream& os, Genome *genome){
  for (int i=0;i<genome->getAbsoluteSeq().size();i++){
    //on affiche les instants les uns à la suite des autres
    cout<<genome->getAbsoluteSeq()[i]<<"-";
  }
  cout<<endl;
  return os;
}
