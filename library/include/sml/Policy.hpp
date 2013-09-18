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
    float initial;
    int tiling;
    
    float ia_threshold;
    float mc_threshold;
};

enum StrategyEffectsAdvice {
    None, InformedExploration, Max, Decreased, Fixed
};

struct LearnReturn {
    DAction* ac;
    bool gotGreedy;
};

template<class State>
class Policy
{

public:
    Policy(RLParam param, StrategyEffectsAdvice sea):adviceStrat(sea), param(param) {}
    virtual ~Policy() {}


    virtual DAction* decision(const State& st, bool greedy) = 0;
    virtual void clear_history(const State& s, const DAction& a) = 0;
    virtual void should_done(const State& s, const DAction& a) = 0;
    virtual void should_do(const State& s, const DAction& a, double reward) = 0;
    virtual void had_choosed(const State& s, const DAction& a, double reward, bool) = 0;
    virtual LearnReturn _learn(const State& s, double reward) = 0;
    virtual float getStateImportance(const State& s) = 0;
    DAction* learn(const State& s, double reward) {
        return _learn(s, reward).ac;
    }

    virtual Policy<State>* copyPolicy() = 0;

    RLParam getParams() {
        return param;
    }

protected:
    StrategyEffectsAdvice adviceStrat;
    RLParam param;
};

typedef Policy<DState> DPolicy;

}




#endif // MCARTASK_H
