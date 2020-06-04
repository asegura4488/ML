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



void MACRO_TriggerEff(){
  
  cout << "working" << endl;
  
 // setTDRStyle();
  
  TCanvas* c1 = new TCanvas("c1","c1",200,200,800,700);
  c1->SetGrid();
  
  TFile *f1 = TFile::Open("output_testbdt.root","READ");
  if (f1 == 0) {
    printf("Error: cannot open Trig Eff root file \n");
    return;
  }
  
  
  TH1F *Numerator = (TH1F*)f1->Get("h3");
  TH1F *Denominator = (TH1F*)f1->Get("h4");
  /*
    TH1F *Numerator = (TH1F*)f1->Get("NRecoTriggers2/FirstLeadingJetPt");
    TH1F *Denominator = (TH1F*)f1->Get("METCut/FirstLeadingJetPt");
  */
  
  Int_t nbins   = Denominator->GetXaxis()->GetNbins();
  //Int_t nbins  = 20;
  
  cout<<"Numerator "<< Numerator->Integral()<<"   Denominator "<< Denominator->Integral()<<endl;
  cout<<"Numerator "<< Numerator->GetXaxis()->GetNbins()<<"   Denominator "<< Denominator->GetXaxis()->GetNbins() <<endl;
  
  int nb = 22;
  
  float BINSJET[35] = {10.0, 20.0,  30.0, 40.0, 50.0, 60.0, 70.0, 80.5, 90.0, 100.0, 
		       150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0,
		       600.0, 700.0, 800.0, 900.0, 1000.0};  
  
  
  float BINS[48] = {50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0, 120.0, 130.0, 140.0, 150.0, 
                    160.0, 170.0, 180.0, 190.0, 200.0, 210.0, 220.0, 230.0, 240.0, 250.0,
                    260.0, 270.0, 280.0, 290.0, 300.0, 310.0, 320.0, 330.0, 340.0, 350.0,
                    360.0, 370.0, 380.0, 390.0, 400.0, 410.0, 420.0, 430.0, 440.0, 450.0,
                    460.0, 470.0, 480.0, 490.0, 500.0};
  
  
  TH1F *H_bins_num = new TH1F("h_NUM", "h_NUM", 200, 0,30);
  TH1F *H_bins_den = new TH1F("h_DEN", "h_DEN", 200, 0,30);
  /*
  
  
    TH1F *H_bins_num = new TH1F("h_NUM", "h_NUM", nb, BINSJET);
    TH1F *H_bins_den = new TH1F("h_DEN", "h_DEN", nb, BINSJET);
    
  */
  for (int b = 1; b <= H_bins_den->GetXaxis()->GetNbins(); b++ ){
    int num_bin = 0;
    int den_bin = 0;
    
    for (int i = 1; i < nbins; i++ ){
      
      if(((( Denominator->GetXaxis()->GetBinLowEdge(i)) + (Denominator->GetXaxis()->GetBinWidth(i))) <= ((H_bins_den->GetXaxis()->GetBinLowEdge(b))+ (H_bins_den->GetXaxis()->GetBinWidth(b)))) &&
	 ((( Denominator->GetXaxis()->GetBinLowEdge(i))+ (Denominator->GetXaxis()->GetBinWidth(i))) > ((H_bins_den->GetXaxis()->GetBinLowEdge(b))))){
	
	num_bin += Numerator->GetBinContent(i);
	den_bin += Denominator->GetBinContent(i);
	
      }else{
	continue;
      }
  ///    cout << num_bin << endl;
      H_bins_num->SetBinContent(b, num_bin);
      H_bins_den->SetBinContent(b, den_bin);
      
    }
    
  }
  
  for(int i = 0; i < nbins; i++){
    if(Denominator->GetXaxis()->GetNbins() >= i ){
      cout << "  Bin: " << i <<  " NumBinnned: "<< H_bins_num->GetBinContent(i) <<"  DenBinned: "<< H_bins_den->GetBinContent(i)<<  " Num: "<<Numerator->GetBinContent(i) <<"  Den: "<<Denominator->GetBinContent(i)<<endl;
    }else{
      cout << "  Bin: " << i <<  " NumBinnned: "<< " --- " <<"  DenBinned: "<< " --- " <<  " Num: "<<Numerator->GetBinContent(i) <<"  Den: "<<Denominator->GetBinContent(i)<<endl;
    }
  }
  
//  H_bins_num->Divide(H_bins_den);
//  H_bins_num->SetLineColor(1); 
//  H_bins_num->Draw();
  
    H_bins_num->Draw();
    H_bins_den->Draw("same");

//   TGraphAsymmErrors* gr1 = new TGraphAsymmErrors( H_bins_num, H_bins_den, "b(1,1) mode" );
 
//  gr1->Draw("AP");
/*
  TMultiGraph *mg = new TMultiGraph();
  
  mg->Add(gr1);
  */


//  TF1* gr_fit = fitGamma(gr1, 0.5, 0.5);
  /*
  gr1->SetMarkerColor(1);
  gr1->SetMarkerStyle(20);
  gr1->SetMarkerSize(1.3);
  gr1->Draw("AEP");
*/
 // gr_fit->Draw("same");
  
  //mg->SetTitle("Trigger Efficiency HLT_PFMETNoMu120_PFMHTNoMu120_IDTight");
  // mg->GetXaxis()->SetTitle("E^{miss}_{T} [GeV]");
  //mg->GetXaxis()->SetTitle("p_{T}(leading \ jet) [GeV]");
 /*
  mg->GetXaxis()->SetLabelFont(42);
  mg->GetXaxis()->SetLabelSize(0.035);
  mg->GetXaxis()->SetTitleSize(0.05);
  mg->GetXaxis()->SetTitleOffset(0.89);
  mg->GetXaxis()->SetTitleFont(42);
  mg->GetYaxis()->SetTitle("#epsilon");
  mg->GetYaxis()->SetLabelFont(42);
  mg->GetYaxis()->SetLabelSize(0.035);
  mg->GetYaxis()->SetTitleSize(0.06);
  mg->GetYaxis()->SetTitleOffset(0.69);
  mg->GetYaxis()->SetTitleFont(42);
  */
/*
   mg->Draw("AP");
*/
}



