
#include "test/MCarQLTable.hpp"


namespace test {

MCarQLTable::MCarQLTable(bool easy) : RLSimulation<MCarState>(new MCar(8,12)), easy(easy) {

}

MCarQLTable::~MCarQLTable() {
    if(easy)
        delete algo1;
    else
        delete algo2;
}

void MCarQLTable::createAgent(const DState& dst, const MCarState& st, const DAction& a) {
    (void) st;
    if(easy)
        algo1 = new QLearning(prob->getStates(), prob->getActions(), dst, a);
    else
        algo2 = new QLearningLamb(prob->getStates(), prob->getActions(), dst, a);
}

void MCarQLTable::resetAgent(const DState& dst, const MCarState& st, const DAction& a) {
    (void) st;

    if(easy)
        algo1->clear_history(dst, a);
    else
        algo2->clear_history(dst, a);
}

DAction* MCarQLTable::step(const DState& dst, const MCarState& st, double reward) {
    (void) st;

    if(easy)
        return algo1->learn(dst, reward, alpha, epsilon, gamma);

    return algo2->learn(dst, reward, alpha, epsilon, gamma, lambda, accumu);
}

int MCarQLTable::local_run() {
    int l = RLSimulation<MCarState>::local_run();

//     algo1->getPolicy().print();

    return l;
}

}