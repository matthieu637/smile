#ifndef RLSimulation_HPP
#define RLSimulation_HPP

#include "simu/MCar.hpp"
#include "bib/Logger.hpp"
#include <sml/Policy.hpp>

namespace simu {


class DiscretizeSelection{

protected:
  template<typename EnvState> 
  const DState& getState(Environnement<EnvState>* env)
  {
    return env->getDState();
  }
};

  
template<typename EnvState, typename PolicyState, typename StateType>
class RLSimulation : private StateType
{
   using StateType::getState;

public:
    struct stats {
        int nbStep;
        double total_reward;
        int min_step;
    };

    RLSimulation(Environnement<EnvState>* prob):prob(prob) {
        fac = prob->getInitialAction();;
    }

    virtual ~RLSimulation() {
        delete fac;
        delete prob;
        delete agent;
    }

    stats run() {
        agent = this->createAgent(getState(prob), *fac);

        return local_run();
    }

    stats keepRun(int additional_step) {

        prob->init();
        agent->clear_history(getState(prob), *fac);
        stats s = local_run();
        int min_step = s.min_step;
        int nbStep = s.nbStep;
        double total_reward = s.total_reward;

        do {
            additional_step--;
            prob->init();
            agent->clear_history(getState(prob), *fac);
            stats s = local_run();
            min_step = s.min_step;
            nbStep = s.nbStep;
            total_reward = s.total_reward;
            if(nbStep < min_step)
                min_step = nbStep;
        }
        while(additional_step > 0);

        LOG(min_step);

        return {nbStep, total_reward, min_step};
    }

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

            ac = this->step(getState(prob), prob->reward());
        }
        while(!prob->goal() && step < (int)prob->maxStep());

        LOG(step<< " " << total_reward);

        return {step, total_reward, step};
    }

protected:
    virtual Policy<PolicyState>* createAgent(const PolicyState& s, const DAction& a) = 0;
    virtual DAction* step(const PolicyState& s, double reward) = 0;

protected:
    Environnement<EnvState>* prob;
    DAction* fac;
    Policy<PolicyState>* agent;
};

}

#endif // MCARTASK_H
