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
const double QLearnGenCmplx::total_speed = 85;
const double QLearnGenCmplx::max_straight = 60;


QLearnGenCmplx::QLearnGenCmplx(int index):DriverFeedback(index, DECISION_EACH, simu_time,
{{GFCTRL_TYPE_SKEYBOARD, 114}, {GFCTRL_TYPE_SKEYBOARD, 100}, {GFCTRL_TYPE_SKEYBOARD, 101}, {GFCTRL_TYPE_SKEYBOARD, 102}, {GFCTRL_TYPE_SKEYBOARD, 103}}
                                                        )
{
    QLearnGradient<State>::featuredList *features = new QLearnGradient<State>::featuredList();


// 	MIX STATE/ACTION 3D
    {
        Feature<State> f( {TFeatures::_1DMiddle(road_width),
                           TFeatures::_1DAngle(total_angle),
                           TFeatures::_1DAction(ACC)
                          },
        {nbXinter, road_width, nbYinter, total_angle, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC});
        features->push_back(f);
    }
    {
        Feature<State> f( {TFeatures::_1DMiddle(road_width),
                           TFeatures::_1DAngle(total_angle),
                           TFeatures::_1DAction(DIRE)
                          },
        {nbXinter, road_width, nbYinter, total_angle, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION});
        features->push_back(f);
    }
    {
        Feature<State> f( {TFeatures::_1DAction(ACC),
                           TFeatures::_1DAction(DIRE),
                           TFeatures::_1DMiddle(road_width)
                          },
        {TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION, nbXinter, road_width});
        features->push_back(f);
    }
    {
        Feature<State> f( {TFeatures::_1DAction(ACC),
                           TFeatures::_1DAction(DIRE),
                           TFeatures::_1DAngle(total_angle)
                          },
        {TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION, nbYinter, total_angle});
        features->push_back(f);
    }
// //     
// MIX STATE/ACTION 4D

    {
        Feature<State> f({TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DSpeed(total_speed),
                         TFeatures::_1DAction(ACC)},
        {nbXinter, road_width, nbYinter, total_angle, nbZinter, total_speed, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC});
        features->push_back(f);
    }

    {
        Feature<State> f({TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DSpeed(total_speed),
                         TFeatures::_1DAction(DIRE)},
        {nbXinter, road_width, nbYinter, total_angle, nbZinter, total_speed, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION});
        features->push_back(f);
    }

    {
        Feature<State> f({TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DAction(ACC),
                         TFeatures::_1DAction(DIRE)},
        {nbXinter, road_width, nbYinter, total_angle, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION});
        features->push_back(f);
    }

    {
        Feature<State> f({TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DSpeed(total_speed),
                         TFeatures::_1DAction(ACC),
                         TFeatures::_1DAction(DIRE)},
        {nbXinter, road_width, nbZinter, total_speed, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION});
        features->push_back(f);
    }

    {
        Feature<State> f({TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DSpeed(total_speed),
                         TFeatures::_1DAction(ACC),
                         TFeatures::_1DAction(DIRE)},
        {nbYinter, total_angle, nbZinter, total_speed, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION});
        features->push_back(f);
    }

// //     
    // 4D == bias
    {
        Feature<State> f( {TFeatures::_1DMiddle(road_width),
                           TFeatures::_1DAngle(total_angle),
                           TFeatures::_1DSpeed(total_speed),
                           TFeatures::_1DStraight(max_straight)
                          },
        {nbXinter, road_width, nbYinter, total_angle, nbZinter, total_speed, nbUinter, max_straight});
        features->push_back(f);
    }
// //     
    {
        Feature<State> f( {TFeatures::_1DAngle(total_angle),
                           TFeatures::_1DSpeed(total_speed),
                           TFeatures::_1DStraight(max_straight),
                           TFeatures::_1DAction(ACC),
                           TFeatures::_1DAction(DIRE)
                          },
        {   nbYinter, total_angle, nbZinter, total_speed, nbUinter, max_straight,
            TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION
        });
        features->push_back(f);
    }
    {
        Feature<State> f( {TFeatures::_1DMiddle(road_width),
                           TFeatures::_1DSpeed(total_speed),
                           TFeatures::_1DStraight(max_straight),
                           TFeatures::_1DAction(ACC),
                           TFeatures::_1DAction(DIRE)
                          },
        {   nbXinter, road_width, nbZinter, total_speed, nbUinter, max_straight,
            TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION
        });
        features->push_back(f);
    }    {
        Feature<State> f( {TFeatures::_1DMiddle(road_width),
                           TFeatures::_1DAngle(total_angle),
                           TFeatures::_1DStraight(max_straight),
                           TFeatures::_1DAction(ACC),
                           TFeatures::_1DAction(DIRE)
                          },
        {   nbXinter, road_width, nbYinter, total_angle, nbUinter, max_straight,
            TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION
        });
        features->push_back(f);
    }    {
        Feature<State> f( {TFeatures::_1DMiddle(road_width),
                           TFeatures::_1DAngle(total_angle),
                           TFeatures::_1DSpeed(total_speed),
                           TFeatures::_1DAction(ACC),
                           TFeatures::_1DAction(DIRE)
                          },
        {   nbXinter, road_width, nbYinter, total_angle, nbZinter, total_speed,
            TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION
        });
        features->push_back(f);
    }    {
        Feature<State> f( {TFeatures::_1DMiddle(road_width),
                           TFeatures::_1DAngle(total_angle),
                           TFeatures::_1DSpeed(total_speed),
                           TFeatures::_1DStraight(max_straight),
                           TFeatures::_1DAction(DIRE)
                          },
        {   nbXinter, road_width, nbYinter, total_angle, nbZinter, total_speed, nbUinter, max_straight,
            QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION
        });
        features->push_back(f);
    }    {
        Feature<State> f( {TFeatures::_1DMiddle(road_width),
                           TFeatures::_1DAngle(total_angle),
                           TFeatures::_1DSpeed(total_speed),
                           TFeatures::_1DStraight(max_straight),
                           TFeatures::_1DAction(ACC),
                          },
        {   nbXinter, road_width,  nbYinter, total_angle, nbZinter, total_speed, nbUinter, max_straight,
            TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC,
        });
        features->push_back(f);
    }

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

            qlg->observeTutor(st, *ac, reward + 3000, lrate, lambda, discount, false);

            LOG_DEBUG("under tutor control -- action " << *ac << " recomp : " << reward <<" " << car->_speed_x);
            applyActionOn(*ac, car);
            lastAction = ac;
        } else {
            DAction ac = *qlg->learn(st, reward, lrate,  epsilon, lambda, discount, false);
            LOG_DEBUG(" action " << ac << " recomp : " << reward);
            applyActionOn(ac, car);
            lastAction = TWorld::initialAction(&ACTION_TEMPLATE);;
        }
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

    car->ctrl.steer = TWorld::computeSteering(ac[DIRE], ACTIONS_DIRECTION, -0.5, 0.5);
    TWorld::applyAccelerationGear(*this, car, ac[ACC]);
}

