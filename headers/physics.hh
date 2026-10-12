#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "G4VModularPhysicsList.hh"

#include "G4EmStandardPhysicsSS.hh"
#include "G4EmExtraPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4OpticalPhysics.hh"
#include "G4PhysicsListHelper.hh"
#include "G4CoulombScattering.hh"
#include "G4hCoulombScatteringModel.hh"
#include "G4Proton.hh"
#include "G4ProcessManager.hh"

class PhysicsList : public G4VModularPhysicsList
{
public:
  PhysicsList();
  ~PhysicsList();

  void ConstructProcess() override;
};

#endif