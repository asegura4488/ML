#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "THStack.h"
#include "TLegend.h"
#include "TTreePlayer.h"
#include "TStyle.h"
#include "TGaxis.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TEfficiency.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <iostream>

#include <vector>
#include "math.h"

#include <TMultiGraph.h>

using namespace std;

vector<string> input; 
vector<string> Histo;
vector<string> Legends_;

void fill(){
  
  input.push_back("knn");
  input.push_back("BDT");
  input.push_back("BDTG");
  input.push_back("RandomF");

  Histo.push_back("Predicted");
  Histo.push_back("Real");     
  
  Legends_.push_back("KNN algorithm");
  Legends_.push_back("BDT algorithm");
  Legends_.push_back("BDTG algorithm");
  Legends_.push_back("RandomF algorithm"); 
  
}


void plot_efficiency(){
  
  fill();
  
  ifstream *IFILES;
  IFILES = new ifstream[input.size()];
  
  for(int i = 0; i < input.size(); i++){
    IFILES[i].open((input[i]+".txt").c_str());
    if(!IFILES[i]) {
      std::cout << "could not open: " << input[i]+".txt" <<std::endl;
      exit(1);
    }
  }
  
  
  float X=0;
  float Y=0;
  std::pair<float, float> temporal;
  std::vector<std::pair<float, float>> vector_method;
  std::map<int, std::vector<std::pair<float, float>>> MAP;
  
  for(int i = 0; i < input.size(); i++){
    
    vector_method.clear();
    while(!IFILES[i].eof()){
      IFILES[i] >> X;
      IFILES[i] >> Y;
      temporal = std::make_pair(X,Y);
      vector_method.push_back(temporal);
    }
    MAP[i] = vector_method;   
  }
  
  
  TH1D* histo_[4][2];
  for(int i = 0; i < input.size(); i++){
    for(int j = 0; j < Histo.size(); j++){
      TH1D* h = new TH1D( (input[i]+Histo[j]).c_str(), (input[i]+"_"+Histo[j]).c_str(), 30, 0.,30. );
      histo_[i][j] = h;
    }
  }  
  
  
  typedef  std::vector<std::pair<float, float>>::iterator iterVector;
  
  double pt_array[2] = {0.}; // Zero for predicted and 1 for real;
  double pt_cut = 3.;
  bool pass= false;

  for(int i = 0; i < input.size(); i++){
    //i in the map is the file method
    for( iterVector iter = MAP[i].begin(); iter != MAP[i].end(); iter++){
      
      pt_array[0] = 1.0/(*iter).first;
      pt_array[1] = 1.0/(*iter).second;
      pass = false;      
       if(abs(pt_array[0]-pt_array[1]) < pt_cut) pass = 1; 

         for(int k = 0; k < Histo.size(); k++){
           if( k==0 && !pass ) continue;  
	   else histo_[i][k]->Fill(pt_array[k]);  
         }
    }
  }
  
  
  
  TCanvas *c1 = new TCanvas("c1", "c1",245,158,756,727);
  c1->Range(-20.38653,-0.005764521,153.3666,0.04198848);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetLeftMargin(0.1173305);
  c1->SetRightMargin(0.01937567);
  c1->SetTopMargin(0.01937407);
  c1->SetBottomMargin(0.1207154);
  c1->SetFrameBorderMode(0);
  c1->SetFrameBorderMode(0);
  
  TLegend *leg = new TLegend(0.5530504,0.7347826,0.9708223,0.9695652,NULL,"brNDC");
  leg->SetBorderSize(1);
  leg->SetTextFont(62);
  leg->SetTextSize(0.04);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  
  std::map<int, string> legends;
  for(int k = 0; k < Legends_.size(); k++) {
    legends[k] = Legends_[k]; 
  }
  
 //  histo_[0][1]->Draw();
 //  histo_[0][0]->Draw("same");  

  for(int i = 0; i < input.size(); i++){
    for(int k = 0; k < Histo.size(); k++) {
      histo_[i][k]->SetStats(kFALSE);
      histo_[i][k]->Sumw2();
      histo_[i][k]->SetTitle("");
      histo_[i][k]->SetLineColor(i+1);
      histo_[i][k]->SetLineWidth(3);
      
      histo_[i][k]->GetXaxis()->SetTitle("");
      histo_[i][k]->GetXaxis()->SetRangeUser(0., 30.);
      histo_[i][k]->GetXaxis()->SetTitleSize(0.05);
      histo_[i][k]->GetXaxis()->SetTitleOffset(1.0);
      
      histo_[i][k]->GetYaxis()->SetTitle("Regression Efficiency");
      histo_[i][k]->GetYaxis()->SetRangeUser(0., 1.0);
      histo_[i][k]->GetYaxis()->SetTitleSize(0.05);
      histo_[i][k]->GetYaxis()->SetTitleOffset(1.0);
      
      
      if(k==0){ 
	//  histo_[i][k]->GetXaxis()->SetTitle("1/BDT_{p_{T}}-1/Real_{p_{T}}");
	histo_[i][k]->GetXaxis()->SetTitle("p_{T}(GeV)"); 
	histo_[i][k]->Divide(histo_[i][k+1]);
  
      if(i==0) histo_[i][k]->Draw();
      else histo_[i][k]->Draw("same");

      
      }
      //else histo_[i][k]->Draw("same");   
      //cout << i << " " << k << endl;
      //  std::cout << "Histogram: " << histo_[i][k] << " MEAN: " << histo_[i][k]->GetMean() << " RMS: " <<  histo_[i][k]->GetRMS() << endl;
      if(k==0) leg->AddEntry(histo_[i][k], legends[i].c_str());
    }  
  }
  //TGraphAsymmErrors *effi = new TGraphAsymmErrors( HISTO[0], HISTO[1], "b(1,1) mode" );
  //TMultiGraph *MG = new TMultiGraph();
  //MG->Add(effi);
  //MG->Draw("AP"); 
  
  leg->Draw();
  
  
}
