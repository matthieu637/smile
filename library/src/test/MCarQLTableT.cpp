
#include "test/MCarQLTableT.hpp"
#include <test/Teacher.hpp>


namespace test {

MCarQLTableT::MCarQLTableT() : RLSimulation(new Teacher(new MCar(8,12))) {

}

MCarQLTableT::~MCarQLTableT() {
    delete teacher;
}

void MCarQLTableT::createAgent(const DState& dst, const TeacherState& st, const DAction& a) {
    (void) st;
    
    bib::Logger::getInstance()->setIgnoredBuffer(true);
     bestPol = new MCarQLTable(false);
     bestPol->run();
    int score = bestPol->keepRun(2000).min_step;
    
    teacher = new QLearningLamb(prob->getStates(), prob->getActions(), dst, a);
}

void MCarQLTableT::resetAgent(const DState& dst, const TeacherState& st, const DAction& a) {
  
}

DAction* MCarQLTableT::step(const DState& dst, const TeacherState& st, double reward) {
    (void) st;
    return teacher->learn(dst, reward, alpha, epsilon, gamma, lambda, true);
}

DAction* MCarQLTableT::policy(const DState& dst, const TeacherState& st) {
    (void) st;
    return teacher->decision(dst);
}

}
