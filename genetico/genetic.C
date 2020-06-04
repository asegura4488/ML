#include "src/Population.h"

int iterator_ = 0;
int generations = 2000;
int n_crosses = 400;
double probability_crosses = 0.6;
double probability_mutation = 0.5;

int population_size = 800;

/////////////// Internal
Int_t  N_Crosses = 0;
Int_t  N_Mutations = 0; 
Double_t  Best_Score = 0; 


TCanvas *c1;
TCanvas *c2;
TGraph *g1;
TGraph *g2;
TGraph *g3;
TH1D *h1;
TH1D *h2;
TH1D *h3;
TLegend *legend;
TTimer *counter;
TRandom *rand_;
Population *p1;
TControlBar *bar;
//Dynamic Array
Double_t *X; 
Double_t *Y; 
Double_t *X_; 
Double_t *Y_; 
Double_t *X_s;
Double_t *Y_s;

std::chrono::time_point<std::chrono::system_clock> start_;
std::chrono::time_point<std::chrono::system_clock> end_;

void crossing(Int_t crosses, Double_t crossing_prob, Double_t mutation_prob, Int_t& N_Crosses_, Int_t& N_Mutations_){
    
  Double_t seed=0.; 
  Double_t min_mutation = 0.;
  Double_t max_mutation = 0.;
  Double_t son_score = 0.;
  Double_t mother_score = 0.;
  
  for(int i = 0; i < crosses; i++){
    seed = rand_->Uniform(0.,1.); 
    //seed = abs(rand_->Gaus(0.5,1.0));
    if(seed < crossing_prob){
       
      N_Crosses_++;
      int index_mother_1 = rand() % p1->po_size;
      int index_mother_2 = 0;
      
      do{
	index_mother_2 = rand() % p1->po_size; 
      }while(index_mother_1 == index_mother_2); // Elegir posiciones diferentes para el cruze 
      
      std::vector<Double_t> son;
      p1->Get_Crossing(index_mother_1, index_mother_2, son);
      
      
      min_mutation = rand_->Uniform(-20.,0.);
      max_mutation = rand_->Uniform(0.,20.);
  
       // Mutation of the son
      seed = rand_->Uniform(0.,1.);  
      if(seed < mutation_prob){
        N_Mutations_++; 
        p1->Get_Mutation(son, min_mutation, max_mutation);
      }

      son_score = p1->Get_Score(son);
      mother_score = p1->Get_Score(p1->Get_Individual(index_mother_1));
      
      if(son_score < mother_score) p1->Set_Individual(index_mother_1, son);
      
    } // Minimum value to cross

  } // loop over crosses
  
}

void Update_graph(){
  for(Int_t i = 0; i < p1->po_size; i++){
    X[i] = p1->Get_Individual(i)[0];
    Y[i] = p1->Get_Individual(i)[1]; 
  }
}

void Update_score(Double_t& best){

 for(Int_t i = 0; i < p1->po_size; i++){
   X_[i] = i+1;
   Y_[i] = p1->Get_Total_Score()[i];
   best += Y_[i];   
 }
   best /= p1->po_size;
}

void Destructor(){
  counter->TurnOff();

   delete c1;
   delete c2;
  // delete g1;
  // delete g2;
  // delete g3;
   delete h1;
   delete h2;
   delete h3;
   delete legend;
   delete counter;
   delete rand_;
   delete p1;
   delete bar;
//Dynamic Array
   delete[] X; 
   delete[] Y; 
   delete[] X_; 
   delete[] Y_; 
   delete[] X_s;
   delete[] Y_s;

}

void Generation(Int_t &k){
  k++;
 
  if(k % 100 == 0){ 
	std::cout << " Generation: " << k << " Population Size: " << p1->Print_Size() << std::endl;      
  }
  
  c1->Clear();
  c1->Divide(2,2); 
 
  N_Crosses = 0;
  N_Mutations = 0; 
  crossing(n_crosses, probability_crosses, probability_mutation, N_Crosses, N_Mutations);

  h1->Fill(N_Crosses);
  h2->Fill(N_Mutations);
  Update_graph();
  Update_score(Best_Score);
 // h3->GetXaxis()->SetRangeUser(0.,1.0/Best_Score);
 // h3->SetBins(100.,0.,Best_Score);
  h3->Fill(Best_Score);
 
  g1 = new TGraph(p1->po_size,X,Y);
  g1->SetTitle("Fitness");
  g1->SetMarkerStyle(20);
  g1->SetMarkerSize(0.7);
  g1->SetMarkerColor(4);
  g1->GetXaxis()->SetTitle("X[]");
  g1->GetYaxis()->SetTitle("Y[]");

  g2 = new TGraph(p1->po_size,X_,Y_);
  g2->SetTitle("Score_per_Individual");
  g2->SetMarkerStyle(4);
  g2->SetMarkerSize(0.7);
  g2->SetMarkerColor(kGreen+4);
  g2->GetXaxis()->SetTitle("Individual");
  g2->GetYaxis()->SetTitle("Score");

  c1->cd(1);
  g1->Draw("AP");
  c1->cd(2);
  gPad->SetLogy();
  g2->Draw("AP");
  c1->cd(3);
  
  h1->Draw();
  h2->Draw("same");

  if(k==1){
  legend->AddEntry("h1","N_Crosses","l");
  legend->AddEntry("h2","N_Mutations","l");
  } 
  legend->Draw();
   
  c1->cd(4);
  gPad->SetLogy();
  h3->Draw();
      
  c1->Modified();
  c1->Update();
  
  c2->Clear();
  c2->cd();
  gPad->SetLogy();
  X_s[k-1] = k;
  Y_s[k-1] = Best_Score;
  g3 = new TGraph(k,X_s,Y_s);
  g3->SetTitle("Mean_Score_Individual");
  g3->SetMarkerStyle(4);
  g3->SetMarkerSize(0.7);
  g3->SetMarkerColor(kGreen+4);
  g3->GetXaxis()->SetTitle("Generation");
  g3->GetYaxis()->SetTitle("Mean_Score");
  g3->Draw();

  c2->Modified();
  c2->Update();

  
  if( k == generations ){
    
    end_ = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_-start_;
    double run_time_real=elapsed_seconds.count();
    std::cout << " elapsed time: " << run_time_real 
	      << " elapsed time per generation: " << run_time_real/generations  
	      << std::endl;
   // p1->Print();
    
	Destructor();
  } 
  
}

void Start(){
counter->TurnOn();  
}

void Pause(){
counter->TurnOff();  
}

void genetic();

void Reset(){
Destructor();
iterator_=0;
genetic();
}

void gui(){
//TCanvas *c4 = new TCanvas("c1","Fitness", 300,300,800,600 ); 
bar = new TControlBar("vertical", "Options",30,30);
bar->AddButton("Reset","Reset()", "Reset");
bar->AddButton("Start","Start()", "Start");
bar->AddButton("Pause","Pause()", "Pause");
bar->AddButton("Stop","Destructor()", "Stop genetic");
bar->SetButtonWidth(100);
bar->SetTextColor("black");
//bar->SetFont("monospaceblack");  
bar->Show();
}


void genetic(){
  p1 = new Population(population_size,3); 
  p1->Init(-20.,20.);
  c1 = new TCanvas("c1","Fitness", 100,100,500,500 ); 
  c2 = new TCanvas("c2","mean", 600,100,500,500 ); 
  h1 = new TH1D("h1","Crosses", n_crosses, 0.,n_crosses);
  h2 = new TH1D("h2","Mutations", n_crosses, 0.,n_crosses);
  h3 = new TH1D("h3","Best_Score", 100., 0.,100.);

  h1->SetTitle("");
  h1->SetLineColor(1);
  h1->SetLineWidth(2);
  h1->SetStats(kFALSE);

  h2->SetTitle("");
  h2->SetLineColor(2);
  h2->SetLineWidth(2);
  h2->SetStats(kFALSE);

  h3->SetTitle("");
  h3->SetLineColor(2);
  h3->SetLineWidth(2);
  h3->SetStats(kFALSE);
  h3->GetXaxis()->SetTitle("Score");

  legend = new TLegend(0.70,0.70,0.9,0.9, NULL,"brNDC");
 

  rand_ = new TRandom3();
  X = new Double_t[population_size];
  Y = new Double_t[population_size];
  X_ = new Double_t[population_size];
  Y_ = new Double_t[population_size];
  X_s = new Double_t[generations];
  Y_s = new Double_t[generations];
  
  
  counter = new TTimer("Generation(iterator_)", 1);
  counter->TurnOn();  
  start_ = std::chrono::system_clock::now();  

   
  gui();

}



int main(int argc, char **argv){
  genetic();
  return 0;
}




	
