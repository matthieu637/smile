#include "TWorld.hpp"
#include <Driver.hpp>

#include <car.h>
#include <bib/Logger.hpp>
#include <sml/Utils.hpp>

double TWorld::reward(const Driver& d) {
    const tCarElt* car = d.getCar();
    double distParcourue = d.getCoveredDistance();

//     double r = distParcourue;
    double r = car->_speed_x;

    const double startMalus = 1000;
    const double malus = startMalus + 500;

    if(d.isStuck() && distParcourue < -0.01)
        r = 15;
    else {
        if(distParcourue > 0.01 && !d.isStuck()) //increase reward if everything's ok r^3
        {
            r+=4; // 4² -> 16 > 15
//             r = r*r*r;//dist
            r=r*r;
        }
        else if(distParcourue < 0.01 && !d.isStuck()) { // don't move don't stuck
            if(distParcourue > -0.01)
                r = - malus;
            else //drive backwards
                r = - malus*2.*(1.+sml::Utils::abs(distParcourue));
        }
        else if(distParcourue < 0.01 && d.isStuck()) // don't move because stuck
            r = -malus*2.;
        else  // move and stuck -> forcing the wall -_-
            r = -malus*3.;

        // keep on the road
        // if near or out of the way
        double limit = 1.5;
        double factor = 5;
        if(car->_trkPos.toRight < limit) {
            if(r>0)
                r=0;

            if(car->_trkPos.toRight > 0)
                r -= startMalus + factor*car->_trkPos.toRight;
            else
                r -= startMalus + factor*(limit + sml::Utils::abs(car->_trkPos.toRight));
        }
        else if (car->_trkPos.toLeft < limit) {
            if(r>0)
                r=0;

            if(car->_trkPos.toLeft > 0)
                r -= startMalus + factor*car->_trkPos.toLeft;
            else
                r -= startMalus + factor*(limit + sml::Utils::abs(car->_trkPos.toLeft));
        }

        // stay in a good position
        double aAgl = sml::Utils::abs(d.getAngle());
        double bonusMax = 100.;
        if(r > 0 && !d.isStuck() && car->_trkPos.toRight >= limit &&  car->_trkPos.toLeft >= limit) { //not out or stuck
            double bonus = bonusMax - sml::Utils::transform(aAgl, 0., M_PI, 0, bonusMax);
            r+= bonus;
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
//     s->stuck = d.isStuck();
    s->angle = d.getAngle();
    s->speed = car->_speed_x;
    s->distanceFromMiddle = car->_trkPos.toMiddle;
    s->leftDistance = car->_trkPos.toLeft;
    s->rightDistance = car->_trkPos.toRight;
    s->straightLength = d.straightLength();

    return s;
}

State* TWorld::initialState() {
    State* s = new State;
//     s->stuck = 0;
    s->angle = 0;
    s->speed = 0;
    s->distanceFromMiddle = -1.8;
    s->leftDistance = 7;
    s->rightDistance = 3.6;
    return s;
}

DAction* TWorld::initialAction(const sml::ActionTemplate* atmp) {
    int d = atmp->indexFor(DIRE);
    list<int> args;

    if(atmp->sizesActions()->size() == 2) {
        int ac = atmp->indexFor(ACC);

        if(d < ac) { //dir before acc
            args.push_back(atmp->sizesActions()->front()/2);
            args.push_back(2);
        }
        else { //acc before dir
            args.push_back(2);
            args.push_back(atmp->sizesActions()->back()/2);
        }
    } // == 1 only DIRE
    else
    {
        args.push_back(atmp->sizesActions()->front()/2);
    }

    DAction* a = new DAction(atmp, args);
    return a;
}

unsigned int TWorld::discretizeAngle(float angle, unsigned int cardinal) {
    return round(sml::Utils::transform(angle, -M_PI, M_PI, 0, (double)(cardinal - 1)));
}

unsigned int TWorld::discretizeDistance(float distance, unsigned int cardinal, double dismin, double dismax) {
    return round(sml::Utils::transform(distance, dismin, dismax, 0, (double)(cardinal - 1)));
}

float TWorld::computeSteering(unsigned int discretized, unsigned int cardinal, double smin, double smax) {
    return smin+((float)discretized/(float)cardinal)*(smax-smin);
}

void TWorld::applyAcceleration(tCarElt* car, int accel) {

    switch(accel)
    {
    case 0:
        car->ctrl.gear = -1;
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = 1;
        break;
    case 1:
        car->ctrl.brakeCmd = 1;
        car->ctrl.accelCmd = 0;
        break;
    case 2:
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = 0;
        break;
    case 3:
        car->ctrl.gear = 1;
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = 1;
        break;
    }
}

void TWorld::applyAccelerationGear(const Driver& d, tCarElt* car, int accel) {

    switch(accel)
    {
    case 0:
        car->ctrl.gear = -1;
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = 1;
        break;
    case 1:
        car->ctrl.brakeCmd = 1;
        car->ctrl.accelCmd = 0;
        break;
    case 2:
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = 0;
        break;
    case 3:
        car->ctrl.gear = d.getGear();
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = 1;
        break;
    }
}
