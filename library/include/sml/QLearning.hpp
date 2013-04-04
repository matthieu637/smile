#ifndef QLEARNING_HPP
#define QLEARNING_HPP
#include "Q.hpp"

namespace sml{

class QLearning
{

public:
    QLearning(const StateTemplate* stmp, const ActionTemplate* atmp, const DState& s, const DAction& a);
    DAction* decision(const DState& s, double r, float lrate, float epsilon, float discount);
    void write(const string& arg1);
    void read(const string& arg1);
private:
    QTable Q;
    const ActionTemplate* atmp;
    const DState* ds;
    const DAction* da;
    
};

}
#endif // QLEARNING_HPP
