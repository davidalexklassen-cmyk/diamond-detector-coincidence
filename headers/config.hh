#ifndef CONFIG_HH
#define CONFIG_HH
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include <chrono>
#include <atomic>

namespace cfg
{
    //biasing
    inline G4bool bias_active = false;
    inline G4double thetaMin = 38 * deg;
    inline G4double thetaMax = 52 * deg;
    inline G4double dPhi = 6 * deg;

    // Parameters to start simulation
    inline G4double beamEnergy = 6. * MeV;
    inline G4long nEvents = 0;
    inline bool pcb_3x3 = false; //false = 5x5 pcb
    inline G4String foilMat = "G4_POLYETHYLENE";
    inline G4double foilThickness = 10 * um;
    inline G4String filename = "output.root";

    // Detector Layer dimension
    inline G4double pcb_width = 30. *mm;
    inline G4double pcb_length =  30. *mm;
    inline G4double pcb_thickness =  0.5 *mm;

    // Detector config
    inline G4double radius_L = 26.5 *cm;
    inline G4double radius_R = 22.5 *cm;
    inline G4double active_diamond_detector_width = 4.3*mm;
    inline G4double active_diamond_detector_length = 4.3*mm;
    inline G4double active_diamond_detector_thickness = 500 *um;

    inline G4int layer_n_stacked = 1;
    inline G4double layer_spacing = 1.*cm;

    // Target and holder
    inline G4double depth_target = 400 *um;
    inline G4double length_target = 12.5 *mm;
    inline G4double width_target = 27 *mm;
    inline G4double foil_width = 12.2 *mm;
    inline G4double foil_length = 9.1 *mm;

    // Beam spread. 1 equals 1mm Gaussian spread
    inline G4double scalar_y = 0.2;
    inline G4double scalar_x = 1;

    // clock
    inline std::atomic<G4long>eventsStarted{};
    inline std::chrono::steady_clock::time_point tStart;

}
#endif