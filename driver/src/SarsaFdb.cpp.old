#include "SarsaFdb.hpp"
#include <TWorld.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>
#include <TFeatures.hpp>

using sml::Feature;



const sml::StateTemplate SarsaFdb::STATE_TEMPLATE = sml::StateTemplate( {DST, AGL, SPD}, {SarsaFdb::nbXinter, SarsaFdb::nbYinter,SarsaFdb::nbZinter});
const sml::ActionTemplate SarsaFdb::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE, ACC}, {SarsaFdb::ACTIONS_DIRECTION, TWorld::ACTIONS_ACC});



SarsaFdb::SarsaFdb(int index):DriverFeedback(index, DECISION_EACH, simu_time,
{{GFCTRL_TYPE_SKEYBOARD, 114}, {GFCTRL_TYPE_SKEYBOARD, 100}, {GFCTRL_TYPE_SKEYBOARD, 101}, {GFCTRL_TYPE_SKEYBOARD, 102}, {GFCTRL_TYPE_SKEYBOARD, 103}}
                                                        )
{
    State st = *TWorld::initialState();
    DState* dst = discretize(st);
    qlg = new sml::SarsaLamb (&STATE_TEMPLATE, &ACTION_TEMPLATE, *dst, *TWorld::initialAction(&ACTION_TEMPLATE), conf);
}

SarsaFdb::~SarsaFdb()
{

}


DState* SarsaFdb::discretize(const State& st) {
    DState* dst = new DState(&STATE_TEMPLATE, 0) ;
    dst->set(DST, TWorld::discretizeDistance(st.distanceFromMiddle, nbXinter, -6., 6.));
    dst->set(AGL, TWorld::discretizeAngle(st.angle, nbYinter));
    dst->set(SPD, TWorld::discretizeDistance(st.speed, nbZinter, -15, 70));

    return dst;
}

void SarsaFdb::decision()
{
    if(car->_speed_x < 15 && car->_distRaced < 15) {
        DAction ac;
        ac = *TWorld::initialAction(&ACTION_TEMPLATE);
        ac.set(ACC, 3);
        applyActionOn(ac, car);
        lastAction = TWorld::initialAction(&ACTION_TEMPLATE);
    }
    else {
        State st = *TWorld::observe(*this);

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
            else if (skeyInfo[103].state)
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

            qlg->observeTutor(*discretize(st), *ac, reward + 3000, lrate, lambda, discount, false);

            LOG_DEBUG("under tutor control -- action " << *ac << " recomp : " << reward <<" " << car->_speed_x);
            applyActionOn(*ac, car);
            lastAction = ac;
        } else {
            DAction ac = *qlg->learn(*discretize(st), reward, lrate,  epsilon, lambda, discount, false);
            LOG_DEBUG(" action " << ac << " recomp : " << reward);
            applyActionOn(ac, car);
            lastAction = TWorld::initialAction(&ACTION_TEMPLATE);;
        }
    }
}

void SarsaFdb::newRace(tCarElt* car, tSituation *s) {
    Driver::newRace(car,s);

    qlg->read("smile5.data");
    activeTutor = false;
    delay = 0;
}

void SarsaFdb::endRace() {
    Driver::endRace();
    qlg->write("smile5.data");
}

sml::LearnStat* SarsaFdb::getAlgo() {
    return qlg;
}

void SarsaFdb::applyActionOn(const DAction& ac, tCarElt* car) {

    car->ctrl.steer = TWorld::computeSteering(ac[DIRE], ACTIONS_DIRECTION, -0.5, 0.5);
    TWorld::applyAccelerationGear(*this, car, ac[ACC]);
}

