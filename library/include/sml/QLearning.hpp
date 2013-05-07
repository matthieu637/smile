#ifndef QLEARNING_HPP
#define QLEARNING_HPP
#include "Q.hpp"
#include "LearnStat.hpp"

namespace sml{

class QLearning : public LearnStat
{

public:
    QLearning(const StateTemplate* stmp, const ActionTemplate* atmp, const DState& s, const DAction& a);
    DAction* decision(const DState& s, double r, float lrate, float epsilon, float discount);
protected:
    void save(boost::archive::xml_oarchive* xml);
    void load(boost::archive::xml_iarchive* xml);
private:
    QTable Q;
    const ActionTemplate* atmp;
    const DState* ds;
    const DAction* da;
    
};

}
#endif // QLEARNING_HPP
