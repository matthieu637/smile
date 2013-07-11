#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

#include "sml/Action.hpp"

namespace simu {

class Environnement
{
public:
    virtual sml::DAction* getInitialAction() const = 0;
    virtual double reward() const = 0;
    virtual const sml::ActionTemplate* getActions() const = 0;

    virtual ~Environnement();
};

}

#endif // ENVIRONNEMENT_H
