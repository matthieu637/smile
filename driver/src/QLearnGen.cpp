#include "QLearnGen.hpp"
#include <sml/SaveLoad.hpp>
#include <sml/Utils.hpp>
#include <iostream>

QLearnGen::QLearnGen(int index):Driver(index, DECISION_EACH)
{

}

QLearnGen::~QLearnGen()
{
    sml::SaveLoad::writeQ("smile1.data", Q, STATES_ALPHA, STATES_DISTANCE, ACTIONS_ACC, ACTIONS_DIRECTION);
    sml::SaveLoad::writeQ("smile1.2.data", N, STATES_ALPHA, STATES_DISTANCE, ACTIONS_ACC, ACTIONS_DIRECTION);

    delete Q;//TODO:improve
    delete N;
}

float QLearnGen::reward() {
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

void QLearnGen::decision()
{
   /* DState s = lastState;

    //Take action a, observe r, s'
    DAction a = lastAction;
    float r = reward();
    State st = { angle , car->_trkPos.toMiddle };
    DState sp = discretize(st);

    //Choose a' from s' using policy derived from Q
    DAction ap = bestQVal(sp);
    DAction as = ap; //if a' ties for the max, then a* <- a'
    if(sml::Utils::rand01() < espilon ) {
        ap = {rand() % ACTIONS_ACC, rand() % ACTIONS_DIRECTION};
    }

    if(lastState.alpha != -1 && lastState.distance != -1) {

        float delta = r+discount*Q[sp.alpha][sp.distance][as.acc][as.direc] - Q[s.alpha][s.distance][a.acc][a.direc];
        N[s.alpha][s.distance][a.acc][a.direc] = N[s.alpha][s.distance][a.acc][a.direc] + 1.;

        for(int i = 0; i < STATES_ALPHA; i++)
            for(int j = 0; j < STATES_DISTANCE; j++)
                for(int k = 0; k < ACTIONS_ACC; k++)
                    for(int m = 0; m < ACTIONS_DIRECTION; m++) {
                        Q[i][j][k][m] = Q[i][j][k][m] + lrate * delta * N[i][j][k][m];
                        if(ap.acc == as.acc && ap.direc == as.direc) //a* == a'
                            N[i][j][k][m] = discount*lamba*N[i][j][k][m];
                        else
                            N[i][j][k][m] = 0.;
                    }

    }
//     //update Q value with lastState
//     if(lastState.alpha != -1 && lastState.distance != -1) {
//         Q[lastState.alpha][lastState.distance][lastAction.acc][lastAction.direc] =
//             (float) Q[lastState.alpha][lastState.distance][lastAction.acc][lastAction.direc] + //alpha*N[lastState.alpha][lastState.distance][lastAction.acc][lastAction.direc]*
//             lrate*(r+discount*Q[dst.alpha][dst.distance][dac.acc][dac.direc] - Q[lastState.alpha][lastState.distance][lastAction.acc][lastAction.direc] );
//
//     }



    lastAction = ap;
    lastState = sp;

    std::cout << "etat " << sp.alpha << " "<< sp.distance << " action " << ap.acc << " "<< ap.direc << "   recomp : " << r << " : "  << angle << std::endl;
    std::cout << std::flush;

    applyActionOn(ap, car);
*/
}

void QLearnGen::newRace(tCarElt* car, tSituation *s) {
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

    N = new float***[STATES_ALPHA];//[STATES_DISTANCE][ACTIONS_ACC][ACTIONS_DIRECTION];
    for(int i = 0; i < STATES_ALPHA; i++) {
        N[i] = new float**[STATES_DISTANCE];
        for(int j = 0; j < STATES_DISTANCE; j++) {
            N[i][j] = new float*[ACTIONS_ACC];
            for(int k = 0; k < ACTIONS_ACC; k++) {
                N[i][j][k] = new float[ACTIONS_DIRECTION];
            }
        }
    }


    sml::SaveLoad::initializeQ("smile1.data", Q, STATES_ALPHA, STATES_DISTANCE, ACTIONS_ACC, ACTIONS_DIRECTION);
    sml::SaveLoad::initializeQ("smile1.2.data", N, STATES_ALPHA, STATES_DISTANCE, ACTIONS_ACC, ACTIONS_DIRECTION);
}



void QLearnGen::applyActionOn(const DAction& ac, tCarElt* car) {
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
        car->ctrl.gear = getGear();
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = accel / 4.;
    }
}
