#include "run.hh"

RunAction::RunAction()
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    
    man->SetNtupleMerging(true); 
    man->CreateNtuple("DetectorData","Detector Hits");
    man->CreateNtupleIColumn("EventID");
    man->CreateNtupleIColumn("Array");
    man->CreateNtupleIColumn("Detector");
    man->CreateNtupleDColumn("Energyloss");
    man->CreateNtupleDColumn("X");
    man->CreateNtupleDColumn("Y");
    man->CreateNtupleDColumn("Z");
    man->FinishNtuple(0);

    man->CreateNtuple("scatter","theta");
    man->CreateNtupleIColumn("EventID");
    man->CreateNtupleDColumn("theta_deg");
    man->CreateNtupleDColumn("E_MeV");    
    man->FinishNtuple(1);
}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->OpenFile("output.root");
}

void RunAction::EndOfRunAction(const G4Run*)
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();
}