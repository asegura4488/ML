#ifndef POPULATION_H
#define POPULATION_H

#include "TROOT.h"
#include "TRint.h"

#include <TCanvas.h>
#include <TH1.h>
#include <TF1.h>
#include <TGraph.h>
#include <TLegend.h> 
#include <TTimer.h>
#include <TControlBar.h>
#include <TMath.h> 
#include <TRandom3.h>

#include <iostream>
#include <vector>
#include <chrono>

class Population{
  
 public:
  Population();
  Population(Int_t po_size_, Int_t ge_size_);
  ~Population();
  //Methods
  void Init(Int_t Min, Int_t Max); 
  void Print();
  Int_t Print_Size();
  void Clear();
  void Print_Individual(Int_t index);
  //Method for genetic
  
  std::vector<Double_t> Get_Individual(Int_t index); 
  void Set_Individual(Int_t index, std::vector<Double_t> single_);
  Double_t Get_Score(std::vector<Double_t> single_);
  std::vector<Double_t> Get_Total_Score();
  void Get_Crossing(Int_t index_1, Int_t index_2, std::vector<Double_t>& daughter_);
  void Get_Mutation(std::vector<Double_t>& single_, Int_t Min, Int_t Max);
  
  
  //Variables
  Int_t po_size;
  Int_t ge_size;
  
 private:
  
  std::vector<std::vector<Double_t> > population;
  TRandom *rand;
  
  
};


#endif
