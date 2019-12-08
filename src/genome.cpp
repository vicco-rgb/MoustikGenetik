#include "genome.hpp"
#include "moustik.hpp"

/*
SURCHARGES
*/

ostream& operator<<(ostream& os, Population pop){
  for (int i=0;i<pop.getMoustiks().size();i++){
    //on affiche tous les génomes les uns après les autres.
    cout<<"##### GENOME "<<i<<" #####"<<endl<<pop.getMoustiks()[i]->getGenome()<<endl;
  }
  cout<<endl;
  return os;
}
ostream& operator<<(ostream& os, Genome *genome){
  cout<<genome->getAbsoluteSeq()<<endl;
  return os;
}
ostream& operator<<(ostream& os, vector<int> seq){
  for (int i=0;i<seq.size();i++){
    //on affiche les instants les uns à la suite des autres
    cout<<seq[i]<<"\t";
  }
  return os;
}

/*
GENOME
*/

Genome::Genome(vector<int> seq) {
  tauxMutation=0.3;
  this->seq=seq;
  fitness=-1;
}
Genome::Genome(int tailleSeq) {
  //cette fonction fabrique un génome[tailleSeq] aléatoirement
  for (int i=0; i++; i<tailleSeq){
    //nombres aléatoires entre 0 et 20;
    seq.push_back(rand()%20);
  }
  fitness=-1;
  tauxMutation=0.3;
}
Genome::Genome() {
  tauxMutation=0.3;
  seq={1};
  fitness=-1;
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
void Genome::addAbsoluteDate(int nFrame){
  seq.push_back(nFrame-seq.back());
}
Genome* Genome::crossSplit(Genome* genome){
  //cette fonction produit un enfant génome à partir de deux parents en prenant la première partie du père et la dernière partie de la mère.
  Genome *fils = new Genome();
  int prop = rand()%seq.size(); //prop in [0 taillegenome]
  for (int i=0;i<prop;i++){
    //on recopie les prop premières dates du père
    fils->seq[i]=seq[i];
  }
  for (int j=prop;j<seq.size();j++){
    //on recopie les seqSize-prop dernières dates de la mère
    fils->seq[j]=genome->seq[j];
  }
  fils->fitness=-1;
  return fils;
}
Genome* Genome::crossAvg(Genome* genome){
  //cette fonction produit un enfant génome à partir de deux parents en faisant la moyenne de chaque paire de dates. Cette moyenne est pondérée par le ratio des fitness.
  Genome *fils = new Genome();
  vector<int> bigSeq;
  vector<int> litSeq;
  float ratio; // fitnessBigSeq/fitnessLitSeq
  if (seq.size()>=genome->getRelativeSeq().size()){
    bigSeq = seq;
    litSeq = genome->getRelativeSeq();
    ratio = fitness/genome->getFitness();
  } else {
    bigSeq = genome->getRelativeSeq();
    litSeq = seq;
    ratio = genome->getFitness()/fitness;
  }
  for (int i=0; i<litSeq.size(); i++){
    //moyenne pondérée des dates par le ratio des fitness
    fils->getRelativeSeq().push_back(bigSeq[i]*ratio + (1-ratio)*litSeq[i]);
  }
  for (int j=litSeq.size(); j<bigSeq.size();j++){
    //on complète sans moyenner avec la séquence qui est plus grande
    fils->getRelativeSeq().push_back(seq[j]);
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
  mutated->fitness=-1;
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
Population::Population(vector<MoustikIA*> nMoustiks, int gen){
  moustiks=nMoustiks;
  for (int i=0; i<nMoustiks.size();i++){
    moustiks[i]->setID("0/"+to_string(i));
  }
  generation=gen;
}
Population::Population(Population *pop){
  moustiks = pop->getMoustiks();
  generation = pop->getGeneration();
}
Population::~Population(){}
//getters
vector<MoustikIA*> Population::getMoustiks(){
  return moustiks;
}
void Population::addMoustik(MoustikIA* moustik){
  moustik->setID(to_string(generation)+"/"+to_string(moustiks.size()));
  moustiks.push_back(moustik);
}
int Population::getGeneration(){
  return generation;
}
//reproduction
Population Population::bests(int n){
  int best=0;
  //on trie les moustiks selon leur fitness
  for(int i=0;i<moustiks.size()-1;i++){
    MoustikIA* tmp;
    best=i;
    for(int j=i+1;j<moustiks.size();j++){
      if (moustiks[j]->getGenome()->betterThan(moustiks[best]->getGenome())){
        best=j;
      }
      if (best!=i){
        //alors on intervertit le moustik[i] et le moustik[best]
        tmp=moustiks[i];
        moustiks[i]=moustiks[best];
        moustiks[best]=tmp;
      }
    }
  }
  //on récupère les n premiers
  vector<MoustikIA*> bestMoustiks;
  int i=0;
  while (bestMoustiks.size()<n){
    bestMoustiks.push_back(moustiks[i]);
  }
  return Population(bestMoustiks, generation);
}
Population Population::reproduction(Population pop){
  int size=pop.getMoustiks().size();
  vector<MoustikIA*> children = pop.getMoustiks();
  for (int i=0;i<size;i++){
    //on cherche un papa et une maman pour l'enfant.
    int dad=rand()%size;
    int mom=rand()%size;
    while (dad==mom){
      //first et second doivent être différents.
      mom=rand()%size;
    }
    children[i]->setGenome(pop.getMoustiks()[dad]->getGenome()->crossAvg(pop.getMoustiks()[mom]->getGenome()));
  }
  return Population(children, pop.getGeneration()+1);
}
Population Population::mutateGroup(Population pop){
  vector<MoustikIA*> mutatedMoustiks = pop.getMoustiks();
  for (int i=0; i<pop.getMoustiks().size();i++){
    pop.getMoustiks()[i]->setGenome(pop.getMoustiks()[i]->getGenome()->mutation());
    mutatedMoustiks[i]=pop.getMoustiks()[i];
  }
  return Population(mutatedMoustiks, pop.getGeneration());
}
Population Population::getChildren(int n){
  //cette fonction renvoie une population correspondant aux enfants issus des nmeilleurs parents de la population manipulée.
  //La population manipulée n'est pas écrasée
  if (n>moustiks.size()){ //pour ne pas faire planter l'algorithme
    n=moustiks.size();
  }
  Population bestPop = bests(n); //les trouve les n meilleurs parents
  Population newGeneration = reproduction(bestPop); //on récupère les n enfants (incrémente generation)
  while (newGeneration.getMoustiks().size()<moustiks.size()){
    //on recrée une population de la taille originale en complétant avec les meilleurs parents.
    newGeneration.addMoustik(bestPop.getMoustiks()[rand()%n]);
  }
  newGeneration = mutateGroup(newGeneration); // on les fait muter
  return newGeneration;
}
