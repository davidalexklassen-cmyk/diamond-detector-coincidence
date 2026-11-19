#ifndef CONFIG_HH
#define CONFIG_HH
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include <chrono>
#include <atomic>

namespace cfg
{
    // Parameters to start simulation
    inline G4long nEvents = 0;
    inline bool pcb_3x3 = false; //false = 5x5 pcb
    inline G4String foilMat = "G4_POLYETHYLENE";
    inline G4double foilThickness = 10 * um;
    inline G4String filename = "output.root";

    // Individual Detector Distances
    inline G4double radius_L = 26.5*cm;
    inline G4double radius_R = 22.5*cm;

    // Targetholder type
    inline G4double depth_target = 400 *um;
    inline G4double length_target = 2.7 *cm;
    inline G4double width_target = 1.25 *cm;
    inline G4double foil_width = 9.1 *mm;
    inline G4double foil_length =  3.1*mm;

    // Detector Layer
    inline G4double pcb_width = 30. *mm;
    inline G4double pcb_length =  30. *mm;   

    // Beam spread. 1 equals 1mm Gaussian spread
    inline G4double scalar_y = 0.2;
    inline G4double scalar_x = 1;

    // clock
    inline std::atomic<G4long>eventsStarted{};
    inline std::chrono::steady_clock::time_point tStart;

}
#endif