#include "detector.hh"

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

SensitiveDetector::~SensitiveDetector()
{}

G4bool SensitiveDetector::ProcessHits(G4Step* astep, G4TouchableHistory* R0hist)
{
    G4StepPoint* preStepPoint = astep->GetPreStepPoint();
    G4StepPoint* postStepPoint = astep->GetPostStepPoint();

    G4ThreeVector posProton = preStepPoint->GetPosition();

    const G4VTouchable* touchable = preStepPoint->GetTouchable();
    G4int detectorID = touchable->GetCopyNumber(0);
    G4int arrayID = touchable->GetCopyNumber(1);

    G4VPhysicalVolume* physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();
    G4double edepos = astep->GetTotalEnergyDeposit();

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager* man = G4AnalysisManager::Instance();
    
    man->FillNtupleIColumn(0, 0, evt);
    man->FillNtupleIColumn(0, 1, arrayID);
    man->FillNtupleIColumn(0, 2, detectorID);
    man->FillNtupleDColumn(0, 3, edepos / keV);
    man->FillNtupleDColumn(0, 4, posProton.x());
    man->FillNtupleDColumn(0, 5, posProton.y());
    man->FillNtupleDColumn(0, 6, posProton.z());
    man->AddNtupleRow(0);

    return true;
}