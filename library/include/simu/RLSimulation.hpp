#ifndef RLSimulation_HPP
#define RLSimulation_HPP

#include "simu/MCar.hpp"
#include "bib/Logger.hpp"
#include <sml/Policy.hpp>

namespace simu {


class DiscretizeSelection {

protected:
    template<typename EnvState>
    const DState& getState(Environnement<EnvState>* env)
    {
        return env->getDState();
    }
};

struct stats {
    int nbStep;
    double total_reward;
    int min_step;
};

template<typename EnvState, typename PolicyState, typename StateType>
class RLSimulation : private StateType
{
    using StateType::getState;

public:


    RLSimulation(Environnement<EnvState>* prob):prob(prob) {
        fac = prob->getInitialAction();
    }

    virtual ~RLSimulation() {
        delete fac;
        delete prob;
        delete agent;
        delete best_policy;
    }

    stats run() {
        agent = this->createAgent(getState(prob), *fac);

        stats s = local_run();
        best_policy = agent->copyPolicy();
        return s;
    }//best_policy not cleared

    std::list<stats>* keepRun(int additional_step) {
        std::list<stats>* stats_history = new std::list<stats>;

        additional_step--;
        prob->init();
        agent->clear_history(getState(prob), *fac);
        stats s = local_run();
        int min_step = s.min_step;
        stats_history->push_back(s);

        delete best_policy;
        best_policy = agent->copyPolicy();

        do {
            additional_step--;
            prob->init();
            agent->clear_history(getState(prob), *fac);
            stats s = local_run();

            if(s.nbStep <= min_step) {
                min_step = s.nbStep;
                delete best_policy;
                best_policy = agent->copyPolicy();
            }
            s.min_step = min_step;
//             LOG_INFO(s.nbStep << " " << s.total_reward << " " << s.min_step << " " << additional_step);
            stats_history->push_back( s);
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
        delete agent;
        agent = this->createAgent(getState(prob), *fac);
        delete best_policy;
        best_policy = agent->copyPolicy();
    }

    virtual Policy<PolicyState>* createAgent(const PolicyState& s, const DAction& a) = 0;
    virtual DAction* computeNextAction(const PolicyState& s, double reward) = 0;


protected:
    virtual stats local_run() {
        DAction* ac = fac;
        int step = 0;
        double total_reward = 0;
        do
        {
            step++;
            prob->apply(*ac);
            total_reward += prob->reward();

            ac = this->computeNextAction(getState(prob), prob->reward());

// 	    LOG_DEBUG(*ac << " " << getState(prob));
        }
        while(!prob->goal() && step < (int)prob->maxStep());

//      LOG(step<< " " << total_reward);
// 	LOG(step);
// 	LOG(total_reward);

        return {step, total_reward, step};
    }

protected:
    Environnement<EnvState>* prob;
    DAction* fac;
    Policy<PolicyState>* agent;
    Policy<PolicyState>* best_policy;
};

}

#endif // MCARTASK_H
