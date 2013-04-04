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

    if(d.isStuck() && distParcourue < -0.01)
        r = 15;
    else {
        if(distParcourue > 0.01 && !d.isStuck()) //increase reward if everything's ok r^3
        {
            r+=4; // 4² -> 16 > 15
            r = r*r*r;
        }
        else if(distParcourue < 0.01 && !d.isStuck()) { // don't move don't stuck
            if(distParcourue > -0.01)
                r = -500.;
            else //drive backwards
                r = -2000.*(1.+sml::Utils::abs(distParcourue));
        }
        else if(distParcourue < 0.01 && d.isStuck()) // don't move because stuck
            r = -1000.;
        else  // move and stuck -> forcing the wall -_-
            r = -2000.;

//     r-=dammageGet;

        // keep on the road
        // if near or out of the way
        double limit = 1.5;
        double factor = 30;
        if(car->_trkPos.toRight < limit) {
            if(r>0)
                r=0;

            if(car->_trkPos.toRight > 0)
                r -= factor*car->_trkPos.toRight;
            else
                r -= factor*(limit + sml::Utils::abs(car->_trkPos.toRight));
        }
        else if (car->_trkPos.toLeft < limit) {
            if(r>0)
                r=0;

            if(car->_trkPos.toLeft > 0)
                r -= factor*car->_trkPos.toLeft;
            else
                r -= factor*(limit + sml::Utils::abs(car->_trkPos.toLeft));
        }

        // stay in a good position
//     double aLimit = M_PI/6.;
        double aAgl = sml::Utils::abs(d.getAngle());
        double bonusMax = 100.;
        if(r > 0 && !d.isStuck() && car->_trkPos.toRight >= limit &&  car->_trkPos.toLeft >= limit) { //not out or stuck

//         if(aAgl < aLimit) {
            double bonus = bonusMax - sml::Utils::transform(aAgl, 0., M_PI, 0, bonusMax);
            r+= bonus;
//         }
        } else {
            //TODO: come on the race
//             double malus = sml::Utils::transform(aAgl, 0., M_PI, 0, bonusMax);
//             r -= malus;
        }

    }




//      LOG_DEBUG("recompense "<< r << "\t\t" <<  car->_trkPos.toRight <<"\t\t" <<  distParcourue << "\t" << d.getAngle()  );

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

State* TWorld::initialState() {
    State* s = new State;
    s->stuck = 0;
    s->angle = 0;
    s->speed = 0;
    s->distanceFromMiddle = -1.8;
    s->leftDistance = 7;
    s->rightDistance = 3.6;
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

