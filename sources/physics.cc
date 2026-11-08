#include "physics.hh"

PhysicsList::PhysicsList()
{
    RegisterPhysics(new G4EmStandardPhysicsSS());
    G4EmParameters* em = G4EmParameters::Instance();
    em->SetNuclearFormfactorType(fNoneNF);
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::ConstructProcess()
{
    G4VModularPhysicsList::ConstructProcess();
    G4ProcessVector* procs = G4Proton::Proton()->GetProcessManager()->GetProcessList();
    for (size_t i = 0; i < procs->size(); ++i)
    if (auto* cs = dynamic_cast<G4CoulombScattering*>((*procs)[i]))
    cs->SetEmModel(new RemoveHydrogenCoulomb);
}