#ifndef PPELASTICPROCESS_HH
#define PPELASTICPROCESS_HH

#include "ppxsection.hh"
#include "G4VEmModel.hh"
#include "G4VEmProcess.hh"
#include <memory>
#include "G4DynamicParticle.hh"
#include "G4EmProcessSubType.hh"
#include "G4ParticleChangeForGamma.hh"
#include "G4Proton.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

class G4ParticleChangeForGamma;

class PPelasticModel : public G4VEmModel
{
    public:
        explicit PPelasticModel(G4double thetaMin_CM);

        void Initialise(const G4ParticleDefinition*, const G4DataVector&) override;

        G4double ComputeCrossSectionPerAtom(const G4ParticleDefinition*, G4double kinEnergy, G4double Z, G4double A, G4double cut, G4double emax) override;

        void SampleSecondaries(std::vector<G4DynamicParticle*>*, const G4MaterialCutsCouple*, const G4DynamicParticle*, G4double cut, G4double) override;
    private:
        G4double fThetaMin;
        std::unique_ptr<PPxsection> fxs;
        G4ParticleChangeForGamma* fParticleChange = nullptr;
};

class PPelasticProcess : public G4VEmProcess
{
    public:
        explicit PPelasticProcess(G4double thetaMin_CM);
        G4bool IsApplicable(const G4ParticleDefinition& p) override;

    private:
        G4double fThetaMin;
        G4bool fInitialised = false;

    protected:
        void InitialiseProcess(const G4ParticleDefinition*) override;

};

#endif