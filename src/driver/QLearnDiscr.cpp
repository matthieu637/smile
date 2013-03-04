#include "QLearnDiscr.hpp"
#include <sml/SaveLoad.hpp>
#include <sml/Utils.hpp>
#include <iostream>

QLearnDiscr::QLearnDiscr(int index):Driver(index, DECISION_EACH)
{

}

QLearnDiscr::~QLearnDiscr()
{
    sml::SaveLoad::writeQ(Q, STATES_ALPHA, STATES_DISTANCE, ACTIONS_ACC, ACTIONS_DIRECTION);
}

float QLearnDiscr::reward() {
    float distanceMilieu = 10. - abs( car->_trkPos.toMiddle );
    float distParcourue;
    float dammageGet = car->_dammage - lastDammage;

    if(lastDist==-1)
        distParcourue = 0;
    else
        distParcourue = car->_distFromStartLine - lastDist;
    if(distParcourue > 1000|| distParcourue < -1000) //passe ligne
        distParcourue = 20;
    float r = distanceMilieu*distParcourue;
    if (r < 1 )
        r = -10;
    if(isStuck() && car->ctrl.gear == -1)
        r = 15;
    else if(distParcourue < 0.5) {
        r = -40;
        if(isStuck())
            r -= 45;
    }
    else if(distanceMilieu < 0. || distParcourue < 0.)
        r = -r;
    else r = r*r/4.;

    r-=dammageGet;

    if(car->_trkPos.toRight < 0. )
        r += 15*car->_trkPos.toRight ;
    else if (car->_trkPos.toLeft < 0.)
        r += 15*car->_trkPos.toLeft ;
    /*
    if(r > 0)
       if((angle < M_PI/16 && angle >= 0) || (angle > -M_PI/16 && angle <= 0 ))
    r += 40;*/

    //std::cout << "recompense " << distanceMilieu << " "<< distParcourue << " " << r << " -- "  << car->_distFromStartLine << " " << lastDist << std::endl;
    //std::cout << std::flush;

    lastDist = car->_distFromStartLine;
    lastDammage = car->_dammage;

    return r;
}

void QLearnDiscr::decision()
{

    float r = reward();

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
    Q = (float****) new float[STATES_ALPHA][STATES_DISTANCE][ACTIONS_ACC][ACTIONS_DIRECTION];
    sml::SaveLoad::initializeQ(Q, STATES_ALPHA, STATES_DISTANCE, ACTIONS_ACC, ACTIONS_DIRECTION);
}


DState QLearnDiscr::discretize(const State& st) {
    //pointer me
    DState dst = {0,0} ; //init me pls or bug

    for(int i=0; i<STATES_ALPHA; i++)
        if(st.alpha < -M_PI +(M_PI/8*i) ) {
            dst.alpha = i;
            break;
        }

//	std::cout << "distance " << st.distance << " "<< dst.distance << std::endl;
//	std::cout << std::flush;

    float dismin = -6.;
    float dismax = 6.;

    for(int i=0; i<STATES_DISTANCE; i++)
        if(st.distance < dismin + (dismax-dismin/(float)STATES_DISTANCE)*(float)i) {
            dst.distance = i;
            break;
        }

    //std::cout << "etat " << dst.alpha << " "<< dst.distance << " " << std::endl;
    //std::cout << std::flush;

    return dst;
}

DAction QLearnDiscr::bestQVal(const DState& dst) {


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
    float smin = -0.4;
    float smax = 0.4;


    car->ctrl.steer =  smin+((float)ac.direc/(float)ACTIONS_DIRECTION)*(smax-smin);

    //std::cout << "steer " << car->ctrl.steer << " " << ac.direc << " " << (float)(ac.direc/ACTIONS_DIRECTION) << " " << (smax-smin) <<" "<< (float)smin+((float)(ac.direc/ACTIONS_DIRECTION))*(smax-smin) << std::endl;
    //std::cout << std::flush;

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