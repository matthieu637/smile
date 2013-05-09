#include "QLearnGen.hpp"
#include <TWorld.hpp>
#include <TFeatures.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>

using sml::Feature;

const sml::ActionTemplate QLearnGen::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE, ACC}, {QLearnGen::ACTIONS_DIRECTION, TWorld::ACTIONS_ACC});
// const sml::ActionTemplate QLearnGen::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE}, {QLearnGen::ACTIONS_DIRECTION});

const double QLearnGen::road_width = 12;
const double QLearnGen::total_angle = 2*M_PI;

QLearnGen::QLearnGen(int index):Driver(index, DECISION_EACH, simu_time)
{
    QLearnGradient<State>::featuredList *features = new QLearnGradient<State>::featuredList();
    /*
    //     STATE 1D
        {
            Feature<State> f({TFeatures::_1DMiddle(road_width), {nbXinter, road_width});
            QLearnGradient<State>::sfeaturedList p(f, nbXinter );
            features->push_back(f);
        }
        {
            Feature<State> f({TFeatures::_1DAngle(total_angle), {nbYinter, total_angle});
            QLearnGradient<State>::sfeaturedList p(f, nbYinter );
            features->push_back(f);
        }
  */  //     STATE 2D
        {
            Feature<State> f({TFeatures::_1DMiddle(road_width),
                             TFeatures::_1DAngle(total_angle)},
            {nbXinter, road_width, nbYinter, total_angle});
            features->push_back(f);
        }
    // 	MIX STATE/ACTION 2D
        {
            Feature<State> f({TFeatures::_1DMiddle(road_width),
                             TFeatures::_1DAction(ACC)},
            {nbXinter, road_width, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC});
            features->push_back(f);
        }

        {
            Feature<State> f({TFeatures::_1DAngle(total_angle),
                             TFeatures::_1DAction(ACC)},
            {nbYinter, total_angle, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC});
            features->push_back(f);
        }
        {
            Feature<State> f({TFeatures::_1DMiddle(road_width),
                             TFeatures::_1DAction(DIRE)},
            {nbXinter, road_width, QLearnGen::ACTIONS_DIRECTION, QLearnGen::ACTIONS_DIRECTION});
            features->push_back(f);
        }

        {
            Feature<State> f({TFeatures::_1DAngle(total_angle),
                             TFeatures::_1DAction(DIRE)},
            {nbYinter, total_angle, QLearnGen::ACTIONS_DIRECTION, QLearnGen::ACTIONS_DIRECTION});
            features->push_back(f);
        }
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
        {nbXinter, road_width, nbYinter, total_angle, QLearnGen::ACTIONS_DIRECTION, QLearnGen::ACTIONS_DIRECTION});
        features->push_back(f);
    }
    {
        Feature<State> f( {TFeatures::_1DAction(ACC),
                           TFeatures::_1DAction(DIRE),
                           TFeatures::_1DMiddle(road_width)
                          },
        {TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGen::ACTIONS_DIRECTION, QLearnGen::ACTIONS_DIRECTION, nbXinter, road_width});
        features->push_back(f);
    }
    {
        Feature<State> f( {TFeatures::_1DAction(ACC),
                           TFeatures::_1DAction(DIRE),
                           TFeatures::_1DAngle(total_angle)
                          },
        {TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGen::ACTIONS_DIRECTION, QLearnGen::ACTIONS_DIRECTION, nbYinter, total_angle});
        features->push_back(f);
    }


    unsigned int nbFeature = 0;
    for(QLearnGradient<State>::fLiterator flist = features->begin() ; flist != features->end(); ++flist) {
        nbFeature += flist->getSize();
    }

    qlg = new QLearnGradient<State>(features, nbFeature, &ACTION_TEMPLATE, *TWorld::initialAction(&ACTION_TEMPLATE) , conf);
}

QLearnGen::~QLearnGen()
{

}

void QLearnGen::decision()
{
    State st = *TWorld::observe(*this);
    
    if(car->_speed_x < 15 && car->_distRaced < 15) {
        DAction* ac;
        ac = TWorld::initialAction(&ACTION_TEMPLATE);
        ac->set(ACC, 3);
        applyActionOn(*ac, car);
        qlg->update(*ac);
    }
    else {
        const DAction* ac;
        if(learn) {
            ac = qlg->learn(st, reward, lrate,  epsilon, lamda, discount, false);
            LOG_DEBUG(" action " << *ac << " recomp : " << reward);
        }
        else ac = qlg->decision(st);

        applyActionOn(*ac, car);
    }
}

void QLearnGen::newRace(tCarElt* car, tSituation *s) {
    Driver::newRace(car,s);

    qlg->read("smile2.data");
}

void QLearnGen::endRace() {
    Driver::endRace();
    qlg->write("smile2.data");
}

sml::LearnStat* QLearnGen::getAlgo() {
    return qlg;
}

void QLearnGen::applyActionOn(const DAction& ac, tCarElt* car) {
    car->ctrl.steer = TWorld::computeSteering(ac[DIRE], ACTIONS_DIRECTION, -0.4, 0.4);
    TWorld::applyAcceleration(car, ac[ACC]);

//     car->ctrl.gear = 1;
//     car->ctrl.brakeCmd = 0;
//
//     if({car->_speed_x  > 10)
//         car->ctrl.accelCmd = 0;
//     else
//         car->ctrl.accelCmd = 0.3;
}

