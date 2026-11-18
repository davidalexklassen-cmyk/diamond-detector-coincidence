#ifndef CONFIG_HH
#define CONFIG_HH
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include <chrono>
#include <atomic>

namespace cfg
{
    inline G4long nEvents = 0;
    inline std::atomic<G4long>eventsStarted{};
    inline std::chrono::steady_clock::time_point tStart;
    inline bool pcb_3x3 = false; //false = 5x5 pcb
    inline G4String foilMat = "G4_POLYETHYLENE";
    inline G4double foilThickness = 10 * um;
    inline G4double radius_L = 26.5*cm;
    inline G4double radius_R = 22.5*cm;
    inline G4String filename = "output.root";
}
#endif