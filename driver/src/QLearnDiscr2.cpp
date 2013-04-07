#include "QLearnDiscr2.hpp"
#include <TWorld.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>

const sml::ActionTemplate QLearnDiscr2::ACTION_TEMPLATE = sml::ActionTemplate( {ACC, DIRE}, {TWorld::ACTIONS_ACC, QLearnDiscr2::ACTIONS_DIRECTION});

// const sml::StateTemplate QLearnDiscr2::STATE_TEMPLATE = sml::StateTemplate( {AGL, RDST, LDST}, {QLearnDiscr2::STATES_ALPHA, STATES_DISTANCE, STATES_DISTANCE});
const sml::StateTemplate QLearnDiscr2::STATE_TEMPLATE = sml::StateTemplate( {AGL, DST}, {QLearnDiscr2::STATES_ALPHA, QLearnDiscr2::STATES_DISTANCE*2});
// const sml::StateTemplate QLearnDiscr2::STATE_TEMPLATE = sml::StateTemplate( {AGL, DST, SPD}, {QLearnDiscr2::STATES_ALPHA, QLearnDiscr2::STATES_DISTANCE*2, 3});

QLearnDiscr2::QLearnDiscr2(int index):Driver(index, DECISION_EACH, 2.), Q(&STATE_TEMPLATE, &ACTION_TEMPLATE), N(&STATE_TEMPLATE, &ACTION_TEMPLATE)
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
        PaP = new DAction(&ACTION_TEMPLATE, rand() % (int)ACTION_TEMPLATE.sizeNeeded());//TODO:memory
        ap = *PaP;
    }


    if(init) {
        double delta = reward + discount*Q(sp, as) - Q(s,a);
        N(s,a) = N(s, a) + 1.;

        for(std::set< std::pair<DState* , DAction* >, HistoryComparator >::iterator it = history.begin(); it != history.end(); ++it ) {
            DState sa = *(*it).first;
            DState aa = *(*it).second;
            Q(sa,aa) = Q(sa,aa) + lrate * delta * N(sa,aa);
            if(ap == as)
                N(sa, aa) = discount*lamba*N(sa,aa);
            else
                N(sa, aa) = 0.;
        }

        if(ap == as)
            history.insert(std::pair<DState* , DAction* >(lastState, lastAction));
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

    dst->set(AGL, TWorld::discretizeAngle(st.angle, STATES_ALPHA));
//     dst->set(RDST, TWorld::discretizeDistance(st.rightDistance, STATES_DISTANCE, -3., 5.));
//     dst->set(LDST, TWorld::discretizeDistance(st.leftDistance, STATES_DISTANCE, -3., 5.));
    dst->set(DST, TWorld::discretizeDistance(st.distanceFromMiddle, STATES_DISTANCE*2, -6., 6.));
//     dst->set(SPD, TWorld::discretizeDistance(st.speed, 3, -5., 10.));
    return dst;
}

void QLearnDiscr2::applyActionOn(const DAction& ac, tCarElt* car) {
    car->ctrl.steer = TWorld::computeSteering(ac[DIRE], ACTIONS_DIRECTION, -0.4, 0.4);
    TWorld::applyAcceleration(car, ac[ACC]);
}

