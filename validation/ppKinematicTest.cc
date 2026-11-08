#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "ppkinematics.hh"
#include <cmath>
#include <cstdio>

int fails = 0;
void Check(const char* what, double value, double expected, double tol)
{
    const double dev = std::abs(value - expected) / std::abs(expected);
    const bool ok = dev < tol;
    if (!ok) ++fails;
    std::printf("%-34s %14.7g expected %14.7g rel.dev %.1e %s \n", what, value, expected, dev, ok ? "ok" : "FAIL");
}

int main ()
{
    using namespace CLHEP;
    const double m = proton_mass_c2;
    const double E = 6. * MeV;
    Check("sqrt(E_CM_sq) [MeV]", std::sqrt(ppKin::E_CM_sq(E)), 1879.542, 1e-6);
    Check("p_lab c [MeV]", ppKin::Plab(E), 106.279, 1e-5);
    Check("P_CM c [MeV]", ppKin::P_CM(E), 53.0548, 1e-5);
    Check("Wave_k [1/fm]", ppKin::Wave_k(E) * fermi, 0.2689, 2e-4);
    Check("Sommerfeld", ppKin::Sommerfeld(E), 0.06484, 2e-4);
    Check("gamma_cm", ppKin::Gamma_CM(E), 1.001597, 1e-6);
    Check("beta_cm", ppKin::Beta_CM(E), 0.05646, 1e-3);

    for(double thDeg : {10., 45., 90.})
    {
        const double th = thDeg * deg;
        const ppKin::FinalState f = ppKin::Scatter(E, std::cos(th), 0.3);
        const double g = ppKin::Gamma_CM(E);
        char lbl[64];
        std::snprintf(lbl, sizeof lbl, "E1 = E cos^2(th/2), th = %g", thDeg);
        Check(lbl, f.E1, E * std::pow(std::cos(th/2.), 2), 1e-12);

        std::snprintf(lbl, sizeof lbl, "tan th1 = tan(th/2)/g, th=%g", thDeg);
        Check(lbl, std::tan(f.p1.theta()), std::tan(th / 2.)/g, 1e-12);

        std::snprintf(lbl, sizeof lbl, "tan th1 tan th2 = 1/g^2, th=%g", thDeg);
        Check(lbl, std::tan(f.p1.theta()) * std::tan(f.p2.theta()), 1./ (g*g), 1e-12);

        std::snprintf(lbl, sizeof lbl, "m2 of proton 2, th = %g", thDeg);
        Check(lbl, f.p2.m(), m, 1e-12);

        std::snprintf(lbl, sizeof lbl, "phi2 - phi1 = pi, th=%g", thDeg);
        Check(lbl, std::abs(f.p2.phi() - f.p1.phi()), pi, 1e-12);
    }
    std::printf("%s\n", fails ? "CHECKS FAILED" : "all checks passed");
    return fails != 0;
}