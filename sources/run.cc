#include "run.hh"

RunAction::RunAction()
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    
    man->SetNtupleMerging(true); 
    man->CreateNtuple("DetectorData","Detector Hits");
    man->CreateNtupleIColumn("Array");
    man->CreateNtupleIColumn("Detector");
    man->CreateNtupleDColumn("Energyloss");
    man->CreateNtupleDColumn("X");
    man->CreateNtupleDColumn("Y");
    man->CreateNtupleDColumn("Z");
    man->FinishNtuple(0);
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