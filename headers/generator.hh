#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "config.hh"
#include "Randomize.hh"
#include "ppxsection.hh"
#include <memory>
#include "G4Event.hh"
#include "biasing.hh"

class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
    public:
        PrimaryGenerator();
        ~PrimaryGenerator();

        void GeneratePrimaries(G4Event* event) override;

    private:
        G4ParticleGun* fParticleGun;
        std::unique_ptr<PPxsection> fpp;
};
#endif