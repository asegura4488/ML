/// \file
/// \ingroup tutorial_tmva
/// \notebook -nodraw
/// This macro provides a simple example on how to use the trained regression MVAs
/// within an analysis module
///
///  - Project   : TMVA - a Root-integrated toolkit for multivariate data analysis
///  - Package   : TMVA
///  - Exectuable: TMVARegressionApplication
///
/// \macro_output
/// \macro_code
/// \author Andreas Hoecker

#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

using namespace TMVA;

std::vector<string> Digis_Pt;
std::vector<string> Digis;

void fill(){
  
  Digis_Pt.push_back("Pt_Muon_Matched");
  Digis_Pt.push_back("IPt_Muon");
  
  
  Digis.push_back("phi1");
  Digis.push_back("phi2");
  Digis.push_back("phi3");
  Digis.push_back("phi4");


  Digis.push_back("theta1");
  Digis.push_back("theta2");
  Digis.push_back("theta3");
  Digis.push_back("theta4");
/*
  Digis.push_back("deltaphi12");
  Digis.push_back("deltaphi23");
  Digis.push_back("deltaphi34");

  Digis.push_back("deltatheta12");
  Digis.push_back("deltatheta23");
  Digis.push_back("deltatheta34");
*/
  Digis.push_back("deltatheta14");

/*
  Digis.push_back("cluster1");
  Digis.push_back("cluster2");
  Digis.push_back("cluster3");
  Digis.push_back("cluster4");
*/
  //Digis.push_back("NMuons");
  //Digis.push_back("NRechits");
  Digis.push_back("type");
  Digis.push_back("NCPPF");
  
  
}
void TMVARegressionApplication( TString myMethodList = "" ) 
{
   fill();
    std::cout << "Set of features for saving: " << std::endl;
    std::map<std::string, int> map_branches;
     for(int i = 0; i < Digis.size(); i++){
      map_branches[Digis[i]] = i;  
       std::cout << Digis[i] << " " << i << std::endl;    
    } 
   //---------------------------------------------------------------
   // This loads the library
   TMVA::Tools::Instance();

   // Default MVA methods to be trained + tested
   std::map<std::string,int> Use;

   // --- Mutidimensional likelihood and Nearest-Neighbour methods
   Use["PDERS"]           = 0;
   Use["PDEFoam"]         = 0; 
   Use["KNN"]             = 1;
   // 
   // --- Linear Discriminant Analysis
   Use["LD"]		        = 0;
   // 
   // --- Function Discriminant analysis
   Use["FDA_GA"]          = 0;
   Use["FDA_MC"]          = 0;
   Use["FDA_MT"]          = 0;
   Use["FDA_GAMT"]        = 0;
   // 
   // --- Neural Network
   Use["MLP"] = 0;
   Use["DNN_CPU"] = 0;
   // 
   // --- Support Vector Machine 
   Use["SVM"]             = 0;
   // 
   // --- Boosted Decision Trees
   Use["BDT"]             = 1;
   Use["BDTG"]            = 1;
   // ---------------------------------------------------------------

   std::cout << std::endl;
   std::cout << "==> Start TMVARegressionApplication" << std::endl;

   // Select methods (don't look at this code - not of interest)
   if (myMethodList != "") {
      for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) it->second = 0;

      std::vector<TString> mlist = gTools().SplitString( myMethodList, ',' );
      for (UInt_t i=0; i<mlist.size(); i++) {
         std::string regMethod(mlist[i]);

         if (Use.find(regMethod) == Use.end()) {
            std::cout << "Method \"" << regMethod << "\" not known in TMVA under this name. Choose among the following:" << std::endl;
            for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) std::cout << it->first << " ";
            std::cout << std::endl;
            return;
         }
         Use[regMethod] = 1;
      }
   }

   // --------------------------------------------------------------------------------------------------

   // --- Create the Reader object

    TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" ); 
  
    Float_t *DIGIS;
    DIGIS = new Float_t(Digis.size());
    
    for(int i = 0; i < Digis.size(); i++){ 
	reader->AddVariable(Digis[i].c_str(), &DIGIS[i]);
    }

   // Spectator variables declared in the training have to be added to the reader, too
/*
   Float_t spec1,spec2;
   reader->AddSpectator( "spec1:=var1*2",  &spec1 );
   reader->AddSpectator( "spec2:=var1*3",  &spec2 );

*/

   // --- Book the MVA methods

   TString dir    = "../dataset/weights/";
   TString prefix = "TMVARegression";

   // Book method(s)
   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      if (it->second) {
         TString methodName = it->first + " method";
         TString weightfile = dir + prefix + "_" + TString(it->first) + ".weights.xml";
         reader->BookMVA( methodName, weightfile ); 
      }
   }
   
   // Book output histograms
   TH1* hists[10];
   TH1* resol[10];
   TH1* ratio[10];
   TH1* numerator[10];
   TH1* Muon_Pt[10];
    
   Int_t nhists = -1;
   Int_t nhists_1 = -1;
   Int_t nhists_2 = -1;
   Int_t nhists_3 = -1;
   Int_t nhists_4 = -1;

   for (std::map<std::string,int>::iterator it = Use.begin(); it != Use.end(); it++) {
      TH1* h = new TH1F( it->first.c_str(), TString(it->first) + " method", 100, 0., 30. );
      TH1* h1 = new TH1F( (it->first+"1").c_str(), TString(it->first) + " resolution", 20, -10, 10 );
      TH1* h2 = new TH1F( (it->first+"2").c_str(), TString(it->first) + " ratio", 100, 0.1, 5. );
      TH1* h3 = new TH1F( (it->first+"3").c_str(), TString(it->first) + " numerator", 30, 0., 30. ); 
      TH1* h4 = new TH1F( (it->first+"4").c_str(), TString(it->first) + " muon_pt", 30, 0., 30. );        
              
      if (it->second){
      hists[++nhists] = h;
      resol[++nhists_1 ] = h1;
      ratio[++nhists_2 ] = h2;
      numerator[++nhists_3 ] = h3;
      Muon_Pt[++nhists_4] = h4; 
      }
   }
   nhists++;
  

             
   // Prepare input tree (this must be replaced by your data source)
   // in this example, there is a toy tree with signal and one with background events
   // we'll later on use only the "signal" events for the test in this example.
   //
   TFile *input(0);
   TString fname = "../data/BDT_CPPF.root";

   if (!gSystem->AccessPathName( fname )) {
      input = TFile::Open( fname ); // check if file in local directory exists
   }
   else {
      TFile::SetCacheFileDir(".");
      input = TFile::Open("http://root.cern.ch/files/tmva_reg_example.root", "CACHEREAD"); // if not: download from ROOT server
   }
   if (!input) {
      std::cout << "ERROR: could not open data file" << std::endl;
      exit(1);
   }
   std::cout << "--- TMVARegressionApp        : Using input file: " << input->GetName() << std::endl;

   // --- Event loop

   // Prepare the tree
   // - here the variable names have to corresponds to your tree
   // - you can use the same variables as above which is slightly faster,
   //   but of course you can use different ones and copy the values inside the event loop
   //
  

   TTree* Tree_ = (TTree*)input->Get("BDTCPPF/bdt_cppf");
   std::cout << "--- Select signal sample" << std::endl;

   
   Float_t *test_DIGIS_Pt;
   test_DIGIS_Pt = new Float_t[Digis_Pt.size()];
   for(int i = 0; i < Digis_Pt.size(); i++){  
     Tree_->SetBranchAddress(Digis_Pt[i].c_str(), &test_DIGIS_Pt[i] );
   }

   Short_t *test_DIGIS;
   test_DIGIS = new Short_t[Digis.size()];
   for(int i = 0; i < Digis.size(); i++){  
     Tree_->SetBranchAddress(Digis[i].c_str(), &test_DIGIS[i] );
   }
   // Create a set of variables and declare them to the reader
   // - the variable names MUST corresponds in name and type to those given in the weight file(s) used
  

   std::cout << "--- Processing: " << Tree_->GetEntries() << " events" << std::endl;
   TStopwatch sw;
   sw.Start();


   Double_t cut_trigger = 2;
  //for (Long64_t ievt=0; ievt<20000;ievt++) {
   for (Long64_t ievt=0; ievt<Tree_->GetEntries();ievt++) {

      if (ievt%1000 == 0) {	
         std::cout << "--- ... Processing event: " << ievt << std::endl;
      }
      Tree_->GetEntry(ievt);
      for(int i = 0; i < Digis.size(); i++)  DIGIS[i] = float(test_DIGIS[i]);
       
      if(DIGIS[map_branches["NCPPF"]] < 2) continue;
       
      //std::cout << DIGIS[map_branches["NCPPF"]] << std::endl;
      for (Int_t ih=0; ih<nhists; ih++) {
         TString title = hists[ih]->GetTitle();
        
          Float_t val = (reader->EvaluateRegression( title ))[0]; 
      
            
          hists[ih]->Fill( val );                
          resol[ih]->Fill( val - test_DIGIS_Pt[0] );
          ratio[ih]->Fill( val / test_DIGIS_Pt[0] );

          //if((test_DIGIS_Pt[0] < 15.) || (test_DIGIS_Pt[0] > 20.)) continue;

          if(abs(val - test_DIGIS_Pt[0]) < cut_trigger)  numerator[ih]->Fill(val);
          Muon_Pt[ih]->Fill(test_DIGIS_Pt[0]);   
       
      }
 
   }
   sw.Stop();
   std::cout << "--- End of event loop: "; sw.Print();

   // --- Write histograms

//   h3->Divide(h4);

   TFile *target  = new TFile( "output_testbdt.root","RECREATE" );
   for (Int_t ih=0; ih<nhists; ih++){
	 hists[ih]->Write();
         resol[ih]->Write();
         ratio[ih]->Write();
         numerator[ih]->Write();
         Muon_Pt[ih]->Write(); 
   }

   target->Close();

   std::cout << "--- Created root file: \"" << target->GetName() 
             << "\" containing the MVA output histograms" << std::endl;
  
   delete reader;
    
   std::cout << "==> TMVARegressionApplication is done!" << std::endl << std::endl;
}

int main( int argc, char** argv )
{
   // Select methods (don't look at this code - not of interest)
   TString methodList; 
   for (int i=1; i<argc; i++) {
      TString regMethod(argv[i]);
      if(regMethod=="-b" || regMethod=="--batch") continue;
      if (!methodList.IsNull()) methodList += TString(","); 
      methodList += regMethod;
   }
   TMVARegressionApplication(methodList);
   return 0;
}
