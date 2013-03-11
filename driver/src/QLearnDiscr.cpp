#include "QLearnDiscr.hpp"
#include <TWorld.hpp>
#include <sml/SaveLoad.hpp>
#include <sml/Utils.hpp>
#include <iostream>

QLearnDiscr::QLearnDiscr(int index):Driver(index, DECISION_EACH)
{

}

QLearnDiscr::~QLearnDiscr()
{ 
    sml::SaveLoad::writeQ("smile0.data", Q, STATES_ALPHA, STATES_DISTANCE, ACTIONS_ACC, ACTIONS_DIRECTION);
    
    delete Q;//TODO:improve
}

void QLearnDiscr::decision()
{

    float r = TWorld::reward(*this);

    State st = { angle , car->_trkPos.toMiddle };
    DState dst = discretize(st);
    DAction dac = bestQVal(dst);

    //update Q value with lastState
    if(lastState.alpha != -1 && lastState.distance != -1) {
        Q[lastState.alpha][lastState.distance][lastAction.acc][lastAction.direc] =
            (float) Q[lastState.alpha][lastState.distance][lastAction.acc][lastAction.direc] + //alpha*N[lastState.alpha][lastState.distance][lastAction.acc][lastAction.direc]*
            lrate*(r+discount*Q[dst.alpha][dst.distance][dac.acc][dac.direc] - Q[lastState.alpha][lastState.distance][lastAction.acc][lastAction.direc] );

    }

    if(sml::Utils::rand01() < espilon ) {
        dac = {rand() % ACTIONS_ACC, rand() % ACTIONS_DIRECTION};
    }

    lastAction = dac;
    lastState = dst;

    std::cout << "etat " << dst.alpha << " "<< dst.distance << " action " << dac.acc << " "<< dac.direc << "   recomp : " << r << " : "  << angle << std::endl;
    std::cout << std::flush;

    applyActionOn(dac, car);

}

void QLearnDiscr::newRace(tCarElt* car, tSituation *s) {
    Driver::newRace(car,s);
    
    
    Q = new float***[STATES_ALPHA];//[STATES_DISTANCE][ACTIONS_ACC][ACTIONS_DIRECTION];
    for(int i = 0; i < STATES_ALPHA; i++) {
        Q[i] = new float**[STATES_DISTANCE];
        for(int j = 0; j < STATES_DISTANCE; j++) {
            Q[i][j] = new float*[ACTIONS_ACC];
            for(int k = 0; k < ACTIONS_ACC; k++) {
                Q[i][j][k] = new float[ACTIONS_DIRECTION];
            }
        }
    }
    
  
    sml::SaveLoad::initializeQ("smile0.data", Q, STATES_ALPHA, STATES_DISTANCE, ACTIONS_ACC, ACTIONS_DIRECTION);
}


QLearnDiscr::DState QLearnDiscr::discretize(const State& st) {
    //pointer me
    DState dst = {0,0} ;
    dst.alpha = TWorld::discretizeAngle(st.alpha, STATES_ALPHA);
    dst.distance = TWorld::discretizeDistanceFromMiddle(st.distance, STATES_DISTANCE, -6., 6.);
    return dst;
}

QLearnDiscr::DAction QLearnDiscr::bestQVal(const DState& dst) {


    DAction imax= {rand() % ACTIONS_ACC, rand() % ACTIONS_DIRECTION}; //pointer me
    for(int i=0; i<ACTIONS_ACC; i++)
        for(int k=0; k<ACTIONS_DIRECTION; k++) {
            //std::cout << dst.alpha << " "<< dst.distance <<" "<< i <<" " << j << " " << k << std::endl;
            //std::cout << std::flush;
            if(Q[dst.alpha][dst.distance][i][k] > Q[dst.alpha][dst.distance][imax.acc][imax.direc])
            {
                imax.acc = i;
                imax.direc = k;
            }
        }

//	std::cout << "action " << imax.acc << " "<< imax.direc << " " << imax.stop << std::endl;
//	std::cout << std::flush;

    return imax;
}

void QLearnDiscr::applyActionOn(const DAction& ac, tCarElt* car) {
    car->ctrl.steer = TWorld::computeSteering(ac.direc, ACTIONS_DIRECTION, -0.3, 0.3);

    int accel = ac.acc;

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