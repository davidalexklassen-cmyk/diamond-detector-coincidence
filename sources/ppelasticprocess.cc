#include "ppelasticprocess.hh"

PPelasticModel::PPelasticModel(G4double thetaMin_CM) : G4VEmModel("PPelastic"), fThetaMin(thetaMin_CM)
{
    SetLowEnergyLimit(PPxsection::MinEnergy());
    SetHighEnergyLimit(PPxsection::MaxEnergy());
}

void PPelasticModel::Initialise(const G4ParticleDefinition*, const G4DataVector&)
{
    if (!fxs) fxs = std::make_unique<PPxsection>(G4String(PROJECT_DATA_DIR) + "/ppPhaseShift.dat", fThetaMin);
    if (!fParticleChange) fParticleChange = GetParticleChangeForGamma();
}

G4double PPelasticModel::ComputeCrossSectionPerAtom(const G4ParticleDefinition* p, G4double kinEnergy, G4double Z, G4double, G4double, G4double)
{
    if(p != G4Proton::Proton() || G4lrint(Z) != 1) return 0.;
    return fxs->Totalxs(kinEnergy);
}

void PPelasticModel::SampleSecondaries(std::vector<G4DynamicParticle*>* secondaries, const G4MaterialCutsCouple*, const G4DynamicParticle* dp, G4double cut, G4double)
{
    const G4double kinE = dp->GetKineticEnergy();

    const G4double cost = fxs->SampleCosTheta_CM(kinE, G4Random::getTheEngine());
    const G4double phi = CLHEP::twopi * G4UniformRand();
    const ppKin::FinalState f = ppKin::Scatter(kinE, cost, phi);
    const G4ThreeVector dir0 = dp->GetMomentumDirection();
    G4ThreeVector dir1 = f.p1.vect().unit();
    G4ThreeVector dir2 = f.p2.vect().unit();
    dir1.rotateUz(dir0);
    dir2.rotateUz(dir0);
    fParticleChange->ProposeMomentumDirection(dir1);
    fParticleChange->SetProposedKineticEnergy(f.E1);
    
    if (f.E2 > cut)
        secondaries->push_back(new G4DynamicParticle(G4Proton::Proton(), dir2, f.E2));
    else
        fParticleChange->ProposeLocalEnergyDeposit(f.E2);
}

PPelasticProcess::PPelasticProcess(G4double thetaMin_CM) : G4VEmProcess("ppElastic"), fThetaMin(thetaMin_CM)
{
    SetSecondaryParticle(G4Proton::Proton());
    SetProcessSubType(fCoulombScattering);
    SetCrossSectionType(fEmNoIntegral);
    SetBuildTableFlag(false);
}

G4bool PPelasticProcess::IsApplicable(const G4ParticleDefinition& p)
{
    return &p == G4Proton::Proton();
}

void PPelasticProcess::InitialiseProcess(const G4ParticleDefinition*)
{
    if (fInitialised) return;
    if (nullptr == EmModel(0)) SetEmModel(new PPelasticModel(fThetaMin));
    AddEmModel(1, EmModel(0));
    fInitialised = true;
}