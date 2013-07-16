#ifndef RLSimulation_HPP
#define RLSimulation_HPP

#include "simu/MCar.hpp"
#include "bib/Logger.hpp"

namespace simu {

template<typename State>
class RLSimulation
{

public:
    RLSimulation(Environnement<State>* prob):prob(prob) {
        fac = prob->getInitialAction();;
    }

    virtual ~RLSimulation() {
        delete fac;
        delete prob;
    }

    int run() {
        this->createAgent(prob->getDState(), prob->getState(), *fac);

	return local_run();
    }

    int keepRun(int additional_step) {
      
	prob->init();
	this->resetAgent(prob->getDState(), prob->getState(), *fac);
	int min_step = local_run();
	
	do{
	    additional_step--;
	    prob->init();
	    this->resetAgent(prob->getDState(), prob->getState(), *fac);
	    int score = local_run();
	    if(score < min_step)
	      min_step = score;
	}
	while(additional_step > 0);
	
	LOG(min_step);
	
	return min_step;
    }
    
protected:
    virtual int local_run(){
      	DAction* ac = fac;
        int step = 0;
        double total_reward = 0;
        do
        {
            step++;
            prob->apply(*ac);
            total_reward += prob->reward();

            ac = this->step(prob->getDState(), prob->getState(), prob->reward());
        }
        while(!prob->goal() && step < (int)prob->maxStep());

        LOG(step<< " " << total_reward);
	
	return step;
    }

protected:
    virtual void createAgent(const DState& dst, const State& st, const DAction& a) = 0;
    virtual void resetAgent(const DState& dst, const State& st, const DAction& a) = 0;
    virtual DAction* step(const DState& dst, const State& st, double reward) = 0;

protected:
    Environnement<State>* prob;
    DAction* fac;
};

}

#endif // MCARTASK_H
