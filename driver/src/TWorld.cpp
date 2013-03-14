#include "TWorld.hpp"
#include <Driver.hpp>

#include <car.h>
#include <bib/Logger.hpp>
#include <sml/Utils.hpp>

double TWorld::reward(const Driver& d) {
    const tCarElt* car = d.getCar();
//     double distanceMilieu = 10. - sml::Utils::abs( car->_trkPos.toMiddle );
    double distParcourue = d.getCoveredDistance();
//     double dammageGet = d.getDamageGet();

    double r = distParcourue;

    if(d.isStuck() && car->ctrl.gear == -1 && car->ctrl.accelCmd > 0.2 && car->ctrl.brakeCmd < 0.1)
        r = 15;
    else if(distParcourue < 0.05) {
        r = -100;
        if(d.isStuck())
            r -= 350;
        else if(distParcourue < 0)
            r -= 600;
    }
    else r = (r+1.)*(r+1.); //increase reward if everything's ok

//     r-=dammageGet;

    // if near or out of the way
    double limit = 1.5;
    if(car->_trkPos.toRight < limit) {
	if(r>0)
	  r=0;
      
        if(car->_trkPos.toRight > limit)
            r -= 10*car->_trkPos.toRight;
        else
            r -= 10*(limit + sml::Utils::abs(car->_trkPos.toRight));
    }
    else if (car->_trkPos.toLeft < limit) {
	if(r>0)
	  r=0;
      
        if(car->_trkPos.toLeft > limit)
            r -= 10*car->_trkPos.toLeft;
        else
            r -= 10*(limit + sml::Utils::abs(car->_trkPos.toLeft));
    }


//     if(r > 0 && !d.isStuck() && car->_trkPos.toRight > 0.5 &&  car->_trkPos.toLeft > 0.5) //not out or stuck
//         if((d.getAngle() < M_PI/8 && d.getAngle() >= 0) || (d.getAngle() > -M_PI/8 && d.getAngle() <= 0 ))
//             r += 40;

//     LOG_DEBUG("recompense "<< r << "\t\t" <<  car->_trkPos.toRight <<"\t\t" <<  distParcourue );

    return r;
}

State* TWorld::observe(const Driver& d) {
    const tCarElt* car = d.getCar();
    State* s = new State;
    s->stuck = d.isStuck();
    s->angle = d.getAngle();
    s->speed = car->_speed_x;
    s->distanceFromMiddle = car->_trkPos.toMiddle;
    s->leftDistance = car->_trkPos.toLeft;
    s->rightDistance = car->_trkPos.toRight;
    return s;
}

unsigned int TWorld::discretizeAngle(float angle, unsigned int cardinal) {
    return round(sml::Utils::transform(angle, -M_PI, M_PI, 0, (double)(cardinal - 1)));
}

unsigned int TWorld::discretizeDistance(float distance, unsigned int cardinal, double dismin, double dismax) {
    return round(sml::Utils::transform(distance, dismin, dismax, 0, (double)(cardinal - 1)));
}

float TWorld::computeSteering(unsigned int discetized, unsigned int cardinal, double smin, double smax) {
    return smin+((float)discetized/(float)cardinal)*(smax-smin);
}

