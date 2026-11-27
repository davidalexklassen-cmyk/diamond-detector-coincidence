#include "run.hh"

RunAction::RunAction()
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    
    //Sensitive Detector
    man->SetNtupleMerging(true); 
    man->CreateNtuple("DetectorData","Detector Hits");
    man->CreateNtupleIColumn("EventID");
    man->CreateNtupleIColumn("Array");
    man->CreateNtupleIColumn("Detector");
    man->CreateNtupleDColumn("Edep");
    man->CreateNtupleDColumn("Weight");
    man->CreateNtupleIColumn("TrueEdep");
    man->CreateNtupleIColumn("Z");
    man->CreateNtupleDColumn("Theta_CM");
    man->CreateNtupleDColumn("EVertex");
    man->FinishNtuple(0);

    /*
    //rutherford validation
    man->CreateNtuple("scatter","theta");
    man->CreateNtupleIColumn("EventID");
    man->CreateNtupleDColumn("theta_deg");
    man->CreateNtupleDColumn("E_MeV");      
    man->FinishNtuple(1);
    */
    
}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->OpenFile("runs/" + cfg::filename);
}

void RunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();
}