#include "QLearnDiscr2.hpp"
#include <TWorld.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>

const sml::ActionTemplate QLearnDiscr2::ACTION_TEMPLATE = sml::ActionTemplate( {ACC, DIRE}, {QLearnDiscr2::ACTIONS_ACC, QLearnDiscr2::ACTIONS_DIRECTION});

// const sml::StateTemplate QLearnDiscr2::STATE_TEMPLATE = sml::StateTemplate( {STK, AGL, DST, SPD},
// 							{2,QLearnDiscr2::STATES_ALPHA, QLearnDiscr2::STATES_DISTANCE, 6});
const sml::StateTemplate QLearnDiscr2::STATE_TEMPLATE = sml::StateTemplate( {AGL, DST}, {QLearnDiscr2::STATES_ALPHA, QLearnDiscr2::STATES_DISTANCE});

QLearnDiscr2::QLearnDiscr2(int index):Driver(index, DECISION_EACH), Q(&STATE_TEMPLATE, &ACTION_TEMPLATE), N(&STATE_TEMPLATE, &ACTION_TEMPLATE)
{

}

QLearnDiscr2::~QLearnDiscr2()
{

}

void QLearnDiscr2::endRace() {
    Q.write("smile1.data");
}

void QLearnDiscr2::decision()
{
    DState s = *lastState;

    //Take action a, observe r, s'
    DAction a = *lastAction;
    State st = *TWorld::observe(*this);
    DState* Psp = discretize(st);//TODO:memory leak
    DState sp = *Psp;

    //Choose a' from s' using policy derived from Q
    DAction* PaP = Q.argmax(sp); //TODO:memory leak
    DAction ap = *PaP;
    DAction as = ap; //if a' ties for the max, then a* <- a'
    if(sml::Utils::rand01() < espilon ) {
        PaP = new DAction(&ACTION_TEMPLATE, {rand() % ACTIONS_ACC, rand() % ACTIONS_DIRECTION});//TODO:memory
        ap = *PaP;
    }


    if(init) {
        double delta = reward + discount*Q(sp, as) - Q(s,a);
        N(s,a) = N(s, a) + 1.;

        /*
            for(unsigned int sa = 0; sa < STATE_TEMPLATE.sizeNeeded(); sa++)
                for(unsigned int aa = 0; aa < ACTION_TEMPLATE.sizeNeeded(); aa++) {
        	  Q(sa,aa) = Q(sa,aa) + lrate * delta * N(sa,aa);
                    if(ap == as)
                        N(sa, aa) = discount*lamba*N(sa,aa);
                    else
                        N(sa, aa) = 0.;
                }*/

        for(std::list< std::pair<DState* , DAction* > >::iterator it = history.begin(); it != history.end(); ++it ) {
            DState sa = *(*it).first;
            DState aa = *(*it).second;
            Q(sa,aa) = Q(sa,aa) + lrate * delta * N(sa,aa);
            if(ap == as)
                N(sa, aa) = discount*lamba*N(sa,aa);
            else
                N(sa, aa) = 0.;
        }

        if(ap == as)
            history.push_back(std::pair<DState* , DAction* >(lastState, lastAction));
        else
            history.clear();
    }


    lastAction = PaP;
    lastState = Psp;
    init=true;

    LOG_DEBUG("etat " << sp << " action " << ap << " recomp : " << reward);

    applyActionOn(ap, car);
}

void QLearnDiscr2::newRace(tCarElt* car, tSituation *s) {
    Driver::newRace(car,s);

    Q.read("smile1.data");

    lastState = new DState(&STATE_TEMPLATE, 0);
    lastAction = new DAction(&ACTION_TEMPLATE, 0);
}


DState* QLearnDiscr2::discretize(const State& st) {
    DState* dst = new DState(&STATE_TEMPLATE, 0) ;
//     dst->set(STK, st.stuck);
    dst->set(AGL, TWorld::discretizeAngle(st.angle, STATES_ALPHA));
    dst->set(DST, TWorld::discretizeDistance(st.distanceFromMiddle, STATES_DISTANCE, -6., 6.));
//     dst->set(SPD, TWorld::discretizeDistance(st.speed, 6, -40., 40.));
    return dst;
}

void QLearnDiscr2::applyActionOn(const DAction& ac, tCarElt* car) {
    car->ctrl.steer = TWorld::computeSteering(ac[DIRE], ACTIONS_DIRECTION, -0.4, 0.4);

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

