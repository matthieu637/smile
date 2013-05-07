#ifndef QLEARNINGLAMB_HPP
#define QLEARNINGLAMB_HPP
#include "Q.hpp"

namespace sml {

class HistoryComparator
{
public:
    bool operator()(const std::pair<DState*, DAction*>& p1, const std::pair<DState*, DAction*>& p2) const
    {
        if(*p1.first == *p2.first)
            return *p1.second < *p2.second;
        return *p1.first < *p2.first;
    }
};

class QLearningLamb
{

public:
    QLearningLamb(const StateTemplate* stmp, const ActionTemplate* atmp, DState& s, DAction& a);
    DAction* decision(DState& s, double r, float lrate, float epsilon, float discount, float lambda, bool accumulative);
protected:
    void save(boost::archive::xml_oarchive* xml);
    void load(boost::archive::xml_iarchive* xml);
private:
    QTable Q, N;
    const ActionTemplate* atmp;
    DState* s;
    DAction* a;

    std::set< std::pair<DState*, DAction*>, HistoryComparator > history;
};

}
#endif // QLEARNINGLAMB
