#ifndef MCARQLTABLE_HPP
#define MCARQLTABLE_HPP

#include "simu/MCar.hpp"
#include "simu/RLSimulation.hpp"
#include <sml/QLearning.hpp>
#include <sml/QLearningLamb.hpp>

// Standard RL parameters:
#define epsilon 0.01                    // probability of random action
#define alpha 0.5                      // step size parameter
#define lambda 0.9                     // trace-decay parameters
#define gamma 0.75                        // discount-rate parameters
#define accumu false

using namespace simu;

namespace test {

class MCarQLTable : public RLSimulation<MCarState>
{

public:
    MCarQLTable(bool easy=false);
    ~MCarQLTable();

private:
    void createAgent(const DState& dst, const MCarState& st, const DAction& a);
    void resetAgent(const DState& dst, const MCarState& st, const DAction& a);
    DAction* step(const DState& dst, const MCarState& st, double reward);

    int local_run();
    
    QLearning* algo1;
    QLearningLamb* algo2;
    bool easy;
};

}

#endif
