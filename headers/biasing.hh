#ifndef BIASING_HH
#define BIASING_HH

#include "G4ThreeVector.hh"
#include "ppxsection.hh"

struct Scattered
{
    G4ThreeVector dir1, dir2;
    G4double E1 = 0.;
    G4double E2 = 0.;
    G4double z= 0.;
    G4double weight = 1.;
};

Scattered ForceScattering(G4double E, const PPxsection& pp);

#endif