#include "generator.hh"

PrimaryGenerator::PrimaryGenerator()
{
    fParticleGun = new G4ParticleGun();
}

PrimaryGenerator::~PrimaryGenerator()
{
    delete fParticleGun;
}

void PrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "proton";
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName);

    G4ThreeVector pos(0.,0.,50.*cm);
    G4ThreeVector mom(0.,0.,-1.);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleEnergy(5.*MeV);
    fParticleGun->SetParticleDefinition(particle);

    fParticleGun->GeneratePrimaryVertex(anEvent);
}