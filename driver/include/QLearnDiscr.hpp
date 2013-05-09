
#ifndef QLEARNDISCR_HPP
#define QLEARNDISCR_HPP

#include "Driver.hpp"
#include <sml/Action.hpp>
#include <sml/Q.hpp>
#include <sml/QLearning.hpp>
#include <sml/Sarsa.hpp>

using sml::DAction;
using sml::DState;
using sml::QTable;
using sml::QLearning;
using sml::Sarsa;

class State;

class QLearnDiscr : public Driver
{

public:
    QLearnDiscr(int index);
    void decision();
    ~QLearnDiscr();
    void newRace(tCarElt* car, tSituation *s);
    void endRace();
    sml::LearnStat* getAlgo();
    
private:  
  DState* discretize(const State& st);
  void applyActionOn(const DAction& ac, tCarElt* car);
    
private:
    static const int DECISION_EACH = 10;

    static const int STATES_ALPHA = 16;
    static const int STATES_DISTANCE = 12;
    static const int ACTIONS_DIRECTION = 8;
    
    const float lrate = 0.001;
    const float discount = 0.95;
    const float espilon = 0.01;
    
    static const bool learn = true;
    sml::LearnConfig conf = {true, 0.1, 2000};
    static const unsigned int simu_time = 10; //in minutes
    
    static const sml::ActionTemplate ACTION_TEMPLATE;
    static const sml::StateTemplate STATE_TEMPLATE;

    //Sarsa *q;
    QLearning* q;
};

#endif // QLEARNDISCR_HPP
