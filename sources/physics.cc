#include "physics.hh"

PhysicsList::PhysicsList()
{
    RegisterPhysics(new G4EmStandardPhysicsSS());
    RegisterPhysics(new G4HadronElasticPhysics());    
    G4EmParameters* em = G4EmParameters::Instance();
    em->SetNuclearFormfactorType(fExponentialNF);
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::ConstructProcess()
{
    G4VModularPhysicsList::ConstructProcess();
}