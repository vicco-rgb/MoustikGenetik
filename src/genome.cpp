#include "genome.hpp"

Population::Population(){}
Population::Population(vector<Genome*> pop){
  this->pop=pop;
}
vector<Genome*> Population::getPopulation(){
  return pop;
}
vector<Genome*> Population::best(int n){
  int best=0;
  for(int i=0;i<pop.size()-1;i++){
    Genome *tmp = new Genome();
    best=i;
    for(int j=i+1;j<pop.size();j++){
      if ((pop[j]->compare(pop[best]))>0){
        best=j;
      }
      if (best!=i){
        *tmp=*pop[i];
        *pop[i]=*pop[best];
        *pop[best]=*tmp;
      }
    }
  }
  vector<Genome*> popBest;
  for (vector<Genome*>::iterator it = pop.begin(); *it != pop[n]; ++it){
    popBest.push_back(*it);
  }
  return popBest;
}
vector<Genome*> Population::reproduction(vector<Genome*> pop, int n){
  vector<Genome*> popCrossed = pop;
  for (int i=0;i<n;i++){
    int first=rand()%n;
    int sec=rand()%n;
    while (sec==first){
      sec=rand()%n;
    }
    popCrossed[i]=pop[first]->cross(pop[sec]);
  }
  return popCrossed;
}
vector<Genome*> Population::mutateGroup(vector<Genome*> pop){
  vector<Genome*> popMutated = pop;
  for (int i=0; i<pop.size();i++){
    popMutated[i]=pop[i]->mutation();
  }
  return popMutated;
}
vector<Genome*> Population::generation(int m){
  Population newGeneration;
  Populaition bestPop;
  bestPop=best(m);
  newGeneration=reproduction(bestPop,m);
  newGeneration.insert(newGeneration.end(),bestPop.begin(), bestPop.end());
  newGeneration=mutateGroup(newGeneration);
  cout<<newGeneration;
  return newGeneration;
}

ostream& operator<<(ostream& os, const Population pop){
  for (int i=0;i<pop.size();i++){
    //on affiche tous lse génomes les uns après les autres.
    cout<<pop.getPopulation()[i];
  }
  cout<<endl;
}
ostream& operator<<(ostream& os, const vector<Genome*> pop){
  cout<<"{";
  for (int j=0; j<pop[i]->getSeq().size();j++){
    cout<<pop[i]->getSeq()[j]<< ",";
  }
  cout<<"} "<<pop[i]->getFitness()<<endl;
  cout<<endl;
  return os;
}

Genome::Genome(vector<int> seq) : totMutation(0.3) {
  this->seq=seq;
  fitness=0;
  for (int i=0;i<seq.size();i++){
    fitness+=seq[i];
  }
}
Genome::Genome() : totMutation(0.3) {
  seq={0,0,0,0,0};
  fitness=0;
  for (int i=0;i<seq.size();i++){
    fitness+=seq[i];
  }
}
vector<int> Genome::getSeq(){
  return seq;
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
  if (n<totMutation*100){
    seq[rand()%seq.size()]=rand()%10;
  }
  mutated->seq=seq;
  mutated->fitness=0;
  for (int i=0;i<mutated->seq.size();i++){
    mutated->fitness+=mutated->seq[i];
  }
  return mutated;
}
int Genome::compare(Genome* s1){
  int diff = fitness-s1->fitness;
  return diff;
}
