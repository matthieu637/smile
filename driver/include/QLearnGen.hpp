
#ifndef QLEARNGEN_HPP
#define QLEARNGEN_HPP

#include "Driver.hpp"
#include <sml/Action.hpp>
#include <sml/QLearnGradient.hpp>
#include <sml/Q.hpp>
#include "TWorld.hpp"

using sml::DAction;
using sml::DState;
using sml::QTable;
using sml::QLearnGradient;

class QLearnGen : public Driver
{

public:
    QLearnGen(int index);
    void decision();
    ~QLearnGen();
    void newRace(tCarElt* car, tSituation *s);
    void endRace();

private:
  void applyActionOn(const DAction& ac, tCarElt* car);
    
private:
    static const int DECISION_EACH = 4;

    static const int ACTIONS_ACC = 7;
    static const int ACTIONS_DIRECTION = 10;
    
    const double lamda = 0.9;
    const double lrate = 0.01;
    const double discount = 0.35;
    const double epsilon = 0.1;

    static const sml::ActionTemplate ACTION_TEMPLATE;
    
    QLearnGradient<State>* qlg;
};

#endif // QLEARNDISCR_HPP
