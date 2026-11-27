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

    const G4double a0 = std::abs(cfg::ArmAngle(0));
    const G4double a1 = std::abs(cfg::ArmAngle(1));
    const G4double labLo = std::max(std::min(a0, a1) - cfg::biasDTheta, 0.);
    const G4double labHi = std::min(std::max(a0, a1) + cfg::biasDTheta, pi);
    const G4double phi = (G4UniformRand() < 0.5 ? 0.:pi) + (2. * G4UniformRand() -1.) * cfg::biasDPhi;
    const G4double dOmegaPhi = 4. * cfg::biasDPhi;



    if(G4lrint(Z) == 1)
    {
        const G4double g = ppKin::Gamma_CM(E);

        const G4double cHi = std::cos(2. * std::atan(g * std::tan(std::min(labLo, 89.9*deg))));
        const G4double cLo = std::cos(2. * std::atan(g * std::tan(std::min(labHi, 89.9*deg))));
        const G4double c = cLo + G4UniformRand() * (cHi - cLo);

        const ppKin::FinalState f = ppKin::Scatter(E, c, phi);

        s.dir1 = f.p1.vect().unit();
        s.dir1.rotateUz(G4ThreeVector(0., 0., -1.));

        s.dir2 = f.p2.vect().unit();
        s.dir2.rotateUz(G4ThreeVector(0., 0., -1.));

        s.E1 = f.E1;
        s.E2 = f.E2;

        const G4int n = (-c >= cLo && -c <= cHi) ? 2:1;
        s.weight = nt * pp.Diffxs_CM(E, std::acos(c)) * (cHi -cLo) * dOmegaPhi / n;
        rPreprocess = {1, std::acos(c), E};
    }
    else 
    {
        const G4double lo_cm = labLo + std::asin(std::sin(labLo)/A);
        const G4double hi_cm = labHi + std::asin(std::sin(labHi)/A);
        const G4double xMax = 1. / (1. - std::cos(lo_cm));
        const G4double xMin = 1. / (1. - std::cos(hi_cm));

        const G4double c_CM = 1. - 1. / (xMin + G4UniformRand() * (xMax - xMin));
        const G4double s_CM = std::sqrt(1. - c_CM * c_CM);

        const G4double th =std::atan2(s_CM, c_CM + 1. / A);
        const G4double a = Z * elm_coupling / (4. * E * A / (1. + A));

        s.dir1 = G4ThreeVector(std::sin(th) * std::cos(phi), std::sin(th) * std::sin(phi), -std::cos(th));
        s.E1 = E * (A * A + 2. * A * c_CM + 1.) / ((1. + A) * (1. + A));
        s.weight = nt * a * a * 4. * (xMax -xMin) * dOmegaPhi;
        rPreprocess = {G4lrint(Z), std::acos(c_CM), E};
    }
    s.weight *= nEl;
    return s;
}