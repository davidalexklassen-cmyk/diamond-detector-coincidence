#include "ppkinematics.hh"

namespace ppKin
{
    const double m = CLHEP::proton_mass_c2;
    double E_CM_sq(double E_kin_lab) {return 2. * m * (E_kin_lab + 2. * m);}
    double Plab(double E_kin_lab) {return std::sqrt(E_kin_lab * (E_kin_lab + 2. * m));}
    double P_CM(double E_kin_lab) {return std::sqrt(E_CM_sq(E_kin_lab) / 4. - m * m);}
    double Wave_k(double E_kin_lab) {return P_CM(E_kin_lab) / CLHEP::hbarc;}
    double Beta_CM(double E_kin_lab) {return Plab(E_kin_lab) / (E_kin_lab + 2. * m);}
    double Gamma_CM(double E_kin_lab) {return (E_kin_lab + 2. * m) / std::sqrt(E_CM_sq(E_kin_lab));}
    double Sommerfeld(double E_kin_lab) {return CLHEP::fine_structure_const * (E_kin_lab + m) / Plab(E_kin_lab);}

    FinalState Scatter(double E_kin_lab, double cosTheta, double phi)
    {
        const double sinTheta = std::sqrt((1. - cosTheta) * (1. + cosTheta));
        const double pcm = P_CM(E_kin_lab);
        CLHEP::HepLorentzVector p1(pcm * sinTheta * std::cos(phi), pcm * sinTheta * std::sin(phi), pcm * cosTheta, std::sqrt(E_CM_sq(E_kin_lab)) / 2.);
        p1.boost(0., 0., Beta_CM(E_kin_lab));
        const CLHEP::HepLorentzVector pTot(0., 0., Plab(E_kin_lab), E_kin_lab + 2. * m);
        const CLHEP::HepLorentzVector p2 = pTot - p1;

        const double E1 = std::max(p1.e() - m, 0.);
        return {p1, p2, E1, std::max(E_kin_lab - E1, 0.)};
    }
}