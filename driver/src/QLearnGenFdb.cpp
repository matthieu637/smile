#include "QLearnGenFdb.hpp"
#include <TWorld.hpp>
#include <TFeatures.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>

using sml::Feature;

const sml::ActionTemplate QLearnGenFdb::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE, ACC}, {QLearnGenFdb::ACTIONS_DIRECTION, TWorld::ACTIONS_ACC});
// const sml::ActionTemplate QLearnGenFdb::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE}, {QLearnGenFdb::ACTIONS_DIRECTION});

const double QLearnGenFdb::road_width = 14;
const double QLearnGenFdb::total_angle = 2*M_PI;

QLearnGenFdb::QLearnGenFdb(int index):
    DriverFeedback(index, DECISION_EACH, 2,
{{GFCTRL_TYPE_KEYBOARD, 's'}, {GFCTRL_TYPE_KEYBOARD, 'd'}, {GFCTRL_TYPE_KEYBOARD, 'f'}, {GFCTRL_TYPE_KEYBOARD, 'g'}})
{
    QLearnGradient<State>::featuredList *features = new QLearnGradient<State>::featuredList();
    /*
    //     STATE 1D
        {
            Feature<State> f(TFeatures::_1DMiddle(road_width), {nbXinter, road_width});
            QLearnGradient<State>::sfeaturedList p(f, nbXinter );
            features->push_back(p);
        }
        {
            Feature<State> f(TFeatures::_1DAngle(total_angle), {nbYinter, total_angle});
            QLearnGradient<State>::sfeaturedList p(f, nbYinter );
            features->push_back(p);
        }*/
//     STATE 2D
    {
        Feature<State> f(TFeatures::_1DMiddle(road_width),
        TFeatures::_1DAngle(total_angle),
        {nbXinter, road_width, nbYinter, total_angle});
        QLearnGradient<State>::sfeaturedList p(f, nbXinter * nbYinter );
        features->push_back(p);
    }

    {
        Feature<State> f(TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DAction(ACC),
        {nbXinter, road_width, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC});
        QLearnGradient<State>::sfeaturedList p(f, nbXinter * TWorld::ACTIONS_ACC );
        features->push_back(p);
    }
// 	MIX STATE/ACTION 2D
    {
        Feature<State> f(TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DAction(ACC),
        {nbYinter, total_angle, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC});
        QLearnGradient<State>::sfeaturedList p(f, nbYinter * TWorld::ACTIONS_ACC );
        features->push_back(p);
    }
    {
        Feature<State> f(TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DAction(DIRE),
        {nbXinter, road_width, QLearnGenFdb::ACTIONS_DIRECTION, QLearnGenFdb::ACTIONS_DIRECTION});
        QLearnGradient<State>::sfeaturedList p(f, nbXinter * QLearnGenFdb::ACTIONS_DIRECTION );
        features->push_back(p);
    }

    {
        Feature<State> f(TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DAction(DIRE),
        {nbYinter, total_angle, QLearnGenFdb::ACTIONS_DIRECTION, QLearnGenFdb::ACTIONS_DIRECTION});
        QLearnGradient<State>::sfeaturedList p(f, nbYinter * QLearnGenFdb::ACTIONS_DIRECTION );
        features->push_back(p);
    }
// 	MIX STATE/ACTION 3D
    {
        Feature<State> f(TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DAction(ACC),
        {nbXinter, road_width, nbYinter, total_angle, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC});
        QLearnGradient<State>::sfeaturedList p(f, nbXinter * nbYinter * TWorld::ACTIONS_ACC );
        features->push_back(p);
    }
    {
        Feature<State> f(TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DAction(DIRE),
        {nbXinter, road_width, nbYinter, total_angle, QLearnGenFdb::ACTIONS_DIRECTION, QLearnGenFdb::ACTIONS_DIRECTION});
        QLearnGradient<State>::sfeaturedList p(f, nbXinter * nbYinter * QLearnGenFdb::ACTIONS_DIRECTION );
        features->push_back(p);
    }
    {
        Feature<State> f(TFeatures::_1DAction(ACC),
                         TFeatures::_1DAction(DIRE),
                         TFeatures::_1DMiddle(road_width),
        {TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenFdb::ACTIONS_DIRECTION, QLearnGenFdb::ACTIONS_DIRECTION, nbXinter, road_width});
        QLearnGradient<State>::sfeaturedList p(f, TWorld::ACTIONS_ACC *  QLearnGenFdb::ACTIONS_DIRECTION * nbXinter );
        features->push_back(p);
    }
    {
        Feature<State> f(TFeatures::_1DAction(ACC),
                         TFeatures::_1DAction(DIRE),
                         TFeatures::_1DAngle(total_angle),
        {TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenFdb::ACTIONS_DIRECTION, QLearnGenFdb::ACTIONS_DIRECTION, nbYinter, total_angle});
        QLearnGradient<State>::sfeaturedList p(f, TWorld::ACTIONS_ACC *  QLearnGenFdb::ACTIONS_DIRECTION * nbYinter );
        features->push_back(p);
    }


    unsigned int nbFeature = 0;
    for(QLearnGradient<State>::fLiterator flist = features->begin() ; flist != features->end(); ++flist) {
        QLearnGradient<State>::sfeaturedList sl = *flist;
        nbFeature += sl.second;
    }

    qlg = new QLearnGradient<State>(features, nbFeature, &ACTION_TEMPLATE, *TWorld::initialAction(&ACTION_TEMPLATE) );
}

QLearnGenFdb::~QLearnGenFdb()
{

}

void QLearnGenFdb::decision()
{
    if(car->_speed_x < 15 && car->_distRaced < 15) {
        DAction ac;
        ac = *TWorld::initialAction(&ACTION_TEMPLATE);
        ac.set(ACC, 3);
        applyActionOn(ac, car);
    }
    else {
        double expertReward = 0;

        if(keyInfo['g'].state)
            expertReward=1000;
        else if(keyInfo['f'].state)
            expertReward=400;
        else if(keyInfo['d'].state)
            expertReward=-400;
        else if(keyInfo['s'].state)
            expertReward=-1000;

        const DAction* ac;
        State st = *TWorld::observe(*this);
        ac = qlg->learn(st, reward + expertReward, lrate,  epsilon, lamda, discount, false);
        LOG_DEBUG(" action " << *ac << " recomp : " << reward << " expert : " << expertReward );
        applyActionOn(*ac, car);
    }
}

void QLearnGenFdb::newRace(tCarElt* car, tSituation *s) {
    Driver::newRace(car,s);

    qlg->read("smile4.data");
}

void QLearnGenFdb::endRace() {
    qlg->write("smile4.data");
}


void QLearnGenFdb::applyActionOn(const DAction& ac, tCarElt* car) {
    car->ctrl.steer = TWorld::computeSteering(ac[DIRE], ACTIONS_DIRECTION, -0.4, 0.4);
    TWorld::applyAcceleration(car, ac[ACC]);

//     car->ctrl.gear = 1;
//     car->ctrl.brakeCmd = 0;
//
//     if(car->_speed_x  > 10)
//         car->ctrl.accelCmd = 0;
//     else
//         car->ctrl.accelCmd = 0.3;
}

