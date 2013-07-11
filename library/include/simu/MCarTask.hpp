/*
 *
 */

#ifndef MCARTASK_H
#define MCARTASK_H

#include "simu/MCar.hpp"

// Standard RL parameters:
#define epsilon 0.01                    // probability of random action
#define alpha 0.5                      // step size parameter
#define lambda 0.9                     // trace-decay parameters
#define gamma 0.8                        // discount-rate parameters

namespace simu {

class MCarTask
{

public:
    MCarTask();
    virtual ~MCarTask();
    void run();
    
protected:
    virtual void createAgent(const DState& dst, const MCar::State& st, const DAction& a) = 0;
    virtual DAction* step() = 0;
    //virtual void 
    

protected:
    MCar* prob;
    DAction* fac;
    
};

}

#endif // MCARTASK_H
