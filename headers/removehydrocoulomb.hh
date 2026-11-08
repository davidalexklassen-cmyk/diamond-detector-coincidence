#ifndef REMOVEHYDROCOULOMB_HH
#define REMOVEHYDROCOULOMB_HH

#include "G4eCoulombScatteringModel.hh"

class RemoveHydrogenCoulomb : public G4eCoulombScatteringModel
{
    public:
        RemoveHydrogenCoulomb() : G4eCoulombScatteringModel(false) {}
        G4double ComputeCrossSectionPerAtom(const G4ParticleDefinition* p, G4double E, G4double Z, G4double A, G4double cut, G4double emax) override
        {
            if (Z < 1.1) return 0.;
            return G4eCoulombScatteringModel::ComputeCrossSectionPerAtom(p, E, Z, A, cut, emax);

        }
};

#endif