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

struct LearnReturn {
  DAction* ac;
  bool gotGreedy;
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

    virtual DAction* decision(const State& st, bool greedy) = 0;
    virtual void clear_history(const State& s, const DAction& a) = 0;
    virtual void should_done(const State& s, const DAction& a) = 0;
    virtual void should_do(const State& s, const DAction& a, double reward) = 0;
    virtual void had_choosed(const State& s, const DAction& a, double reward, bool) = 0;
    virtual LearnReturn _learn(const State& s, double reward) = 0;
    DAction* learn(const State& s, double reward){
	return _learn(s, reward).ac;
    }
    
    virtual Policy<State>* copyPolicy() = 0;
protected:
    StrategyEffectsAdvice adviceStrat;
    RLParam param;
};

typedef Policy<DState> DPolicy;

}




#endif // MCARTASK_H
