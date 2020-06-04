void run(){
gSystem->mkdir("templates", kTRUE);
gROOT->Reset();
gROOT->ProcessLine(".x src/Population.cxx++");
gROOT->ProcessLine(".x genetic.C++");

// se puede posteriormente 
//root -l 
//gSystem->Load("genetic_C.so")
//genetic() 
}
