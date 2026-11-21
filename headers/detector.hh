#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4ThreeVector.hh"
#include "Randomize.hh"
#include <map>
#include <utility>

class SensitiveDetector : public G4VSensitiveDetector
{
    public:
        SensitiveDetector(G4String);
        ~SensitiveDetector();

    private:
        G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;
        void EndOfEvent(G4HCofThisEvent*) override;
        struct Hit {G4double edep = 0.; G4double weight = 1.; G4ThreeVector pos;};
        std::map<std::pair<G4int,G4int>, Hit> fHits;
        
};

#endif