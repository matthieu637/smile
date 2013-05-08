
#ifndef QLEARNDISCR2_HPP
#define QLEARNDISCR2_HPP

#include "Driver.hpp"
#include <sml/Action.hpp>
#include <sml/QLearningLamb.hpp>
#include "TWorld.hpp"

using sml::DAction;
using sml::DState;
using sml::QLearningLamb;

class QLearnDiscr2 : public Driver
{

public:
    QLearnDiscr2(int index);
    void decision();
    ~QLearnDiscr2();
    void newRace(tCarElt* car, tSituation *s);
    void endRace();
    sml::LearnStat* getAlgo();
    
private:
  DState* discretize(const State& st);
  void applyActionOn(const DAction& ac, tCarElt* car);
    
private:
    static const int DECISION_EACH = 5;

    static const int STATES_ALPHA = 16;
    static const int STATES_DISTANCE = 14;
    static const int ACTIONS_DIRECTION = 10;
    
    const double lambda = 0.95;
    const double lrate = 0.001;
    const double discount = 0.75;
    const double espilon = 0.05;
    
    static const bool learn = false;
    sml::LearnConfig conf = {true, 0.1, 2000};
    static const unsigned int simu_time = 10; //in minutes

    static const sml::ActionTemplate ACTION_TEMPLATE;
    static const sml::StateTemplate STATE_TEMPLATE;
    
    QLearningLamb* q;
};

#endif // QLEARNDISCR_HPP
