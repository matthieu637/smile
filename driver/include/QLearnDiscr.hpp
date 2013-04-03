
#ifndef QLEARNDISCR_HPP
#define QLEARNDISCR_HPP

#include "Driver.hpp"
#include <sml/Action.hpp>
#include <sml/Q.hpp>

using sml::DAction;
using sml::DState;
using sml::QTable;

class State;

class QLearnDiscr : public Driver
{

public:
    QLearnDiscr(int index);
    void decision();
    ~QLearnDiscr();
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
    static const int ACTIONS_DIRECTION = 8;
    
    const float lrate = 0.01;
    const float discount = 0.35;
    const float espilon = 0.01;
    
    static const sml::ActionTemplate ACTION_TEMPLATE;
    static const sml::StateTemplate STATE_TEMPLATE;

    QTable Q;
    
    bool init = false;
    DState* lastState;
    DAction* lastAction;
};

#endif // QLEARNDISCR_HPP
