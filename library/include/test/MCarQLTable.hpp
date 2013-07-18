#ifndef MCARQLTABLE_HPP
#define MCARQLTABLE_HPP

#include "simu/MCar.hpp"
#include "simu/RLSimulation.hpp"
#include <sml/QLearning.hpp>
#include <sml/QLearningLamb.hpp>

// Standard RL parameters:
#define epsilon 0.05                    // probability of random action
#define alpha 0.08                      // step size parameter
#define lambda 0.9                     // trace-decay parameters
#define gamma 1.                        // discount-rate parameters
#define accumu false

using namespace simu;

namespace test {

class MCarQLTable : public RLSimulation<MCarState, DState, DiscretizeSelection>
{

public:
    MCarQLTable(bool easy=false);

private:
    Policy<DState>* createAgent(const DState& dst, const DAction& a);
    DAction* step(const DState& dst, double reward);

    bool easy;
};

}

#endif
