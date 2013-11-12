#ifndef ENVIRONNEMENT_H
#define ENVIRONNEMENT_H

#include "sml/Action.hpp"
#include "bib/Logger.hpp"

namespace simu {

using namespace sml;

struct RAction{
  bool restricted;
  list<int>* possible_actions;
};

template <typename State>
class Environnement
{
public:
    virtual DAction* getInitialAction() const = 0;
    virtual double reward() const = 0;
    virtual bool goal() const = 0;
    virtual unsigned int maxStep() const = 0;
    virtual void computeDState(const State& s, DState* dst, const StateTemplate* repr) = 0;
    virtual RAction restrictedAction(){
	return {false, nullptr};
    }
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

    void apply(const DAction& ac) {
        applyOn(ac);
        computeDState(*state, dstate, stempl);
        step++;
    }

    void init(bool random=false) {
        step = 0;
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

    bool done() const {
        return goal() || step >= maxStep();
    }

protected:
    State* state;
    DState* dstate;

    const StateTemplate* stempl;
    const ActionTemplate* atempl;

public:
    int step;
};

}

#endif // ENVIRONNEMENT_H

