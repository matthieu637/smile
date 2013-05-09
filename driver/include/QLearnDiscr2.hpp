
#ifndef QLEARNDISCR2_HPP
#define QLEARNDISCR2_HPP

///
///\file QLearnDiscr2.hpp
///\brief l'agent qui applique l'algorithme Q-Learning avec l'historique
///
///

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
  
///
///\brief Constructeur
///\param index : numéro de l'agent
    QLearnDiscr2(int index);
 
///
///\brief Fonction à redéfinir pour contrôler la voiture
    void decision();
    
///
///\brief Destructeur    
    ~QLearnDiscr2();
    
    /* callback functions called from TORCS */  
    void newRace(tCarElt* car, tSituation *s);
    void endRace();
    
///
///\brief Retourner l'algorithme d'apprendissage
    sml::LearnStat* getAlgo();
    
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
    static const int DECISION_EACH = 5;

    static const int STATES_ALPHA = 16;
    static const int STATES_DISTANCE = 14;
    static const int ACTIONS_DIRECTION = 10;
    
    const double lambda = 0.95;
    const double lrate = 0.001;
    const double discount = 0.75;
    const double espilon = 0.05;
    
    static const bool learn = true;
    sml::LearnConfig conf = {true, 0.1, 2000};
    static const unsigned int simu_time = 10; //in minutes

    static const sml::ActionTemplate ACTION_TEMPLATE;
    static const sml::StateTemplate STATE_TEMPLATE;
    
    QLearningLamb* q;
};

#endif // QLEARNDISCR_HPP
