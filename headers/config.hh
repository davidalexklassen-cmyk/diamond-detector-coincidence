#ifndef CONFIG_HH
#define CONFIG_HH
#include "G4SystemOfUnits.hh"
#include "globals.hh"

namespace cfg
{
    inline bool pcb_3x3 = false; //false = 5x5 pcb
    inline G4String foilMat = "G4_POLYETHYLENE";
    inline G4double foilThickness = 10 * um;
    inline G4String filename = "ppCH2.root";
}
#endif