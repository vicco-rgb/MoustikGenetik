#include "genome.hpp"

Population testInit(){
  cout<<"TI1"<<endl;
  srand(time(NULL));
  vector<int> seq1 {1,20,50,60,70};
  vector<int> seq2 {3,60,80,10,40};
  vector<int> seq3 {1,10,100,40,20};
  vector<int> seq4 {30,40,20,90,500};
  cout<<"TI2"<<endl;

  Genome *s1 = new Genome(seq1);
  Genome *s2 = new Genome(seq2);
  Genome *s3 = new Genome(seq3);
  Genome *s4 = new Genome(seq4);

  vector<Genome*> listGenomes;
  listGenomes.push_back(s1);
  listGenomes.push_back(s2);
  listGenomes.push_back(s3);
  listGenomes.push_back(s4);
  cout<<"TI3"<<endl;
  Population pop(listGenomes,0);
  cout<<"TI4"<<endl;
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
  //retourne une liste d'entiers strictement positifs
  return seq;
}
vector<int> Genome::getAbsoluteSeq(){
  //retourne une suite de dates strictement croissantes
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
Genome* Genome::cross(Genome* genome){
  //cette fonction produit un enfant génome à partir de deux parents
  Genome *fils = new Genome();
  int prop = rand()%seq.size(); //prop in [0 taillegenome]
  cout<<"CR1"<<endl;
  for (int i=0;i<prop;i++){
    //on recopie les prop premières dates du père
    fils->seq[i]=seq[i];
  }
  cout<<"CR2"<<endl;
  for (int j=prop;j<seq.size();j++){
    //on recopie les seqSize-prop dernières dates de la mère
    fils->seq[j]=genome->seq[j];
  }
  cout<<"CR3"<<endl;
  fils->fitness=0;
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
bool Genome::betterThan(Genome* genome){
  return (fitness > genome->fitness);
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
void Population::addGenome(Genome* nGenome){
  genomes.push_back(nGenome);
}
int Population::getGeneration(){
  return generation;
}
//reproduction
Population Population::bests(int n){
  int best=0;
  //on trie toute la liste des genomes selon leur fitness
  cout<<"BE1"<<endl;
  for(int i=0;i<genomes.size()-1;i++){
    cout<<"BE"<<i<<endl;
    Genome *tmp = new Genome();
    best=i;
    for(int j=i+1;j<genomes.size();j++){
      cout<<"BE"<<i<<"-"<<j<<endl;
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
Population Population::reproduction(Population pop){
  int size=pop.getGenomes().size();
  cout<<"RE1"<<endl;
  vector<Genome*> childGenomes = pop.getGenomes();
  cout<<"RE2"<<endl;
  for (int i=0;i<size;i++){
    //on cherche un papa et une maman pour l'enfant.
    cout<<"REi - "<<i<<endl;
    int dad=rand()%size;
    int mom=rand()%size;
    while (dad==mom){ //first et second doivent être différents.
      cout<<"REequal"<<endl;
      mom=rand()%size;
    }
    childGenomes[i]=pop.getGenomes()[dad]->cross(pop.getGenomes()[mom]);
  }
  cout<<"RE3"<<endl;
  return Population(childGenomes, pop.getGeneration());
}
Population Population::mutateGroup(Population pop){
  vector<Genome*> gensMutated = pop.getGenomes();
  for (int i=0; i<pop.getGenomes().size();i++){
    gensMutated[i]=pop.getGenomes()[i]->mutation();
  }
  return Population(gensMutated, pop.getGeneration());
}
Population Population::getChildren(int n){
  //cette fonction renvoie une population correspondant aux enfants issus des nmeilleurs parents de la population manipulée.
  //La population manipulée n'est pas écrasée
  cout<<"GC1"<<endl;
  Population bestPop = bests(n); //les trouve les n meilleurs parents
  cout<<"GC2"<<endl;
  Population newGeneration = reproduction(bestPop); //on récupère les n enfants
  cout<<"GC3"<<endl;

  while (newGeneration.getGenomes().size()<genomes.size()){
    //on recrée une population de la taille originale en complétant parmis les meilleurs parents.
    newGeneration.addGenome(bestPop.getGenomes()[rand()%n]);
  }
  cout<<"GC4"<<endl;
  newGeneration = mutateGroup(newGeneration); // on les fait muter
  cout<<"GC5"<<endl;
  return newGeneration;
}

ostream& operator<<(ostream& os, Population pop){
  for (int i=0;i<pop.getGenomes().size();i++){
    //on affiche tous lse génomes les uns après les autres.
    cout<<"##### GENOME "<<i<<" #####"<<endl<<pop.getGenomes()[i]<<endl;
  }
  cout<<endl;
  return os;
}
ostream& operator<<(ostream& os, Genome *genome){
  int date=0;
  for (int i=0;i<genome->getAbsoluteSeq().size();i++){
    //on affiche les instants les uns à la suite des autres
    date=genome->getAbsoluteSeq()[i];
    cout<<date<<endl;
  }
  cout<<endl;
  return os;
}
