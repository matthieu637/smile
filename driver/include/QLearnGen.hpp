
#ifndef QLEARNGEN_HPP
#define QLEARNGEN_HPP

#include "Driver.hpp"
#include <sml/Action.hpp>
#include <sml/Q.hpp>
#include "TWorld.hpp"

using sml::DAction;
using sml::DState;
using sml::QTable;

class QLearnGen : public Driver
{

public:
    QLearnGen(int index);
    void decision();
    ~QLearnGen();
    void newRace(tCarElt* car, tSituation *s);

private:
  void applyActionOn(const DAction& ac, tCarElt* car);
    
private:
    static const int DECISION_EACH = 20;

    static const int ACTIONS_ACC = 7;
    static const int ACTIONS_DIRECTION = 12;
    
    const double lamba = 0.2;
    const double lrate = 0.4;
    const double discount = 0.35;
    const double espilon = 0.05;

    static const sml::ActionTemplate ACTION_TEMPLATE;
    
    QTable Q;

    bool init = false;
    State* lastState;
    DAction* lastAction;
};

#endif // QLEARNDISCR_HPP
