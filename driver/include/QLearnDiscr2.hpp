
#ifndef QLEARNDISCR2_HPP
#define QLEARNDISCR2_HPP

#include "Driver.hpp"
#include <sml/Action.hpp>
#include <sml/Q.hpp>
#include "TWorld.hpp"

using sml::DAction;
using sml::DState;
using sml::QTable;

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
    static const int STATES_DISTANCE = 10;
    static const int ACTIONS_ACC = 7;
    static const int ACTIONS_DIRECTION = 10;
    
    const double lamba = 0.9;
    const double lrate = 0.01;
    const double discount = 0.35;
    const double espilon = 0.01;

    static const sml::ActionTemplate ACTION_TEMPLATE;
    static const sml::StateTemplate STATE_TEMPLATE;
    
    
    QTable Q, N;
    std::list< std::pair<DState*, DAction*> > history;

    bool init = false;
    DState* lastState;
    DAction* lastAction;
};

#endif // QLEARNDISCR_HPP
