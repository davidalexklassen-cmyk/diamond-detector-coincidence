#include "construction.hh"

DetectorConstruction::DetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    G4NistManager* nist = G4NistManager::Instance();
    
    G4Material* worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    G4Box* solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, true);

    double pcb_hlength = 30.*mm;
    double pcb_hwidth = 30.*mm;
    G4Box* solidArray = new G4Box("solidArray", pcb_hlength, pcb_hwidth, 0.5*mm);
    G4LogicalVolume* logicArray = new G4LogicalVolume(solidArray, worldMat, "logicArray");

    G4Material* scatterMat = nist->FindOrBuildMaterial("G4_Au");
    G4Box* solidScatter = new G4Box("solidScatter", 25*mm, 50*mm, 0.15*mm);
    G4LogicalVolume* logicScatter = new G4LogicalVolume(solidScatter, scatterMat, "logicScatter");
    G4VPhysicalVolume* physScatter = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicScatter, "physScatter", logicWorld, false, 0, true);

    G4Material* detectorMat = new G4Material("sc_diamond", 6., 12.01*g/mole, 3.515*g/cm3); //molecular mass from wikipedia -> doublecheck
    G4Box* solidDetector = new G4Box("solidDetector", 5.*mm, 5.*mm, 0.25*mm);
    logicDetector = new G4LogicalVolume(solidDetector, detectorMat, "logicDetector");
    
    G4Box* solidHole = new G4Box("solidHole", 5.*mm, 5.*mm, 0.6*mm);
    G4Material* pcbMat = new G4Material("FR4", 1.86*g/cm3, 4);  //chatted material, placeholder
    G4VSolid* solidPCB = new G4Box("solidPCB", pcb_hlength, pcb_hwidth, 0.5*mm);

    G4Element* elH  = nist->FindOrBuildElement("H");
    G4Element* elC  = nist->FindOrBuildElement("C");
    G4Element* elO  = nist->FindOrBuildElement("O");
    G4Element* elSi = nist->FindOrBuildElement("Si");

    pcbMat->AddElement(elH,  0.03);
    pcbMat->AddElement(elC,  0.28);
    pcbMat->AddElement(elO,  0.49);
    pcbMat->AddElement(elSi, 0.20);

    for (int ix = 0; ix <3; ix++)
    {
        for(int jx = 0; jx <3; jx++)
        {
            new G4SubtractionSolid("PCB_with_holes", solidPCB, solidHole, nullptr, G4ThreeVector((1 - jx) * 15.*mm, (ix - 1) * 15.*mm, 0.));
        }
    }

    G4LogicalVolume* logicPCB = new G4LogicalVolume(solidPCB, pcbMat, "logicPCB");

    for (int ix = 0; ix <3; ix++)
    {
        for(int jx = 0; jx <3; jx++)
        {
            new G4PVPlacement(0, G4ThreeVector((1 - jx) * 15.*mm, (ix - 1) * 15.*mm, 0.), logicDetector, "solidDetector", logicArray, false, jx*3 + ix, true);
        }
    }

    G4RotationMatrix* rot = new G4RotationMatrix();
    G4RotationMatrix* rot_neg = new G4RotationMatrix();
    double angle = 10.*deg;
    rot->rotateY(angle);
    rot_neg->rotateY(-angle);

    for(int array = 0; array < 2; array++)
    {
        double radius = -30.*cm - array * 1.*cm;
        double x_pos_Array = -radius * sin(angle);
        double y_pos_Array = 0.;
        double z_pos_Array = radius * cos(angle);

        new G4PVPlacement(rot, G4ThreeVector(x_pos_Array, y_pos_Array, z_pos_Array), logicArray, "physArray", logicWorld, false, array + 2, true);
        new G4PVPlacement(rot_neg, G4ThreeVector(-x_pos_Array, y_pos_Array, z_pos_Array), logicArray, "physArray", logicWorld, false, array, true);
    }
    return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    SensitiveDetector* sensDet = new SensitiveDetector("SensitiveDetector");
    logicDetector->SetSensitiveDetector(sensDet);
}