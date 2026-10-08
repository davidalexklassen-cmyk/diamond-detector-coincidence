#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"   // base class for constructing everything
#include "G4VPhysicalVolume.hh" // important for world boundary and volume
#include "G4LogicalVolume.hh"   // detector volume
#include "G4PVPlacement.hh"     // placing 3d env
#include "G4box.hh"             // shape of world
#include "G4SubtractionSolid.hh"
#include "G4NistManager.hh"     // material database
#include "G4SystemOfUnits.hh"   // SI Units
#include "detector.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    ~DetectorConstruction();

    G4VPhysicalVolume* Construct() override;

private:
    G4LogicalVolume* logicDetector = nullptr;
    void ConstructSDandField() override;
};
#endif