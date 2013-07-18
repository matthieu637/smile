
#include "test/MCarQLTable.hpp"


namespace test {

MCarQLTable::MCarQLTable(bool easy) : RLSimulation(new MCar(8,12)), easy(easy) {

}

Policy<DState>* MCarQLTable::createAgent(const DState& dst, const DAction& a) {

    if(easy)
        return new QLearning(prob->getStates(), prob->getActions(), dst, a);

    return new QLearningLamb(prob->getStates(), prob->getActions(), dst, a);
}

DAction* MCarQLTable::step(const DState& dst, double reward) {
  
    if(easy)
        return static_cast<QLearning*>(agent)->learn(dst, reward, alpha, epsilon, gamma);

    return static_cast<QLearningLamb*>(agent)->learn(dst, reward, alpha, epsilon, gamma, lambda, accumu);
}


}
