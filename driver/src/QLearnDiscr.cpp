#include "QLearnDiscr.hpp"
#include <TWorld.hpp>
#include <sml/SaveLoad.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>

const sml::ActionTemplate QLearnDiscr::ACTION_TEMPLATE = sml::ActionTemplate( {ACC, DIR}, {QLearnDiscr::ACTIONS_ACC, QLearnDiscr::ACTIONS_DIRECTION});
const sml::StateTemplate QLearnDiscr::STATE_TEMPLATE = sml::StateTemplate( {AGL, DST}, {QLearnDiscr::STATES_ALPHA, QLearnDiscr::STATES_DISTANCE}); //, SPD});


QLearnDiscr::QLearnDiscr(int index):Driver(index, DECISION_EACH), Q(&STATE_TEMPLATE, &ACTION_TEMPLATE)
{
//   DAction a(&ACTION_TEMPLATE, {3,2});
//   LOG_DEBUG(a.hash());
//   DAction b(&ACTION_TEMPLATE, a.hash());
//   LOG_DEBUG(b.get(0) << " " << b.get(1));
//   LOG_DEBUG(b.hash());
//   LOG_DEBUG(Q(a, b));
//   LOG_DEBUG(Q(a, b));
//   Q(a, b) = 180;
//   LOG_DEBUG(Q(a, b));
//   LOG_DEBUG(Q(a, DAction(&ACTION_TEMPLATE, {0,0})));
//   LOG_DEBUG(Q(a, DAction(&ACTION_TEMPLATE, {0,1})));
//   LOG_DEBUG(Q.argmax(a)->get(0) << " " << Q.argmax(a)->get(1));
//   exit(1);
}

QLearnDiscr::~QLearnDiscr()
{
    Q.write("smile0.data");
}

void QLearnDiscr::decision()
{
    DAction a = *lastAction;
    DState s = *lastState;
    double r = TWorld::reward(*this);

    State st = *TWorld::observe(*this);
    DState* Psp = discretize(st);
    DState sp = *Psp;
    DAction* Pap = Q.argmax(sp);
    DAction ap = *Pap;

    //update Q value with lastState
    if(init) {
// 	LOG_DEBUG("Q(s,a) = " << Q(s,a) << " |  Q(sp, ap) = " << Q(sp, ap) << " | r = " << r);
        Q(s,a) = Q(s,a) + lrate*(r+discount*Q(sp, ap) - Q(s, a) );
// 	LOG_DEBUG("new Q(s,a) = " << Q(s,a));

        if(sml::Utils::rand01() < espilon ) {
// 	    LOG_DEBUG("epsi-greedy");
            Pap = new DAction(&ACTION_TEMPLATE, {rand() % ACTIONS_ACC, rand() % ACTIONS_DIRECTION});//TODO:memory
	    ap = *Pap;
        }

    }
    init=true;
    
    lastAction = Pap;
    lastState = Psp;

//     std::cout << "etat " << sp[AGL] << " "<< sp[DST] << " action " << ap[ACC] << " "<< ap[DIR] << "   recomp : " << r << std::endl;
//     std::cout << std::flush;
    
//     LOG_DEBUG(car->_speed_x << " " << car->_speed_y);

    applyActionOn(ap, car);
}

void QLearnDiscr::newRace(tCarElt* car, tSituation *s) {
    Driver::newRace(car,s);

    Q.read("smile0.data");    
    
    lastState = new DState(&STATE_TEMPLATE, 0);
    lastAction = new DAction(&ACTION_TEMPLATE, 0);
}
 

DState* QLearnDiscr::discretize(const State& st) {
    DState* dst = new DState(&STATE_TEMPLATE, 0) ;
    dst->set(AGL, TWorld::discretizeAngle(st.angle, STATES_ALPHA));
    dst->set(DST, TWorld::discretizeDistance(st.distanceFromMiddle, STATES_DISTANCE, -6., 6.));
    return dst;
}

void QLearnDiscr::applyActionOn(const DAction& ac, tCarElt* car) {
    car->ctrl.steer = TWorld::computeSteering(ac[DIR], ACTIONS_DIRECTION, -0.4, 0.4);

    int accel = ac[ACC];

    if(accel < 2) {
        accel = 2 - accel;
        car->ctrl.gear = -1;
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = accel / 3.;
    } else if(accel < 4) {
        accel -= 2;
        accel = 2 - accel;
        //car->ctrl.gear = car->ctrl.gear;
        car->ctrl.brakeCmd = accel / 3.;
        car->ctrl.accelCmd = 0;
    } else if(accel < 7)
    {
        accel -= 4;
        accel = 3 - accel;
        car->ctrl.gear = 1;
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = accel / 4.;
    }
    else
    {
        accel -= 7;
        accel = 3 - accel;
        car->ctrl.gear = 2;
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = 1. - (accel / 4.);
    }
}
