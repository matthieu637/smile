
#ifndef QLEARNDISCR2_HPP
#define QLEARNDISCR2_HPP

#include "Driver.hpp"
#include "QLearnDiscr.hpp"

class QLearnDiscr2 : public Driver
{

public:
    QLearnDiscr2(int index);
    float reward();
    void decision();
    ~QLearnDiscr2();
    void newRace(tCarElt* car, tSituation *s);

private:
  DState discretize(const State& st);
  DAction bestQVal(const DState& dst);
  void applyActionOn(const DAction& ac, tCarElt* car);
    
private:
    static const int DECISION_EACH = 13;

    static const int STATES_ALPHA = 16;
    static const int STATES_DISTANCE = 12;
    static const int ACTIONS_ACC = 7;
    static const int ACTIONS_DIRECTION = 10;

    float ****Q;//[STATES_ALPHA][STATES_DISTANCE][ACTIONS_ACC][ACTIONS_DIRECTION];
    float ****N;
    
    float lastDist = -1;
    float lastDammage = 0;
    DState lastState={-1,-1};
    DAction lastAction;
    
    const float lamba = 0.8;
    const float lrate = 0.2;
    const float discount = 0.2;
    const float espilon = 0.025;
};

#endif // QLEARNDISCR_HPP
