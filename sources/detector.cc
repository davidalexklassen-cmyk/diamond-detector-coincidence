#include "detector.hh"

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

SensitiveDetector::~SensitiveDetector()
{}

G4bool SensitiveDetector::ProcessHits(G4Step* astep, G4TouchableHistory* R0hist)
{
    G4Track* track = astep->GetTrack();
    
    G4StepPoint* preStepPoint = astep->GetPreStepPoint();
    G4StepPoint* postStepPoint = astep->GetPostStepPoint();

    G4ThreeVector posProton = preStepPoint->GetPosition();

    const G4VTouchable* touchable = preStepPoint->GetTouchable();
    G4int detectorID = touchable->GetCopyNumber(0);
    G4int arrayID = touchable->GetCopyNumber(1);


    G4VPhysicalVolume* physVol = touchable->GetVolume();
    G4ThreeVector posDetector = physVol->GetTranslation();
    G4double edepos = astep->GetTotalEnergyDeposit();
    
    G4cout 
    << "Array: " << arrayID
    << " Detector: " << detectorID
    << " Position: " << posProton 
    << " Energy: " << edepos / keV << " keV" 
    << G4endl;

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();


    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(0, arrayID);
    man->FillNtupleIColumn(1, detectorID);
    man->FillNtupleDColumn(2, edepos / keV);
    man->FillNtupleDColumn(3, posProton[0]);
    man->FillNtupleDColumn(4, posProton[1]);
    man->FillNtupleDColumn(5, posProton[2]);
    man->AddNtupleRow(0);

    return true;
}