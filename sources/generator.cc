#include "generator.hh"

PrimaryGenerator::PrimaryGenerator()
{
    fParticleGun = new G4ParticleGun();
    fpp = std::make_unique<PPxsection>(G4String(PROJECT_DATA_DIR) + "/ppPhaseShift.dat", 0.25 * deg);
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

    const G4double beamSpot = 1. * mm;
    G4ThreeVector pos(G4RandGauss::shoot(0.,beamSpot) * cfg::scalar_x, G4RandGauss::shoot(0.,beamSpot) * cfg::scalar_y, 50.*cm);
    G4ThreeVector mom(0.,0.,-1.);

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);

    const G4double eSpread = 3. * keV;
    fParticleGun->SetParticleEnergy(G4RandGauss::shoot(cfg::beamEnergy, eSpread));
    fParticleGun->SetParticleDefinition(particle); 

    Scattered s;
    s.dir1 = mom;
    s.E1 = G4RandGauss::shoot(cfg::beamEnergy, eSpread);

    if(cfg::bias_active)
    {
        s = ForceScattering(s.E1, *fpp);
        pos.setZ(s.z);
    }

    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticlePosition(pos);

    for (auto [dir, e] : {std::pair{s.dir1, s.E1}, std::pair{s.dir2, s.E2}})
    
    {
        if (e <= 0.) continue;
        fParticleGun->SetParticleMomentumDirection(dir);
        fParticleGun->SetParticleEnergy(e);
        fParticleGun->GeneratePrimaryVertex(anEvent);
    }

    for (G4int i = 0; i < anEvent->GetNumberOfPrimaryVertex(); i++) 
        anEvent->GetPrimaryVertex(i)->GetPrimary()->SetWeight(s.weight);



}