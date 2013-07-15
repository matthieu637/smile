
#include "test/MCarQLTable.hpp"


namespace test {

MCarQLTable::MCarQLTable(bool easy) : RLSimulation<MCarState>(new MCar(8,12)), easy(easy) {

}

MCarQLTable::~MCarQLTable() {
    delete algo1;
    delete algo2;
}

void MCarQLTable::createAgent(const DState& dst, const MCarState& st, const DAction& a) {
    (void) st;
    algo1 = new QLearning(prob->getStates(), prob->getActions(), dst, a);
    algo2 = new QLearningLamb(prob->getStates(), prob->getActions(), dst, a);
}

DAction* MCarQLTable::step(const DState& dst, const MCarState& st, double reward) {
    (void) st;

    if(easy)
        return algo1->learn(dst, reward, alpha, epsilon, gamma);

    return algo2->learn(dst, reward, alpha, epsilon, gamma, lambda, accumu);
}

}
