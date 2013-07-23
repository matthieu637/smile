#ifndef POLICY_HPP
#define POLICY_HPP

#include "bib/Logger.hpp"
#include "Action.hpp"

namespace sml {

enum StrategyEffectsAdvice {
    None, FixedNExploration, FixedNMax, Max
};

template<class State>
class Policy
{

public:
    Policy():adviceStrat(None) {}
    virtual ~Policy() {}

    void setAdviseStrat(StrategyEffectsAdvice sea) {
        adviceStrat = sea;
    }

    virtual DAction* decision(const State& st, float epsilon=0.L) = 0;
    virtual void clear_history(const State& s, const DAction& a) = 0;
    virtual void should_done(const DState& s, const DAction& a) = 0;
    virtual void should_do(const DState& s, const DAction& a) = 0;
    virtual Policy<State>* copyPolicy() = 0;
protected:
    StrategyEffectsAdvice adviceStrat;
};

typedef Policy<DState> DPolicy;

}




#endif // MCARTASK_H
