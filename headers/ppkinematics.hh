#ifndef PPKINEMATICS_HH
#define PPKINEMATICS_HH

#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include <algorithm>
#include <cmath>


//relativistic kinematics of elastic p+p on proton at rest

namespace ppKin
{
    double Plab(double E_kin_lab);
    double P_CM(double E_kin_lab);
    double Wave_k(double E_kin_lab);
    double E_CM_sq(double E_kin_lab);
    double Beta_CM(double E_kin_lab);
    double Gamma_CM(double E_kin_lab);
    double Sommerfeld(double E_kin_lab);

    struct FinalState
    {
        CLHEP::HepLorentzVector p1, p2;
        double E1, E2;
    };
    FinalState Scatter(double E_kin_lab, double cosTheta, double phi);
}
#endif