#include "Population.h"

Population::Population(){
}

Population::Population(Int_t po_size_, Int_t ge_size_): 
  po_size(po_size_), 
  ge_size(ge_size_)
{
  population.clear();
}

void Population::Init(Int_t Min, Int_t Max){
  Double_t seed;
  rand = new TRandom3(); 
  for(int i = 0; i < po_size; i++){
    vector<Double_t> single; // individuo 
    for(int j = 0; j < ge_size; j++){
      seed = rand->Uniform(Min,Max);
      single.push_back(seed);
    } 
    population.push_back(single);  
  }
  std::cout << "-------  Population Generated. " << std::endl;  
  std::cout << "-------  Individuals: " <<  population.size() << std::endl;  
}

void Population::Print(){
  if(population.size() == 0){
    std::cout << " Empty population " << std::endl; 
  }
  else{
    
    for(std::vector<std::vector<Double_t> >::iterator it= population.begin() ; it != population.end(); it++){
      for(int j = 0; j < ge_size; j++){
	cout << (*it)[j] << " " ;
      }
      cout << endl;  
    }
  } 
}

Int_t Population::Print_Size(){
Int_t size = population.size();
return size;
}


void Population::Print_Individual(Int_t index){
  if(population.size() == 0){
    std::cout << " Empty population " << std::endl; 
  }
  else{
    for(int i = 0; i < ge_size; i++) cout << population[index][i] << " ";  
    cout << endl;
  }  
}


void Population::Clear(){
  population.clear();
}




Double_t Population::Get_Score(std::vector<Double_t> single_){
  Double_t score = -1.;
  Double_t X = single_[0];
  Double_t Y = single_[1];
  if(single_.size() != 0) 
    score = TMath::Abs(Y-(X*X)); 
    //score = TMath::Abs(Y-(TMath::Sin(X)/X)); 
    //score = TMath::Abs(Y-(TMath::Exp(X)));  // no converge demasiado bien interesante
  return score;
}

std::vector<Double_t> Population::Get_Total_Score(){
      
 std::vector<Double_t> single_;
 Double_t score = 0.; 
   
 if(population.size() == 0) return single_; 
   
 for(std::vector<std::vector<Double_t> >::iterator it= population.begin() ; it != population.end(); it++){
 score = Population::Get_Score((*it));
 single_.push_back(score);  
 } 

 return single_;
}

void Population::Get_Crossing(Int_t index_1, Int_t index_2, std::vector<Double_t>& daughter_){
  
  daughter_.push_back(population[index_1][0]);
  daughter_.push_back(population[index_2][1]);
  daughter_.push_back(population[index_1][2]);
  
}

void Population::Get_Mutation(std::vector<Double_t>& single_, Int_t Min, Int_t Max){
  Double_t seed = rand->Uniform(0.,1.);
  if(seed < 0.5) single_[0] = rand->Uniform(Min,Max);
  else single_[1] = rand->Uniform(Min,Max);
}

std::vector<Double_t> Population::Get_Individual(Int_t index){
  
  std::vector<Double_t> Individual;
  
  if(abs(index) >= population.size()){ 
    std::cout << " Individuo fuera de rango " <<  std::endl;
    return Individual;
  }
  
  else Individual = population[index];
  
  return Individual;
}

void Population::Set_Individual(Int_t index, std::vector<Double_t> single_){
  population[index] = single_;  
}

Population::~Population(){
}
