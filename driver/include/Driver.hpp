#ifndef _DRIVER_HPP_
#define _DRIVER_HPP_

///
///\file Driver.hpp
///\brief un agent abstrait conducteur
///
///

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>

#include <track.h>
#include <car.h>
#include <robottools.h>
#include <robot.h>
#include <sml/LearnStat.hpp>

class Driver {
public:
  
///
///\brief Constructeur
///\param index : numéro de l'agent
/// 	  intervalAction : la fréquence d'action de l'agent
///	  simu_time : le temps maximal pour un simulation
    Driver(int index, int intervalAction, float simu_time);
    
///
///\brief Destructeur
    virtual ~Driver();

    /* callback functions called from TORCS */
    void initTrack(tTrack* t, void *carHandle, void **carParmHandle, tSituation *s);
    virtual void newRace(tCarElt* car, tSituation *s);
    virtual void drive(tSituation *s);
    virtual void endRace();

///
///\brief Fonction à redéfinir pour contrôler la voiture
    virtual void decision() = 0;
    
///
///\brief Retourner la voiture    
    const tCarElt* getCar() const;
    
///
///\brief Retourner les dommages réçus par la voiture 
    double getDamageGet() const;

///
///\brief Retourner la distance parcourue par la voiture depuis le dernier instant 
    double getCoveredDistance() const;

///
///\brief Retourner la tangente de la voiture à la route
    float getAngle() const;

    /* utility functions */
///
///\brief Retourner la voiture est bloquée ou non
    bool isStuck() const;

///
///\brief Retourner la boîte de vitesse de la voiture
    int getGear() const;
    
///
///\brief Retourner la distance au prochian segment
    float getDistToSegEnd() const;
///
///\brief Retourner la longeur de la route droite
    std::pair<float, float> straightLength() const;
    
protected:
  
    virtual void update(tSituation *s);
    
///
///\brief Retourner l'algorithme d'apprendissage
    virtual sml::LearnStat* getAlgo() = 0;
    
    double reward;
    double globalReward;
    /* per robot global data */
    int stuck;
    int decision_each;
    float angle;			/* the angle of the car relative to the current segment */
    tCarElt *car;			/* pointer to tCarElt struct */
    float lastDistance = -1;
    float lastDammage = 0;

private:
    
    void updateStuck();
  
    float simu_time;
    
    /* data that should stay constant after first initialization */
    int MAX_UNSTUCK_COUNT;
    int INDEX;
    int INTERVAL_ACTION;

    /* class constants */
    static const float MAX_UNSTUCK_ANGLE;
    static const float MAX_UNSTUCK_SPEED;
    static const float MIN_UNSTUCK_DIST;
    static const float UNSTUCK_TIME_LIMIT;

    static const float SHIFT;
    static const float SHIFT_MARGIN;

    /* track variables */
    tTrack* track;
};

#endif // _DRIVER_H_

