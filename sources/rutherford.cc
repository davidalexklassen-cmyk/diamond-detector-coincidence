#include "rutherford.hh"

void RutherfordAction::UserSteppingAction(const G4Step* step)
{
    G4Track* track = step->GetTrack();
    if (track->GetParentID() != 0) return;

    G4VPhysicalVolume* preStepPoint = step->GetPreStepPoint()->GetPhysicalVolume();
    G4VPhysicalVolume* postStepPoint = step->GetPostStepPoint()->GetPhysicalVolume();
    if (!preStepPoint || preStepPoint->GetName() != "Foil") return;
    if (postStepPoint && postStepPoint ->GetName() == "Foil") return;
    
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager* man = G4AnalysisManager::Instance();
    
    man->FillNtupleIColumn(1, 0, evt);
    man->FillNtupleDColumn(1, 1, track->GetMomentumDirection().angle(G4ThreeVector(0, 0, -1)) / deg);
    man->FillNtupleDColumn(1, 2, step->GetPostStepPoint()->GetKineticEnergy() / MeV);
    man->AddNtupleRow(1);
}