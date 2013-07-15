#ifndef MCARQLTABLET_HPP
#define MCARQLTABLET_HPP

#include "simu/MCar.hpp"
#include "simu/RLSimulation.hpp"
#include <sml/QLearning.hpp>
#include <sml/QLearningLamb.hpp>
#include "Teacher.hpp"

// Standard RL parameters:
#define epsilon 0.01                    // probability of random action
#define alpha 0.5                      // step size parameter
#define lambda 0.9                     // trace-decay parameters
#define gamma 0.8                        // discount-rate parameters
#define accumu false

using namespace simu;

namespace test {

class MCarQLTableT : public RLSimulation<TeacherState>
{

public:
    MCarQLTableT();
    ~MCarQLTableT();

private:
    void createAgent(const DState& dst, const TeacherState& st, const DAction& a);
    DAction* step(const DState& dst, const TeacherState& st, double reward);

    QLearningLamb* teacher;
};

}

#endif
