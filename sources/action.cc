#include "action.hh"

ActionInitialization::ActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction());
}

void ActionInitialization::Build() const
{
    PrimaryGenerator* generator = new PrimaryGenerator();
    SetUserAction(generator);

    SetUserAction(new RunAction());
}