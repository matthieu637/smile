
#ifndef QLEARNGEN_HPP
#define QLEARNGEN_HPP

///
///\file QLearnGen.hpp
///\brief l'agent qui applique l'algorithme Q-Learning avec l'historique par descente de gradient
///
///

#include "Driver.hpp"
#include <sml/Action.hpp>
#include <sml/QLearnGradient.hpp>
#include <sml/Q.hpp>
#include <sml/SarsaGradient.hpp>
#include "TWorld.hpp"

using sml::DAction;
using sml::DState;
using sml::QLearnGradient;

class QLearnGen : public Driver
{

public:
  
///
///\brief Constructeur
///\param index : numéro de l'agent
    QLearnGen(int index);
    
///
///\brief Fonction à redéfinir pour contrôler la voiture
    void decision();
    
///
///\brief Destructeur    
    ~QLearnGen();
    
    /* callback functions called from TORCS */  
    void newRace(tCarElt* car, tSituation *s);
    void endRace();

private:
  
///
///\brief Appliquer l'action donnée sur la voiture
///\param ac : l'action donnée
///	  car : la voiture
    void applyActionOn(const DAction& ac, tCarElt* car);

public:
    static const int DECISION_EACH = 3;

    static const int ACTIONS_DIRECTION = 13;

    static const sml::RLParam conf;
    
    static const double road_width;
    static const double total_angle;

    static const double nbXinter;
    static const double nbYinter;
    
    static const bool learn = true;
    
    static const unsigned int simu_time = 10; //in minutes

    static const sml::ActionTemplate ACTION_TEMPLATE;

//     QLearnGradient<State>* qlg;
    SarsaGradient<State>* qlg;
};

#endif // QLEARNDISCR_HPP

