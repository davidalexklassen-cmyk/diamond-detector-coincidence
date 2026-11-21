// ./sim DETECTOR_TYPE_(3|5) G4_FOILMAT_NIST, FOIL_THICKNESS_um, 

#include <chrono>
#include <iostream>


#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "construction.hh"
#include "physics.hh"
#include "action.hh"
#include "config.hh"

int main(int argc, char** argv)
{
    if(argc != 8)
    {
        G4cerr << "Insufficient arguments: ./sim <3_or_5> <foil_material> <foil_thickness_um> <n_events> <output_name.root> <biasing> <beamEnergy>" << G4endl;
        return 1;
    }
    cfg::pcb_3x3 = std::string(argv[1]) == "3";
    cfg::foilMat = argv[2];
    cfg::foilThickness = std::stod(argv[3]) * um;
    cfg::filename = argv[5];
    cfg::nEvents = (G4long)::std::stod(argv[4]);
    cfg::tStart = std::chrono::steady_clock::now();
    cfg::bias_active = std::string(argv[6]) == "ON";
    cfg::beamEnergy = std::stod(argv[7]) * MeV;

    G4cout << "Biasing:" << (cfg::bias_active ? "ON" : "OFF") << G4endl;

    G4MTRunManager* runManager = new G4MTRunManager();
    runManager->SetNumberOfThreads(10);

    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInitialization());
    runManager->Initialize();
    runManager->BeamOn(std::stod(argv[4]));

    /*
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    UImanager->ApplyCommand("/vis/open OGL");
    UImanager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");
    UImanager->ApplyCommand("/vis/drawVolume");
    UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
    UImanager->ApplyCommand("/vis/scene/add/trajectories smooth");
    UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");

    ui->SessionStart();
    */
    delete runManager;
    return 0;
}