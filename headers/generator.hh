#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGenerator();
    ~PrimaryGenerator();

    void GeneratePrimaries(G4Event* event) override;

private:
    G4ParticleGun* fParticleGun;
};
#endif