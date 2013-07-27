#ifndef POLICY_HPP
#define POLICY_HPP

#include "bib/Logger.hpp"
#include "Action.hpp"

namespace sml {

struct RLParam {
    float epsilon;
    float alpha;
    float lambda;
    float gamma;
    bool accumu;
};  
  
enum StrategyEffectsAdvice {
    None, FixedNExploration, FixedNMax, Max
};

template<class State>
class Policy
{

public:
    Policy(RLParam param):adviceStrat(None), param(param) {}
    virtual ~Policy() {}

    void setAdviseStrat(StrategyEffectsAdvice sea) {
        adviceStrat = sea;
    }

    virtual DAction* decision(const State& st, float epsilon=0.L) = 0;
    virtual void clear_history(const State& s, const DAction& a) = 0;
    virtual void should_done(const State& s, const DAction& a) = 0;
    virtual void should_do(const State& s, const DAction& a, double reward) = 0;
    virtual DAction* learn(const State& s, double reward) = 0;
    virtual Policy<State>* copyPolicy() = 0;
protected:
    StrategyEffectsAdvice adviceStrat;
    RLParam param;
};

typedef Policy<DState> DPolicy;

}




#endif // MCARTASK_H
