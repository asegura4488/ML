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

vector<string> in_put; 
vector<string> Histo;
vector<string> Legends_;

void fill(){
  
  in_put.push_back("output_testbdt");
  in_put.push_back("output_testbdt");
  
  
//  Histo.push_back("BDTG3");
//  Histo.push_back("BDTG4");
  Histo.push_back("KNN3");
  Histo.push_back("KNN4"); 
//  Histo.push_back("DNN_CPU3");
//  Histo.push_back("DNN_CPU4");
    

//  Legends_.push_back("BDT algorithm");
  Legends_.push_back("BDTG algorithm");
  
  
}


void plot_efficiency(){
  
  fill();
  TFile *FILES[20]; 
  
  for(int i = 0; i < in_put.size(); i++){ 
    TFile *f = TFile::Open((in_put[i]+".root").c_str(),"READ");
    FILES[i] = f;
  }
  
  TH1F* HISTO[20]; 
  for(int i = 0; i < in_put.size(); i++){  
    for(int j = 0; j < Histo.size(); j++){
      TH1F* h;   
      h = (TH1F*)FILES[i]->Get(Histo[j].c_str());  
      HISTO[j] = h;
    }
  }
 /* 
  double integral = 0;
  double norm_scale = 1.0;
  for(int j = 0; j < Histo.size(); j++){
    integral = HISTO[j]->Integral();
    if(integral !=  0) HISTO[j]->Scale(norm_scale/integral);
    else std::cout << "Empty: " << Histo[j] << std::endl; 
  }
  */
  
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
  for(int k = 0; k < Histo.size(); k++) {
    legends[k] = Legends_[k]; 
  }
  
  
  for(int k = 0; k < Histo.size(); k++) {
    HISTO[k]->SetStats(kFALSE);
    HISTO[k]->Sumw2();
    HISTO[k]->SetTitle("");
    HISTO[k]->SetLineColor(k+1);
    HISTO[k]->SetLineWidth(3);
    
    HISTO[k]->GetXaxis()->SetTitle("");
    HISTO[k]->GetXaxis()->SetRangeUser(0, 28);
    HISTO[k]->GetXaxis()->SetTitleSize(0.05);
    HISTO[k]->GetXaxis()->SetTitleOffset(1.0);
    
    HISTO[k]->GetYaxis()->SetTitle("Regression Efficiency");
   // HISTO[k]->GetYaxis()->SetRangeUser(0., 500);
    HISTO[k]->GetYaxis()->SetTitleSize(0.05);
    HISTO[k]->GetYaxis()->SetTitleOffset(1.0);
    
    
    if(k==0){ 
      //  HISTO[k]->GetXaxis()->SetTitle("1/BDT_{p_{T}}-1/Real_{p_{T}}");
      HISTO[k]->GetXaxis()->SetTitle("p_{T}(GeV)"); 
      HISTO[k]->Divide(HISTO[k+1]); 
      HISTO[k]->Draw();
      leg->AddEntry(HISTO[k], legends[k].c_str());
    }
    //else HISTO[k]->Draw("same");   
    
  //  std::cout << "Histogram: " << Histo[k] << " MEAN: " << HISTO[k]->GetMean() << " RMS: " <<  HISTO[k]->GetRMS() << endl;
     
  }  
  
     //TGraphAsymmErrors *effi = new TGraphAsymmErrors( HISTO[0], HISTO[1], "b(1,1) mode" );
     //TMultiGraph *MG = new TMultiGraph();
     //MG->Add(effi);
     //MG->Draw("AP"); 

  leg->Draw();
  
}
