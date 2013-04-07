#include "QLearnDiscr.hpp"
#include <TWorld.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>

// const sml::ActionTemplate QLearnDiscr::ACTION_TEMPLATE = sml::ActionTemplate( {ACC, DIRE}, {TWorld::ACTIONS_ACC, QLearnDiscr::ACTIONS_DIRECTION});
const sml::ActionTemplate QLearnDiscr::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE}, {QLearnDiscr::ACTIONS_DIRECTION});
const sml::StateTemplate QLearnDiscr::STATE_TEMPLATE = sml::StateTemplate( {AGL, DST}, {QLearnDiscr::STATES_ALPHA, QLearnDiscr::STATES_DISTANCE}); //, SPD});


QLearnDiscr::QLearnDiscr(int index):Driver(index, DECISION_EACH, 1)
{
    State st = *TWorld::initialState();
    DState* dst = discretize(st);
    q = new sml::QLearning(&STATE_TEMPLATE, &ACTION_TEMPLATE, *dst, *TWorld::initialAction(&ACTION_TEMPLATE));
}

QLearnDiscr::~QLearnDiscr()
{

}

void QLearnDiscr::endRace() {
    q->write("smile0.data");
}

void QLearnDiscr::decision()
{
    DAction* a;

    if(car->_speed_x < 9 && car->_distRaced < 10) {
        a = TWorld::initialAction(&ACTION_TEMPLATE);
    }
    else {
        State st = *TWorld::observe(*this);
        DState* dst = discretize(st);
        a = q->decision(*dst,reward,lrate,espilon,discount);
        LOG_DEBUG("etat " << *dst << " action " << *a << " recomp : " << reward);
    }
    
    applyActionOn(*a, car);
}

void QLearnDiscr::newRace(tCarElt* car, tSituation *s) {
    Driver::newRace(car,s);
    q->read("smile0.data");
}


DState* QLearnDiscr::discretize(const State& st) {
    DState* dst = new DState(&STATE_TEMPLATE, 0) ;
    dst->set(AGL, TWorld::discretizeAngle(st.angle, STATES_ALPHA));
    dst->set(DST, TWorld::discretizeDistance(st.distanceFromMiddle, STATES_DISTANCE, -6., 6.));
    return dst;
}

void QLearnDiscr::applyActionOn(const DAction& ac, tCarElt* car) {
    car->ctrl.steer = TWorld::computeSteering(ac[DIRE], ACTIONS_DIRECTION, -0.4, 0.4);

    car->ctrl.gear = 1;
    car->ctrl.brakeCmd = 0;

    if(car->_speed_x  > 10)
        car->ctrl.accelCmd = 0;
    else
        car->ctrl.accelCmd = 0.3;
}
