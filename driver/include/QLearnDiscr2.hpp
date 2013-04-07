
#ifndef QLEARNDISCR2_HPP
#define QLEARNDISCR2_HPP

#include "Driver.hpp"
#include <sml/Action.hpp>
#include <sml/Q.hpp>
#include "TWorld.hpp"

using sml::DAction;
using sml::DState;
using sml::QTable;

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

class QLearnDiscr2 : public Driver
{

public:
    QLearnDiscr2(int index);
    void decision();
    ~QLearnDiscr2();
    void newRace(tCarElt* car, tSituation *s);
    void endRace();

private:
  DState* discretize(const State& st);
  void applyActionOn(const DAction& ac, tCarElt* car);
    
private:
    static const int DECISION_EACH = 5;

    static const int STATES_ALPHA = 16;
    static const int STATES_DISTANCE = 7;
    static const int ACTIONS_DIRECTION = 10;
    
    const double lamba = 0.95;
    const double lrate = 0.001;
    const double discount = 0.75;
    const double espilon = 0.01;

    static const sml::ActionTemplate ACTION_TEMPLATE;
    static const sml::StateTemplate STATE_TEMPLATE;
    
    
    QTable Q, N;
    std::set< std::pair<DState*, DAction*>, HistoryComparator > history;

    bool init = false;
    DState* lastState;
    DAction* lastAction;
};

#endif // QLEARNDISCR_HPP
