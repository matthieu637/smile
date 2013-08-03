#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

#include "sml/Action.hpp"
#include "bib/Logger.hpp"

namespace simu {

using namespace sml;

template <typename State>
class Environnement
{
public:
    virtual DAction* getInitialAction() const = 0;
    virtual double reward() const = 0;
    virtual bool goal() const = 0;
    virtual unsigned int maxStep() const = 0;
    virtual void computeDState(const State& s, DState* dst, const StateTemplate* repr) = 0;
protected:
    virtual void applyOn(const DAction& ac) = 0;
    virtual void initState(bool random=false) = 0;

    
public:
    Environnement(const StateTemplate* st, const ActionTemplate* at):stempl(st), atempl(at) {
        state = new State;
        dstate = new DState(stempl, 0);
    }

    virtual ~Environnement() {
        delete state;
        delete dstate;
        delete atempl;
        delete stempl;
    }
    
    void apply(const DAction& ac){
	applyOn(ac);
	computeDState(*state, dstate, stempl);
    }
    
    void init(bool random=false){
	initState(random);
	computeDState(*state, dstate, stempl);
// 	LOG_DEBUG("reset");
    }

    const State& getState() const {
        return *state;
    }

    const DState& getDState() const {
        return *dstate;
    }

    const sml::ActionTemplate* getActions() const {
        return atempl;
    }
    
    const sml::StateTemplate* getStates() const {
	return stempl;
    }

protected:
    State* state;
    DState* dstate;

    const StateTemplate* stempl;
    const ActionTemplate* atempl;
};

}

#endif // ENVIRONNEMENT_H

