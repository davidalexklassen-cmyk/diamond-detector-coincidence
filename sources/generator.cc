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
    const G4long n = ++cfg::eventsStarted;
    const G4long step = std::max<G4long>(cfg::nEvents / 10, 1);
    if (n % step == 0)
    {
        const double s = std::chrono::duration<double>(std::chrono::steady_clock::now() - cfg::tStart).count();
        G4cout << "Progress:" << 100 * n / cfg::nEvents << "% (" << n << "/" << cfg::nEvents << ") " << s << "s since start. About " << s * (cfg::nEvents - n) / n << "till done." << G4endl;
    }

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName = "proton";
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName);

    G4ThreeVector pos(0.,0.,50.*cm);
    G4ThreeVector mom(0.,0.,-1.);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleEnergy(6.*MeV);
    fParticleGun->SetParticleDefinition(particle);

    fParticleGun->GeneratePrimaryVertex(anEvent);
}