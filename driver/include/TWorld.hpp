#ifndef TWORLD_HPP
#define TWORLD_HPP

///
///\file TWorld.hpp
///\brief Definir des fonctions de récompenses, de discrètisation, l'état
/// 	  Le monde de TORCS
///

#define STK "stuck"
#define AGL "angle"
#define DST "distanceFromMiddle"
#define LDST "leftDistance"
#define RDST "rightDistance"
#define SPD "speed"

#define ACC "acceleration"
#define DIRE "direction"

#include <sml/Action.hpp>
#include "car.h"

class Driver;
using sml::DAction;
using sml::ActionTemplate;

///
///\struct State
///\brief l'état de la voiture 
struct State {
//     bool stuck;  // 0 1
    float angle; // -pi / pi
    float distanceFromMiddle; // -20 / 20
    float leftDistance; // -4 / 9
    float rightDistance;
    float speed; //-40 / 40 ?
    float straightLength; // -3 / 300
    float nextArc; // -0.2 // 0.2
};


class TWorld
{

public:
  
///
///\brief Retrourner l'action initiale
///\param atmp : le modèle d'action
    static DAction* initialAction(const sml::ActionTemplate* atmp); 

///
///\brief Retrourner l'état initial   
    static State* initialState();

///
///\brief Retrourner l'état actuel
///\param d : l'agent
    static State* observe(const Driver& d);
    
///
///\brief Retrourner la récompense actuelle
///\param d : l'agent
    static double reward(const Driver& d);

///
///\brief Retrourner un entier qui représente l'angle discrètisé
///\param angle : l'angle donné
///	  cardinal :le nombre d'intervalles   
    static unsigned int discretizeAngle(float angle, unsigned int cardinal);

///
///\brief Retrourner un entier qui représente la distance discrètisée
///\param distance : la distance donnée
///	  cardinal : le nombre d'intervalles 
///	  dismin : la distance appartient à l'intervalle minimal
/// 	  dismax : la distance appartient à l'intervalle maximal
    static unsigned int discretizeDistance(float distance, unsigned int cardinal, double dismin, double dismax);

///
///\brief Retrourner une direction entre smin et smax
///\param discretized :la valeur discrètisée
///	  cardinal : le nombre d'intervalles 
///	  smin : la direction minimale
/// 	  smax : la direction maximale
    static float computeSteering(unsigned int discretized, unsigned int cardinal, double smin, double smax);
    
///
///\brief Appliquer l'acceleration discrètisée de la voiture
///\param car : la voiture
///	  acc : l'acceleration discrètisée 
    static void applyAcceleration(tCarElt* car, int acc);
    
///
///\brief Appliquer l'acceleration discrètisée de la voiture et la boîte de vitesse automatiquement
///\param d : l'agent 
///	  car : la voiture
///	  acc : l'acceleration discrètisée    
    static void applyAccelerationGear(const Driver& d, tCarElt* car, int acc);
    
    static const int ACTIONS_ACC = 4;
};

#endif // TWORLD_HPP
