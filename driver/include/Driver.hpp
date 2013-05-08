#ifndef _DRIVER_HPP_
#define _DRIVER_HPP_

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
#include "linalg.h"
#include <sml/LearnStat.hpp>

class Driver {
public:
    Driver(int index, int intervalAction, float simu_time);
    virtual ~Driver();

    /* callback functions called from TORCS */
    void initTrack(tTrack* t, void *carHandle, void **carParmHandle, tSituation *s);
    virtual void newRace(tCarElt* car, tSituation *s);
    virtual void drive(tSituation *s);
    virtual void endRace();

    virtual void decision() = 0;
    
    const tCarElt* getCar() const;
    double getDamageGet() const;
    double getCoveredDistance() const;
    float getAngle() const;

    /* utility functions */
    bool isStuck() const;
    int getGear() const;
    float getDistToSegEnd() const;
    v2d getTargetPoint() const;
protected:
    virtual void update(tSituation *s);
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
    
    static const float LOOKAHEAD_CONST;
    static const float LOOKAHEAD_FACTOR; 

    /* track variables */
    tTrack* track;
};

#endif // _DRIVER_H_

