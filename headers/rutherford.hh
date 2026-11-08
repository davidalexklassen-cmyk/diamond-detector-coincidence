#ifndef RUTHERFORD_HH
#define RUTHERFORD_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

class RutherfordAction : public G4UserSteppingAction
{
    public:
        void UserSteppingAction(const G4Step* step) override;
};

#endif