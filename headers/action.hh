#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"
#include "generator.hh"
#include "run.hh"
#include "rutherford.hh"

class ActionInitialization : public G4VUserActionInitialization
{
    public:
        ActionInitialization();
        ~ActionInitialization();

        void BuildForMaster() const override;
        void Build() const override;
};
#endif
