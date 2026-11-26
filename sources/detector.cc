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

    if (edepos <= 0.) return false;
    Hit& hit = fHits[{arrayID, detectorID}];
    if (hit.edep == 0.) hit.pos = posProton;
    hit.edep += edepos;
    hit.weight = astep->GetTrack()->GetWeight();

    return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    const G4double sigma = cfg::detector_resolution_FWHM / 2.355;
    const G4double threshold = 200. * keV;

    for (const auto& [id, hit] : fHits)
    {
        const G4double e = G4RandGauss::shoot(hit.edep, sigma);
        if (e < threshold) continue;

        man->FillNtupleIColumn(0, 0, evt);
        man->FillNtupleIColumn(0, 1, id.first);
        man->FillNtupleIColumn(0, 2, id.second);
        man->FillNtupleDColumn(0, 3, e / MeV);
        man->FillNtupleDColumn(0, 4, hit.weight);
        man->AddNtupleRow(0);
    }
    fHits.clear();
}