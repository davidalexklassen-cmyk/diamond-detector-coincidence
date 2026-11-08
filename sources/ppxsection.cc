#include "ppxsection.hh"

namespace 
{
    constexpr double kMinE = 1. * CLHEP::keV;
    constexpr double kMaxE = 50. * CLHEP::MeV;
    constexpr int kNW = 512;
    const std::string kChannels[5] = {"1S0", "1D2", "3P0", "3P1", "3P2"};
}

double PPxsection::MinEnergy() {return kMinE;}
double PPxsection::MaxEnergy() {return kMaxE;}

PPxsection::PPxsection(const std::string& phaseShiftFile, double thetaMin_CM)
{
    std::ifstream file(phaseShiftFile);
    if (!file.is_open()) throw std::runtime_error("ppxsection.cc: failed to open" + phaseShiftFile);

    std::vector<double> E[5];
    int w = -1;
    std::string line;

    while (std::getline(file, line))
    {
        if(line.empty()) continue;
        std::istringstream ss(line);
        if(line[0] == '#')
        {
            std::string hash, name;
            ss >> hash >> name;
            const auto it = std::find(kChannels, kChannels + 5, name);
            if(it != kChannels + 5) w = int(it - kChannels);
            continue;
        }
        double t,d;
        if(w >= 0 && (ss >> t >> d))
        {
            E[w].push_back(t*CLHEP::MeV);
            fDelta[w].push_back(d*CLHEP::deg);
        }
    }
    for (int c = 0; c < 5; ++c)
    if(E[c].size() < 2 || E[c] != E[0])
        throw std::runtime_error("Error in" + kChannels[c]);
    fE= E[0];

    fWmin =std::log(1. - std::cos(thetaMin_CM));
    fDW = -fWmin / (kNW-1);
}

void PPxsection::Phases(double Tlab, double d[5]) const
{
    if (Tlab < fE.front())
    {
        for (int c = 0; c < 5; ++c) d[c] = fDelta[c].front() * Tlab / fE.front();
        return;
    }
    int i = int(std::upper_bound(fE.begin(), fE.end(), Tlab) - fE.begin()) - 1;
    i = std::clamp(i, 0, int(fE.size()) - 2);
    const double f = (Tlab - fE[i]) / (fE[i + 1] - fE[i]);
    for (int c = 0; c < 5; ++c) d[c] = (1. - f) * fDelta[c][i] + f * fDelta[c][i + 1];
}

PPxsection::Amplitudes PPxsection::NuclearAmplitudes(double Tlab) const
{
    using cplx = std::complex<double>;
    const cplx I(0., 1.);
    Amplitudes A;
    A.k = ppKin::Wave_k(Tlab);
    A.eta = ppKin::Sommerfeld(Tlab);
    const double sig1 = std::atan(A.eta), sig2 = sig1 + std::atan(A.eta /2.);
    auto a = [&](double sig, double delta)
    {
        return std::exp(2. * I * sig) * (std::exp(2. * I * delta) - 1.) / (2.* I * A.k);
    };
    double d[5];
    Phases(Tlab, d);
    A.aS = a(0., d[0]);
    A.aD = a(sig2, d[1]);
    A.a0 = a(sig1, d[2]);
    A.a1 = a(sig1, d[3]);
    A.a2 = a(sig1, d[4]);
    return A;
}

double PPxsection::Diffxs(const Amplitudes& A, double theta)
{
    using cplx = std::complex<double>;
    const cplx I(0., 1.);
    auto fC = [&](double t)
    {
        const double s2 = std::pow(std::sin(t/2.),2);
        return -A.eta / (2. * A.k * s2) * std::exp(-I * A.eta * std::log(s2));
    };
    const double x = std::cos(theta), s = std::sin(theta);
    const cplx fDir = fC(theta), fExc = fC(CLHEP::pi - theta);
    const cplx fs = fDir + fExc + 2. * A.aS + 5. * (3. * x * x - 1) * A.aD;
    const cplx F = fDir - fExc;
    const double triplet = 2. * std::norm(F + 3. * x * (A.a1 + A.a2)) + std::norm(F + 2. * x * (A.a0 + 2. * A.a2)) + 9. * s * s * std::norm(A.a2 - A.a1) + 4. * s * s * std::norm(A.a2 - A.a0);
    return 0.25 * std::norm(fs) + 0.25 * triplet;
}

double PPxsection::Diffxs_CM(double Tlab, double theta) const{return Diffxs(NuclearAmplitudes(Tlab), theta);}

std::vector<double> PPxsection::Cumulative(double Tlab) const
{
    const Amplitudes A = NuclearAmplitudes(Tlab);
    std::vector<double> cdf(kNW, 0.);
    double prev = 0.;
    for (int j = 0; j < kNW; ++j)
    {
        const double omc = std::exp(fWmin + j * fDW);
        const double pdf = Diffxs(A, std::acos(1. - omc)) * CLHEP::twopi * omc;
        if (j > 0) cdf[j] = cdf[j - 1] + 0.5 * (pdf + prev) * fDW;
        prev = pdf;
    }
    return cdf;
}

double PPxsection::Totalxs(double Tlab) const
{
    if (Tlab < kMinE || Tlab > kMaxE) return 0.;
    return Cumulative(Tlab).back();
}

double PPxsection::SampleCosTheta_CM(double Tlab, CLHEP::HepRandomEngine* engine) const
{
    const std::vector<double> cdf = Cumulative(Tlab);
    const double target = engine->flat() * cdf.back();
    const int j = std::clamp(int(std::upper_bound(cdf.begin(), cdf.end(), target) - cdf.begin()) - 1, 0, kNW - 2);
    const double g = (target - cdf[j]) / (cdf[j + 1] - cdf[j]);
    const double w = fWmin + (j + g) * fDW;
    return 1. - std::exp(w);
}