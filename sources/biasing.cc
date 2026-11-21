#include "biasing.hh"
#include "config.hh"
#include "G4Material.hh"
#include "G4EmCalculator.hh"
#include "Randomize.hh"
#include "G4Proton.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

Scattered ForceScattering(G4double E, const PPxsection& pp)
{
    Scattered s;
    const G4Material* mat = G4Material::GetMaterial(cfg::foilMat);

    const G4int nEl = mat->GetNumberOfElements();
    const G4int i = std::min(G4int(G4UniformRand() * nEl), nEl - 1);
    const G4double Z = mat->GetElement(i)->GetZ();
    const G4double A = mat->GetElement(i)->GetN() * amu_c2 / proton_mass_c2;
    const G4double nt = mat->GetVecNbOfAtomsPerVolume()[i] * cfg::foilThickness;

    s.z = (G4UniformRand() - 0.5) * cfg::foilThickness;
    const G4ParticleDefinition* p = G4Proton::Proton();
    G4EmCalculator calc;
    E = calc.GetKinEnergy(calc.GetRangeFromRestricteDEDX(E, p, mat) - (0.5 * cfg::foilThickness -s.z), p, mat);

    const G4double phi = (G4UniformRand() < 0.5 ? 0. : pi) + (2. * G4UniformRand() -1.) * cfg::dPhi;
    const G4double dPhiTot = 2. * cfg::dPhi * 2.;

    if(G4lrint(Z) == 1)
    {
        const G4double g = ppKin::Gamma_CM(E);

        const G4double cMax = std::cos(2. * std::atan(g * std::tan(cfg::thetaMin)));
        const G4double cMin = std::cos(2. * std::atan(g * std::tan(cfg::thetaMax)));

        const G4double c = cMin + G4UniformRand() * (cMax - cMin);

        const ppKin::FinalState f = ppKin::Scatter(E, c, phi);
        s.dir1 = f.p1.vect().unit();
        s.dir1.rotateUz(G4ThreeVector(0., 0., -1.));

        s.dir2 = f.p2.vect().unit();
        s.dir2.rotateUz(G4ThreeVector(0., 0., -1.));

        s.E1 = f.E1;
        s.E2 = f.E2;

        s.weight = nt * pp.Diffxs_CM(E, std::acos(c)) * (cMax -cMin) * dPhiTot / 2.;
    }
    else 
    {
        const G4double cMax = std::cos(cfg::thetaMin);
        const G4double cMin = std::cos(cfg::thetaMax);

        const G4double c = cMin + G4UniformRand() * (cMax - cMin);

        const G4double th =std::acos(c);
        const G4double th_CM =th + std::asin(std::sin(th) / A);

        const G4double a = Z * elm_coupling / (4. * E * A / (1. + A));
        const G4double jac = std::pow(1. + 2. * std::cos(th_CM) / A + 1. / (A * A), 1.5) / std::abs(1. + std::cos(th_CM) / A);

        s.dir1 = G4ThreeVector(std::sin(th) * std::cos(phi), std::sin(th) * std::sin(phi), -c);
        s.E1 = E * std::pow((c + std::sqrt(A * A - std::sin(th) * std::sin(th))) / (1. + A), 2);
        s.weight = nt * a * a / std::pow(std::sin(th_CM/2.),4) * jac * (cMax -cMin) * dPhiTot;
    }
    s.weight *= nEl;
    return s;
}