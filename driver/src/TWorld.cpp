#include "TWorld.hpp"
#include <Driver.hpp>

#include <car.h>
#include <bib/Logger.hpp>
#include <sml/Utils.hpp>

double TWorld::reward(const Driver& d) {
    double distanceMilieu = 10. - sml::Utils::abs( d.getCar()->_trkPos.toMiddle );
    double distParcourue = d.getCoveredDistance();
    double dammageGet = d.getDamageGet();

    double r = distanceMilieu*(distParcourue);
    if (r < 1 )
        r = -10;
    if(d.isStuck() && d.getCar()->ctrl.gear == -1 && d.getCar()->ctrl.accelCmd > 0.2 && d.getCar()->ctrl.brakeCmd < 0.1)
        r = 15;
    else if(distParcourue < 0.1) {
        r = -100;
        if(d.isStuck())
            r -= 350;
        else if(distParcourue < 0)
            r -= 600;
    }
    else if(distanceMilieu < 0. || distParcourue < 0.)
        r = -r;
    else r = r*r/4.;

    r-=dammageGet;


    if(d.getCar()->_trkPos.toRight < 0.5 )
        r -= sml::Utils::abs(10.*d.getCar()->_trkPos.toRight);
    else if (d.getCar()->_trkPos.toLeft < 0.5)
        r -= sml::Utils::abs(10.*d.getCar()->_trkPos.toLeft) ;


    if(r > 0 && !d.isStuck() && d.getCar()->_trkPos.toRight > 0.5 &&  d.getCar()->_trkPos.toLeft > 0.5) //not out or stuck
        if((d.getAngle() < M_PI/8 && d.getAngle() >= 0) || (d.getAngle() > -M_PI/8 && d.getAngle() <= 0 ))
            r += 40;

//     LOG_DEBUG("recompense " << distanceMilieu << " "<< distParcourue << " " << r << " -- "  << d.getCar()->_distFromStartLine << " ");

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

