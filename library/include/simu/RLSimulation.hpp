#ifndef RLSimulation_HPP
#define RLSimulation_HPP

#include "simu/MCar.hpp"
#include "bib/Logger.hpp"
#include <sml/Policy.hpp>

namespace simu {


class DiscretizeSelection {
protected:
    template<typename EnvState>
    const DState& getState(Environnement<EnvState>* env) const
    {
        return env->getDState();
    }
};

class ContinuousSelection {
protected:
    template<typename EnvState>
    const EnvState& getState(Environnement<EnvState>* env) const
    {
        return env->getState();
    }
};


struct stats {
    int nbStep;
    double total_reward;
    int min_step;
    int index_min;
};

class Simulation {
public:
    virtual ~Simulation() {}
    virtual void init() = 0;
    virtual stats run(bool random=false) = 0;
    virtual std::list<stats>* keepRun(int additional_step, bool random_init=false) = 0;
};

template<typename EnvState, typename PolicyState, typename StateType>
class RLSimulation : private StateType, public Simulation
{
    using StateType::getState;

public:

    RLSimulation(Environnement<EnvState>* prob):prob(prob),agentSet(false),bestPolicySet(false) {
        fac = prob->getInitialAction();
    }

    virtual ~RLSimulation() {
        delete fac;
        if(agentSet)
            delete agent;
        if(bestPolicySet)
            delete best_policy;
    }

    void init() {
        agent = this->createAgent(getState(prob), *fac);
        agentSet = true;
    }

    stats run(bool random=false) {
        assert(agentSet);

        prob->init(random);
        stats s = local_run(0);
        best_policy = agent->copyPolicy();
        bestPolicySet = true;
        return s;
    }

    std::list<stats>* keepRun(int additional_step, bool random_init=false) {
        assert(agentSet);
        std::list<stats>* stats_history = new std::list<stats>;

        additional_step--;
        prob->init(random_init);
        agent->clear_history(getState(prob), *fac);
        stats s = local_run(0);
        int min_step = s.min_step;
        int index_min = 0;
        int index = 1;
        stats_history->push_back(s);

        if(bestPolicySet)
            delete best_policy;
        best_policy = agent->copyPolicy();

        do {
            additional_step--;
            prob->init(random_init);
            agent->clear_history(getState(prob), *fac);
            stats s = local_run(index);

            if(s.nbStep <= min_step) {
                min_step = s.nbStep;
                index_min = index;
                delete best_policy;
                best_policy = agent->copyPolicy();
            }
            s.min_step = min_step;
            s.index_min = index_min;
//             LOG_INFO(s.nbStep << " " << s.total_reward << " " << s.min_step << " " << additional_step);
            stats_history->push_back( s);
            index++;
        }
        while(additional_step > 0);

        return stats_history;
    }

    Policy<PolicyState>* get_best_policy() {
        return best_policy;
    }

    Policy<PolicyState>* get_policy() {
        return agent;
    }

    Environnement<EnvState>* getEnv() {
        return prob;
    }

    void reset() {
        prob->init();
	if(agentSet)
	  delete agent;
        init();
	
	if(bestPolicySet)
	  delete best_policy;
        bestPolicySet = false;
    }

    virtual Policy<PolicyState>* createAgent(const PolicyState& s, const DAction& a) = 0;

    DAction* computeNextAction(const PolicyState& s, double reward) {
        return agent->learn(s, reward);
    }


protected:
    virtual stats local_run(int index) {
        DAction* ac = fac;
        int step = 0;
        double total_reward = 0;
        do
        {
            step++;
            prob->apply(*ac);
            total_reward += prob->reward();

            ac = this->computeNextAction(getState(prob), prob->reward());

//  	    LOG_DEBUG(*ac << " " << getState(prob));
        }
        while(!prob->goal() && step < (int)prob->maxStep());

//      LOG(step<< " " << total_reward);

        return {step, total_reward, step, index};
    }

protected:
    Environnement<EnvState>* prob;
    DAction* fac;
    Policy<PolicyState>* agent;
    Policy<PolicyState>* best_policy;
    bool agentSet, bestPolicySet;
};

}

#endif // MCARTASK_H
