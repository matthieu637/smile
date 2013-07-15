
#include "test/MCarQLTableT.hpp"
#include <test/MCarQLTable.hpp>
#include <test/Teacher.hpp>


namespace test {

MCarQLTableT::MCarQLTableT() : RLSimulation<TeacherState>(new Teacher(new MCar(8,12))) {

}

MCarQLTableT::~MCarQLTableT() {
    delete teacher;
}

void MCarQLTableT::createAgent(const DState& dst, const TeacherState& st, const DAction& a) {
    (void) st;
    
    MCarQLTable bestPol(false);
    bestPol.run();
    bestPol.keepRun(2000);
    
    teacher = new QLearningLamb(prob->getStates(), prob->getActions(), dst, a);
}

DAction* MCarQLTableT::step(const DState& dst, const TeacherState& st, double reward) {
    (void) st;
    return teacher->learn(dst, reward, alpha, epsilon, gamma, lambda, accumu);
}

}
