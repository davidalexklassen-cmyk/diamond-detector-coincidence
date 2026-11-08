#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VModularPhysicsList.hh"

#include "G4EmStandardPhysicsSS.hh"
#include "G4EmParameters.hh"
#include "G4NuclearFormfactorType.hh"
#include "G4ProcessVector.hh"
#include "G4ProcessManager.hh"
#include "G4CoulombScattering.hh"
#include "removehydrocoulomb.hh"
#include "G4Proton.hh"


class PhysicsList : public G4VModularPhysicsList
{
  public:
    PhysicsList();
    ~PhysicsList();

    void ConstructProcess() override;
};

#endif