
#ifndef QLEARNDISCR_HPP
#define QLEARNDISCR_HPP

#include "Driver.hpp"

class QLearnDiscr : public Driver
{

public:
    QLearnDiscr(int index);
    void decision();
    ~QLearnDiscr();
    void newRace(tCarElt* car, tSituation *s);

private:
  struct State {
    float alpha; //direction
    float distance;
  };

  struct DState {
      int alpha;
      int distance;
  };

  struct DAction {
      int acc;
      int direc;
  };
  
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
    
    DState lastState={-1,-1};
    DAction lastAction;
    
    const float lrate = 0.1;
    const float discount = 0.35;
    const float espilon = 0.03;
};

#endif // QLEARNDISCR_HPP
