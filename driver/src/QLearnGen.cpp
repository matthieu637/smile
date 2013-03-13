#include "QLearnGen.hpp"
#include <TWorld.hpp>
#include <sml/SaveLoad.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>

const sml::ActionTemplate QLearnGen::ACTION_TEMPLATE = sml::ActionTemplate( {ACC, DIR}, {QLearnGen::ACTIONS_ACC, QLearnGen::ACTIONS_DIRECTION});

QLearnGen::QLearnGen(int index):Driver(index, DECISION_EACH), Q(&ACTION_TEMPLATE)
{

}

QLearnGen::~QLearnGen()
{
    Q.write("smile1.data");
}

void QLearnGen::decision()
{
//     State s = *lastState;

    //Take action a, observe r, s'
    DAction a = *lastAction;
//     double r = TWorld::reward(*this);
//     State st = *TWorld::observe(*this);
   

    //Choose a' from s' using policy derived from Q
    DAction* PaP;
//     DAction ap = *PaP;
//     DAction as = ap; //if a' ties for the max, then a* <- a'
    if(sml::Utils::rand01() < espilon ){
        PaP = new DAction(&ACTION_TEMPLATE, {rand() % ACTIONS_ACC, rand() % ACTIONS_DIRECTION});//TODO:memory
// 	ap = *PaP;
    }


    if(init) {
          

    }


    lastAction = PaP;

    init=true;

//     applyActionOn(ap, car);

}

void QLearnGen::newRace(tCarElt* car, tSituation *s) {
    Driver::newRace(car,s);
    
    Q.read("smile1.data");

    lastState = TWorld::observe(*this);
    lastAction = new DAction(&ACTION_TEMPLATE, 0);
}

void QLearnGen::applyActionOn(const DAction& ac, tCarElt* car) {
    car->ctrl.steer = TWorld::computeSteering(ac[DIR], ACTIONS_DIRECTION, -0.4, 0.4);

    unsigned int accel = ac[ACC];

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
        //car->ctrl.gear = getGear();
	car->ctrl.gear = 1;
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = accel / 4.;
    } else
    {
        accel -= 7;
        accel = 3 - accel;
        car->ctrl.gear = 2;
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = 1. - (accel / 4.);
    }
}

