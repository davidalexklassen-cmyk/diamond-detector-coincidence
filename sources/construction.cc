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

    double pcb_hlength = 15. * mm;
    double pcb_hwidth = 15. * mm;
    G4Box* solidArray = new G4Box("solidArray", pcb_hlength, pcb_hwidth, 500*um);
    G4LogicalVolume* logicArray = new G4LogicalVolume(solidArray, worldMat, "logicArray");

    G4Material* detectorMat = new G4Material("sc_diamond", 6., 12.01*g/mole, 3.515*g/cm3); //molecular mass from wikipedia -> doublecheck
    G4Box* solidDetector = new G4Box("solidDetector", 2.15*mm, 2.15*mm, 25.*um);
    logicDetector = new G4LogicalVolume(solidDetector, detectorMat, "logicDetector");

    G4Material* scatterMatHolder = nist->FindOrBuildMaterial("G4_Al");
    G4VSolid* solidScatter = new G4Box("solidScatter", 5.*mm, 15.*mm, 0.5*mm);
    G4Tubs* solidZylHole = new G4Tubs("solidZylHole", 0, 3.*mm, 1.*mm, 0.*deg, 360. * deg);
    G4Box* solidHoleScatter = new G4Box("solidHoleScatter", 3.*mm, 5.*mm, 1.*mm);

    solidScatter = new G4SubtractionSolid("scatter_hole_1", solidScatter, solidZylHole, nullptr, G4ThreeVector(0., 5*mm, 0.));
    solidScatter = new G4SubtractionSolid("scatter_hole_2", solidScatter, solidZylHole, nullptr, G4ThreeVector(0., -5*mm, 0.));
    solidScatter = new G4SubtractionSolid("scatterWithHoles", solidScatter, solidHoleScatter, nullptr, G4ThreeVector(0., 0., 0.));

    G4LogicalVolume* logicScatter = new G4LogicalVolume(solidScatter, scatterMatHolder, "logicScatterHolder");
    G4VPhysicalVolume* physScatter = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicScatter, "Holder", logicWorld, false, 0, true);

    double foil_hthickness = 0.1*mm; // need to add real, half thickness
    G4Material* scatterMat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
    G4VSolid* solidFoilBody = new G4Box("solidFoilBody", 3.*mm, 5.*mm, foil_hthickness); 
    G4VSolid* solidFoilTop = new G4Tubs("solidFoilTop", 0, 3.*mm, foil_hthickness, 0.*deg, 180. * deg);
    G4VSolid* solidFoilBottom = new G4Tubs("solidFoilBottom", 0, 3.*mm, 0.1*mm, 0.*deg, 180. * deg);
    G4RotationMatrix* mirrorFoilBottom = new G4RotationMatrix();
    mirrorFoilBottom->rotateZ(180*deg);
    G4UnionSolid* pillFoiltop = new G4UnionSolid("union_1", solidFoilBody, solidFoilTop, nullptr, G4ThreeVector(0., 5.*mm, 0.));
    G4UnionSolid* pillFoil = new G4UnionSolid("union_2", pillFoiltop, solidFoilBottom, mirrorFoilBottom, G4ThreeVector(0., -5.*mm, 0.));
    G4LogicalVolume* logicPillFoil = new G4LogicalVolume(pillFoil, scatterMat, "logicPillFoil");
    G4VPhysicalVolume* physFoil = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicPillFoil, "Foil", logicWorld, false, 0, true);

    bool type = true;

    G4Material* pcbMat = nist->FindOrBuildMaterial("G4_BAKELITE");

    double dod_distance_x = 5.5 * mm; //detector on detector distance centre to centre
    double dod_distance_y = 5.5 * mm;

    /*Detector builds up ->
    0 1 2 3 4
    5 6 7 8 9
    */

    G4Box* solidHole = new G4Box("solidHole", 2.15*mm, 2.15*mm, 1*mm);
    G4VSolid* solidPCB = new G4Box("solidPCB", pcb_hlength, pcb_hwidth, 0.5*mm);

    if (type == false)
    {
        for (int ix = 0; ix <5; ix++)
        {
            for(int jx = 0; jx <5; jx++)
            {
                solidPCB = new G4SubtractionSolid("hole_gen", solidPCB, solidHole, nullptr, G4ThreeVector((jx - 2) * dod_distance_x, (ix - 2) * dod_distance_y, 0.));
            }
        }

        G4LogicalVolume* logicPCB = new G4LogicalVolume(solidPCB, pcbMat, "logicPCB");
        new G4PVPlacement(nullptr, G4ThreeVector(0.,0.,0.), logicPCB, "physPCB", logicArray, false, 0, true);

        for (int ix = 0; ix <5; ix++)
        {
            for(int jx = 0; jx <5; jx++)
            {
                new G4PVPlacement(0, G4ThreeVector((jx - 2) * dod_distance_x, (ix - 2) * dod_distance_y, 0.), logicDetector, "solidDetector", logicArray, false, ix * 5 + jx, true);
            }
        }
    }
    else if (type == true)
    {
        for (int ix = 0; ix <3; ix++)
        {
            for(int jx = 0; jx <3; jx++)
            {
                solidPCB = new G4SubtractionSolid("hole_gen", solidPCB, solidHole, nullptr, G4ThreeVector((1 - jx) * dod_distance_x, (ix - 1) * dod_distance_y, 0.));
            }
        }

        G4LogicalVolume* logicPCB = new G4LogicalVolume(solidPCB, pcbMat, "logicPCB");
        new G4PVPlacement(nullptr, G4ThreeVector(0.,0.,0.), logicPCB, "physPCB", logicArray, false, 0, true);

        for (int ix = 0; ix <3; ix++)
        {
            for(int jx = 0; jx <3; jx++)
            {
                new G4PVPlacement(0, G4ThreeVector((1 - jx) * dod_distance_x, (ix - 1) * dod_distance_y, 0.), logicDetector, "solidDetector", logicArray, false, ix*3 + jx, true);
            }
        }
    };

    G4RotationMatrix* rot = new G4RotationMatrix();
    G4RotationMatrix* rot_neg = new G4RotationMatrix();
    double angle = 45.*deg;
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