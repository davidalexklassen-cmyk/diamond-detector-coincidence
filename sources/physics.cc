#include "physics.hh"

PhysicsList::PhysicsList()
{
    RegisterPhysics(new G4EmStandardPhysicsSS());
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::ConstructProcess()
{
    G4VModularPhysicsList::ConstructProcess();
}