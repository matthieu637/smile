/*
 *
 */

#ifndef RLSimulation_HPP
#define RLSimulation_HPP

#include "simu/MCar.hpp"
#include "bib/Logger.hpp"

// Standard RL parameters:
#define epsilon 0.01                    // probability of random action
#define alpha 0.5                      // step size parameter
#define lambda 0.9                     // trace-decay parameters
#define gamma 0.8                        // discount-rate parameters

namespace simu {

template<typename State>
class RLSimulation
{

public:
    RLSimulation() {
        prob = new MCar(8,12);
        prob->getActions();
        fac = prob->getInitialAction();;
    }

    virtual ~RLSimulation() {
        delete fac;
        delete prob;
    }

    void run() {
        this->createAgent(prob->getDState(), prob->getState(), *fac);

        DAction* ac;
        int step = 0;
        do
        {
            step++;
            prob->step(*ac);

//    	ac = this->step(prob->getDState(), prob->getState(), prob->reward());
//         ac = ql.learn(prob.getDState(), -1, alpha, epsilon, gamma);

//      LOG_DEBUG("etat " << dst << " action " << *ac << " ");
        }
        while(!prob->goal());

        LOG(step);
    }

protected:
    virtual void createAgent(const DState& dst, const MCarState& st, const DAction& a) = 0;
    virtual DAction* step() = 0;
    //virtual void


protected:
    MCar* prob;
    DAction* fac;

};

}

#endif // MCARTASK_H
