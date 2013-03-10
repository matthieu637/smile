
#ifndef QLEARNDISCR2_HPP
#define QLEARNDISCR2_HPP

#include "Driver.hpp"
#include <sml/Action.hpp>
#include <sml/Q.hpp>

using sml::DAction;
using sml::DState;
using sml::QTable;

class QLearnDiscr2 : public Driver
{

public:
    QLearnDiscr2(int index);
    double reward();
    void decision();
    ~QLearnDiscr2();
    void newRace(tCarElt* car, tSituation *s);

private:
  struct State {
    double alpha; //direction
    double distance;
  };
  
  DState* discretize(const State& st);
  void applyActionOn(const DAction& ac, tCarElt* car);
    
private:
    static const int DECISION_EACH = 13;

    static const int STATES_ALPHA = 16;
    static const int STATES_DISTANCE = 12;
    static const int ACTIONS_ACC = 7;
    static const int ACTIONS_DIRECTION = 10;

    static const sml::ActionTemplate ACTION_TEMPLATE;
    static const sml::StateTemplate STATE_TEMPLATE;
    
    
    //double ****Q;//[STATES_ALPHA][STATES_DISTANCE][ACTIONS_ACC][ACTIONS_DIRECTION];
    QTable Q, N;
    
    double lastDist = -1;
    double lastDammage = 0;
    bool init = false;
    DState* lastState;
    const DAction* lastAction;
    
    const double lamba = 0.8;
    const double lrate = 0.2;
    const double discount = 0.2;
    const double espilon = 0.025;
};

#endif // QLEARNDISCR_HPP
