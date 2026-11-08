#ifndef CALLTABLE_HH
#define CALLTABLE_HH

#include <string>
#include <vector>
#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Random/RandomEngine.h"
#include "ppkinematics.hh"
#include <algorithm>
#include <cmath>
#include <complex>
#include <fstream>
#include <sstream>
#include <stdexcept>

class PPxsection
{
    public:
        PPxsection(const std::string& phaseShiftFile, double thetaMin_CM);

        double Diffxs_CM(double Tlab, double theta_CM) const;
        double Totalxs(double Tlab) const;
        double SampleCosTheta_CM(double Tlab, CLHEP::HepRandomEngine* engine) const;
        static double MinEnergy();
        static double MaxEnergy();

    private:
        void Phases(double Tlab, double d[5]) const;

        struct Amplitudes
        {
            double k, eta;
            std::complex<double> aS, aD, a0, a1, a2; // a1S0, a1D2, a3P0, a3P1, a3P2
        };

        Amplitudes NuclearAmplitudes(double Tlab) const;
        static double Diffxs(const Amplitudes& A, double theta_CM);
        std::vector<double> Cumulative(double Tlab) const;
        std::vector<double> fE, fDelta[5];
        double fWmin, fDW;
};

#endif