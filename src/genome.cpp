#include "genome.hpp"

population::population(vector<genome*> pop){
    this->pop=pop;
}
vector<genome*> population::best(int n){
    int best=0;
    for(int i=0;i<pop.size()-1;i++){
        genome *tmp = new genome();
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
    vector<genome*> popBest;
    for (vector<genome*>::iterator it = pop.begin(); *it != pop[n]; ++it){
        popBest.push_back(*it);
    }
    return popBest;
}
vector<genome*> population::reproduction(vector<genome*> pop, int n){
    vector<genome*> popCrossed = pop;
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
vector<genome*> population::mutateGroup(vector<genome*> pop){
    vector<genome*> popMutated = pop;
    for (int i=0; i<pop.size();i++){
        popMutated[i]=pop[i]->mutation();
    }
    return popMutated;
}
vector<genome*> population::generation(int m){
    vector<genome*> newGeneration;
    vector<genome*> bestPop;
    bestPop=best(m);
    newGeneration=reproduction(bestPop,m);
    newGeneration.insert(newGeneration.end(),bestPop.begin(), bestPop.end());
    newGeneration=mutateGroup(newGeneration);
    affichageVec(newGeneration);
    return newGeneration;
}
void population::affichage(){
    for (int i=0;i<pop.size();i++){
        cout<<"{";
        for (int j=0; j<pop[i]->getSeq().size();j++){
            cout<<pop[i]->getSeq()[j]<< ",";
        }
        cout<<"}"<<pop[i]->getFitness()<<endl;
    }
    cout<<endl;
}

ostream& operator<<(ostream& os, const vector<genome*> pop){
  for (int i=0;i<pop.size();i++){
      cout<<"{";
      for (int j=0; j<pop[i]->getSeq().size();j++){
          cout<<pop[i]->getSeq()[j]<< ",";
      }
      cout<<"}"<<pop[i]->getFitness()<<endl;
  }
  cout<<endl;
  return os;
}

genome::genome(vector<int> seq) : totMutation(0.3) {
    this->seq=seq;
    fitness=0;
    for (int i=0;i<seq.size();i++){
        fitness+=seq[i];
    }
}
genome::genome() : totMutation(0.3) {
    seq={0,0,0,0,0};
    fitness=0;
    for (int i=0;i<seq.size();i++){
        fitness+=seq[i];
    }
}
vector<int> genome::getSeq(){
    return seq;
}
int genome::getFitness(){
    return fitness;
}
genome* genome::cross(genome* s2){
    genome *fils = new genome();
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
genome* genome::mutation(){
    genome *mutated = new genome();
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
int genome::compare(genome* s1){
    int diff = fitness-s1->fitness;
    return diff;
}
