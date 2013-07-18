#ifndef MCARQLTABLET_HPP
#define MCARQLTABLET_HPP

#include "simu/MCar.hpp"
#include "simu/RLSimulation.hpp"
#include <sml/QLearning.hpp>
#include <sml/QLearningLamb.hpp>
#include "Teacher.hpp"
#include <test/MCarQLTable.hpp>

using namespace simu;

namespace test {

class MCarQLTableT : public RLSimulation<TeacherState, DState, DiscretizeSelection>
{

public:
    MCarQLTableT();
    ~MCarQLTableT();

private:
    void createAgent(const DState& dst, const TeacherState& st, const DAction& a);
    void resetAgent(const DState& dst, const TeacherState& st, const DAction& a);
    DAction* step(const DState& dst, const TeacherState& st, double reward);
    DAction* policy(const DState& dst, const TeacherState& st);

    QLearningLamb* teacher;
    MCarQLTable* bestPol;
};

}

#endif
