
#ifndef QLEARNDISCR_HPP
#define QLEARNDISCR_HPP

///
///\file QLearnDiscr.hpp
///\brief l'agent qui applique l'algorithme Q-Learning
///
///

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
  
///
///\brief Constructeur
///\param index : numéro de l'agent
    QLearnDiscr(int index);
    
///
///\brief Fonction à redéfinir pour contrôler la voiture
    void decision();
    
///
///\brief Destructeur    
    ~QLearnDiscr();
 
    /* callback functions called from TORCS */    
    void newRace(tCarElt* car, tSituation *s);
    void endRace();
    
private:

///
///\brief Retourner l'état discrètisé
///\param st : l'état continu 
  DState* discretize(const State& st);
  
///
///\brief Appliquer l'action donnée sur la voiture
///\param ac : l'action donnée
///	  car : la voiture
  void applyActionOn(const DAction& ac, tCarElt* car);
    
private:
    static const int DECISION_EACH = 10;

    static const int STATES_ALPHA = 16;
    static const int STATES_DISTANCE = 12;
    static const int ACTIONS_DIRECTION = 8;
    
    static const sml::RLParam conf;
    
    static const bool learn = true;
    
    static const unsigned int simu_time = 10; //in minutes
    
    static const sml::ActionTemplate ACTION_TEMPLATE;
    static const sml::StateTemplate STATE_TEMPLATE;

    //Sarsa *q;
    QLearning* q;
};

#endif // QLEARNDISCR_HPP
