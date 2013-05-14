#include "QLearnGenCmplx.hpp"
#include <TWorld.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>
#include <TFeatures.hpp>

using sml::Feature;

const sml::ActionTemplate QLearnGenCmplx::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE, ACC}, {QLearnGenCmplx::ACTIONS_DIRECTION, TWorld::ACTIONS_ACC});

const double QLearnGenCmplx::road_width = 14;
const double QLearnGenCmplx::total_angle = 2*M_PI;
const double QLearnGenCmplx::total_speed = 50;
const double QLearnGenCmplx::max_straight = 50;
const double QLearnGenCmplx::max_nextarc = 0.4;


QLearnGenCmplx::QLearnGenCmplx(int index):DriverFeedback(index, DECISION_EACH, simu_time,
{{GFCTRL_TYPE_SKEYBOARD, 114}, {GFCTRL_TYPE_SKEYBOARD, 100}, {GFCTRL_TYPE_SKEYBOARD, 101}, {GFCTRL_TYPE_SKEYBOARD, 102}, {GFCTRL_TYPE_SKEYBOARD, 103}}                             )
{
    QLearnGradient<State>::featuredList *features = new QLearnGradient<State>::featuredList();


#define X TFeatures::_1DMiddle(road_width)
#define Y TFeatures::_1DAngle(total_angle)
#define Z TFeatures::_1DSpeed(total_speed)
#define U TFeatures::_1DStraight(max_straight)
#define V TFeatures::_1DNextArc(max_nextarc)
#define P TFeatures::_1DAction(ACC)
#define Q TFeatures::_1DAction(DIRE)

#define XW nbXinter, road_width
#define YW nbYinter, total_angle
#define ZW nbZinter, total_speed
#define UW nbUinter, max_straight
#define VW nbVinter, max_nextarc
#define PW TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC
#define QW QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION


    // // State
    //3D
//     features->push_back(Feature<State>( {X,Y,Z}, {XW, YW, ZW}));
//     features->push_back(Feature<State>( {X,Y,U}, {XW, YW, UW}));
//     features->push_back(Feature<State>( {X,Y,V}, {XW, YW, VW}));
//     features->push_back(Feature<State>( {X,Z,U}, {XW, ZW, UW}));
//     features->push_back(Feature<State>( {X,Z,V}, {XW, ZW, VW}));
//     features->push_back(Feature<State>( {X,U,V}, {XW, UW, VW}));
//     features->push_back(Feature<State>( {Y,Z,U}, {YW, ZW, UW}));
//     features->push_back(Feature<State>( {Y,Z,V}, {YW, ZW, VW}));
//     features->push_back(Feature<State>( {Y,U,V}, {YW, UW, VW}));
//     features->push_back(Feature<State>( {Z,U,V}, {ZW, UW, VW}));
    
    //4D
    features->push_back(Feature<State>( {X,Y,Z,U}, {XW, YW, ZW, UW}));
//     features->push_back(Feature<State>( {X,Y,Z,V}, {XW, YW, ZW, VW}));
//     features->push_back(Feature<State>( {X,Z,U,V}, {XW, ZW, UW, VW}));
//     features->push_back(Feature<State>( {Y,Z,U,V}, {YW, ZW, UW, VW}));
// 5D == bias but it's too big
//     features->push_back(Feature<State>( {X,Y,Z,U,V}, {XW, YW, ZW, UW, VW}));

// // 	MIX STATE/ACTION 3D

//     features->push_back(Feature<State>( {X,Y,P}, {XW, YW, PW}));
//     features->push_back(Feature<State>( {X,Z,P}, {XW, ZW, PW}));
//     features->push_back(Feature<State>( {X,U,P}, {XW, UW, PW}));
// //     features->push_back(Feature<State>( {X,V,P}, {XW, VW, PW}));
//     features->push_back(Feature<State>( {Y,Z,P}, {YW, ZW, PW}));
//     features->push_back(Feature<State>( {Y,U,P}, {YW, UW, PW}));
// //     features->push_back(Feature<State>( {Y,V,P}, {YW, VW, PW}));
//     features->push_back(Feature<State>( {Z,U,P}, {ZW, UW, PW}));
// //     features->push_back(Feature<State>( {Z,V,P}, {ZW, VW, PW}));
// //     features->push_back(Feature<State>( {U,V,P}, {UW, VW, PW}));
// 
//     features->push_back(Feature<State>( {X,Y,Q}, {XW, YW, QW}));
//     features->push_back(Feature<State>( {X,Z,Q}, {XW, ZW, QW}));
//     features->push_back(Feature<State>( {X,U,Q}, {XW, UW, QW}));
// //     features->push_back(Feature<State>( {X,V,Q}, {XW, VW, QW}));
//     features->push_back(Feature<State>( {Y,Z,Q}, {YW, ZW, QW}));
//     features->push_back(Feature<State>( {Y,U,Q}, {YW, UW, QW}));
// //     features->push_back(Feature<State>( {Y,V,Q}, {YW, VW, QW}));
//     features->push_back(Feature<State>( {Z,U,Q}, {ZW, UW, QW}));
// //     features->push_back(Feature<State>( {Z,V,Q}, {ZW, VW, QW}));
// //     features->push_back(Feature<State>( {U,V,Q}, {UW, VW, QW}));


// // 	MIX STATE/ACTION 4D

    features->push_back(Feature<State>( {X,Y,Z,P}, {XW, YW, ZW, PW}));
    features->push_back(Feature<State>( {X,Y,U,P}, {XW, YW, UW, PW}));
//     features->push_back(Feature<State>( {X,Y,V,P}, {XW, YW, VW, PW}));
    features->push_back(Feature<State>( {X,Z,U,P}, {XW, ZW, UW, PW}));
//     features->push_back(Feature<State>( {X,Z,V,P}, {XW, ZW, VW, PW}));
    features->push_back(Feature<State>( {X,U,V,P}, {XW, UW, VW, PW}));
    features->push_back(Feature<State>( {Y,Z,U,P}, {YW, ZW, UW, PW}));
//     features->push_back(Feature<State>( {Y,Z,V,P}, {YW, ZW, VW, PW}));
//     features->push_back(Feature<State>( {Y,U,V,P}, {YW, UW, VW, PW}));
//     features->push_back(Feature<State>( {Z,U,V,P}, {ZW, UW, VW, PW}));

    features->push_back(Feature<State>( {X,Y,Z,Q}, {XW, YW, ZW, QW}));
    features->push_back(Feature<State>( {X,Y,U,Q}, {XW, YW, UW, QW}));
//     features->push_back(Feature<State>( {X,Y,V,Q}, {XW, YW, VW, QW}));
    features->push_back(Feature<State>( {X,Z,U,Q}, {XW, ZW, UW, QW}));
//     features->push_back(Feature<State>( {X,Z,V,Q}, {XW, ZW, VW, QW}));
//     features->push_back(Feature<State>( {X,U,V,Q}, {XW, UW, VW, QW}));
    features->push_back(Feature<State>( {Y,Z,U,Q}, {YW, ZW, UW, QW}));
//     features->push_back(Feature<State>( {Y,Z,V,Q}, {YW, ZW, VW, QW}));
//     features->push_back(Feature<State>( {Y,U,V,Q}, {YW, UW, VW, QW}));
//     features->push_back(Feature<State>( {Z,U,V,Q}, {ZW, UW, VW, QW}));
//
// // 	MIX STATE/ACTION 5D too big
//     features->push_back(Feature<State>( {X,Y,Z,U,P}, {XW, YW, ZW, UW, PW}));
//     features->push_back(Feature<State>( {X,Y,Z,V,P}, {XW, YW, ZW, VW, PW}));
//     features->push_back(Feature<State>( {X,Y,U,V,P}, {XW, YW, UW, VW, PW}));
//     features->push_back(Feature<State>( {X,Z,U,V,P}, {XW, ZW, UW, VW, PW}));
//     features->push_back(Feature<State>( {Y,Z,U,V,P}, {YW, ZW, UW, VW, PW}));
//
//     features->push_back(Feature<State>( {X,Y,Z,U,Q}, {XW, YW, ZW, UW, QW}));
//     features->push_back(Feature<State>( {X,Y,Z,V,Q}, {XW, YW, ZW, VW, QW}));
//     features->push_back(Feature<State>( {X,Y,U,V,Q}, {XW, YW, UW, VW, QW}));
//     features->push_back(Feature<State>( {X,Z,U,V,Q}, {XW, ZW, UW, VW, QW}));
//     features->push_back(Feature<State>( {Y,Z,U,V,Q}, {YW, ZW, UW, VW, QW}));
//
    unsigned int nbFeature = 0;
    for(QLearnGradient<State>::fLiterator flist = features->begin() ; flist != features->end(); ++flist) {
        nbFeature += flist->getSize();
    }

    qlg = new QLearnGradient<State>(features, nbFeature, &ACTION_TEMPLATE, *TWorld::initialAction(&ACTION_TEMPLATE) , conf);
}

QLearnGenCmplx::~QLearnGenCmplx()
{

}

void QLearnGenCmplx::decision()
{
    if(car->_speed_x < 15 && car->_distRaced < 15) {
        DAction ac;
        ac = *TWorld::initialAction(&ACTION_TEMPLATE);
        ac.set(ACC, 3);
        applyActionOn(ac, car);
        lastAction = TWorld::initialAction(&ACTION_TEMPLATE);
    }
    else {
        State* st = TWorld::observe(*this);

        if(skeyInfo[114].state) {
            delay++;
            if(delay>10) {
                activeTutor = !activeTutor;
                delay = 0;
            }
        }
        else delay = 0;

        if(activeTutor) {
            DAction* ac = lastAction;

            if(skeyInfo[102].state && ac->get(DIRE) > 0 )
                ac->set(DIRE,  ac->get(DIRE) - 1);
            else if(skeyInfo[100].state && ac->get(DIRE) + 1 < ACTIONS_DIRECTION)
                ac->set(DIRE,  ac->get(DIRE) + 1);
            else if(!skeyInfo[100].state && !skeyInfo[102].state) { //come back to center
                if(ac->get(DIRE) < ACTIONS_DIRECTION/2 )
                    ac->set(DIRE,  ac->get(DIRE) + 1);
                else if (ac->get(DIRE) > ACTIONS_DIRECTION/2 )
                    ac->set(DIRE,  ac->get(DIRE) - 1);
            }

            if(skeyInfo[101].state)
                ac->set(ACC,  3);
            else if(skeyInfo[103].state)
                if(car->_speed_x < 5)
                    ac->set(ACC,  0);
                else
                    ac->set(ACC,  1);
            else if(!skeyInfo[101].state && !skeyInfo[103].state) { //come back to center
                if(ac->get(ACC) < 2 )
                    ac->set(ACC,  ac->get(ACC) + 1);
                else if (ac->get(ACC) > 2 )
                    ac->set(ACC,  ac->get(ACC) - 1);
            }

            qlg->observeTutor(*st, *ac, reward > 0 ? reward + 3000 : reward, reward >0 ? lrate*3. : lrate, lambda, discount, false);

            LOG_DEBUG("under tutor control -- action " << *ac << " recomp : " << reward <<" " << car->_speed_x);
            applyActionOn(*ac, car);
            lastAction = ac;
        } else {
            DAction ac = *qlg->learn(*st, reward, lrate,  epsilon, lambda, discount, false);
//             LOG_DEBUG(" action " << ac << " recomp : " << reward);
            applyActionOn(ac, car);
	    delete lastAction;
            lastAction = TWorld::initialAction(&ACTION_TEMPLATE);
// 	    delete ac;
        }
        
        delete st;
    }
}

void QLearnGenCmplx::newRace(tCarElt* car, tSituation *s) {
    Driver::newRace(car,s);

    qlg->read("smile3.data");
    activeTutor = false;
    delay = 0;
}

void QLearnGenCmplx::endRace() {
    Driver::endRace();
    qlg->write("smile3.data");
}

sml::LearnStat* QLearnGenCmplx::getAlgo() {
    return qlg;
}

void QLearnGenCmplx::applyActionOn(const DAction& ac, tCarElt* car) {

    car->ctrl.steer = TWorld::computeSteering(ac[DIRE], ACTIONS_DIRECTION, -0.7, 0.7);
    TWorld::applyAccelerationGear(*this, car, ac[ACC]);
}

