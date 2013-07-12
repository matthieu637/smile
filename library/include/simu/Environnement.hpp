#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

#include "sml/Action.hpp"

namespace simu {

template <typename State>
class Environnement
{
public:
    virtual sml::DAction* getInitialAction() const = 0;
    virtual double reward() const = 0;
    virtual const sml::ActionTemplate* getActions() const = 0;
    virtual bool goal() const = 0;
    virtual unsigned int maxStep() const = 0;

    Environnement(){
	state = new State;
// 	dstate = new DState();
    }
    
    virtual ~Environnement(){
      delete state;
      delete dstate;
    }
    
    const State& getState() const{
	return *state;
    }
    
    const sml::DState& getDState() const{
	return *dstate;
    }
    
protected:
    State* state;
    sml::DState* dstate;
};

}

#endif // ENVIRONNEMENT_H
