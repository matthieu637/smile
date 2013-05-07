#include "QLearnGenCmplx.hpp"
#include <TWorld.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>
#include <TFeatures.hpp>

using sml::Feature;

const sml::ActionTemplate QLearnGenCmplx::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE, ACC}, {QLearnGenCmplx::ACTIONS_DIRECTION, TWorld::ACTIONS_ACC});

const double QLearnGenCmplx::road_width = 12;
const double QLearnGenCmplx::total_angle = 2*M_PI;
const double QLearnGenCmplx::total_speed = 60;


QLearnGenCmplx::QLearnGenCmplx(int index):Driver(index, DECISION_EACH, 2)
{
    QLearnGradient<State>::featuredList *features = new QLearnGradient<State>::featuredList();

    //     STATE 1D
// ...

    //     STATE 2D
// ...

    //     STATE 3D == bias

    {
        Feature<State> f(TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DSpeed(total_speed),
        {nbXinter, road_width, nbYinter, total_angle, nbZinter, total_speed});
        QLearnGradient<State>::sfeaturedList p(f, nbXinter * nbYinter * nbZinter );
        features->push_back(p);
    }

    // 	MIX STATE/ACTION 2D
// ...
// 	MIX STATE/ACTION 3D
    {   //abd
        Feature<State> f(TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DAction(ACC),
        {nbXinter, road_width, nbYinter, total_angle, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC});
        QLearnGradient<State>::sfeaturedList p(f, nbXinter * nbYinter * TWorld::ACTIONS_ACC );
        features->push_back(p);
    }
    {   //abe
        Feature<State> f(TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DAction(DIRE),
        {nbXinter, road_width, nbYinter, total_angle, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION});
        QLearnGradient<State>::sfeaturedList p(f, nbXinter * nbYinter * QLearnGenCmplx::ACTIONS_DIRECTION );
        features->push_back(p);
    }
    {   //acd
        Feature<State> f(TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DSpeed(total_speed),
                         TFeatures::_1DAction(ACC),
        {nbXinter, road_width, nbZinter, total_speed, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC});
        QLearnGradient<State>::sfeaturedList p(f, nbXinter * nbZinter * TWorld::ACTIONS_ACC );
        features->push_back(p);
    }
    {   //ace
        Feature<State> f(TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DSpeed(total_speed),
                         TFeatures::_1DAction(DIRE),
        {nbXinter, road_width, nbZinter, total_speed, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION});
        QLearnGradient<State>::sfeaturedList p(f, nbXinter * nbZinter * QLearnGenCmplx::ACTIONS_DIRECTION );
        features->push_back(p);
    }
    {   //bcd
        Feature<State> f(TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DSpeed(total_speed),
                         TFeatures::_1DAction(ACC),
        {nbYinter, total_angle, nbZinter, total_speed, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC});
        QLearnGradient<State>::sfeaturedList p(f, nbYinter * nbZinter * TWorld::ACTIONS_ACC );
        features->push_back(p);
    }
    {   //bce
        Feature<State> f(TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DSpeed(total_speed),
                         TFeatures::_1DAction(DIRE),
        {nbYinter, total_angle, nbZinter, total_speed, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION});
        QLearnGradient<State>::sfeaturedList p(f, nbYinter * nbZinter * QLearnGenCmplx::ACTIONS_DIRECTION );
        features->push_back(p);
    }
    {   //ade
        Feature<State> f(TFeatures::_1DAction(ACC),
                         TFeatures::_1DAction(DIRE),
                         TFeatures::_1DMiddle(road_width),
        {TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION, nbXinter, road_width});
        QLearnGradient<State>::sfeaturedList p(f, TWorld::ACTIONS_ACC *  QLearnGenCmplx::ACTIONS_DIRECTION * nbXinter );
        features->push_back(p);
    }
    {   //bde
        Feature<State> f(TFeatures::_1DAction(ACC),
                         TFeatures::_1DAction(DIRE),
                         TFeatures::_1DAngle(total_angle),
        {TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION, nbYinter, total_angle});
        QLearnGradient<State>::sfeaturedList p(f, TWorld::ACTIONS_ACC *  QLearnGenCmplx::ACTIONS_DIRECTION * nbYinter );
        features->push_back(p);
    }

    {   //cde
        Feature<State> f(TFeatures::_1DAction(ACC),
                         TFeatures::_1DAction(DIRE),
                         TFeatures::_1DSpeed(total_speed),
        {TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION, nbZinter, total_speed});
        QLearnGradient<State>::sfeaturedList p(f, TWorld::ACTIONS_ACC *  QLearnGenCmplx::ACTIONS_DIRECTION * nbZinter );
        features->push_back(p);
    }
// 	MIX STATE/ACTION 4D

    {
        Feature<State> f(TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DSpeed(total_speed),
                         TFeatures::_1DAction(ACC),
        {nbXinter, road_width, nbYinter, total_angle, nbZinter, total_speed, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC});
        QLearnGradient<State>::sfeaturedList p(f, nbXinter * nbYinter * nbZinter * TWorld::ACTIONS_ACC );
        features->push_back(p);
    }

    {
        Feature<State> f(TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DSpeed(total_speed),
                         TFeatures::_1DAction(DIRE),
        {nbXinter, road_width, nbYinter, total_angle, nbZinter, total_speed, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION});
        QLearnGradient<State>::sfeaturedList p(f, nbXinter * nbYinter * nbZinter * QLearnGenCmplx::ACTIONS_DIRECTION );
        features->push_back(p);
    }

    {
        Feature<State> f(TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DAction(ACC),
                         TFeatures::_1DAction(DIRE),
        {nbXinter, road_width, nbYinter, total_angle, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION});
        QLearnGradient<State>::sfeaturedList p(f, nbXinter * nbYinter * TWorld::ACTIONS_ACC * QLearnGenCmplx::ACTIONS_DIRECTION );
        features->push_back(p);
    }

    {
        Feature<State> f(TFeatures::_1DMiddle(road_width),
                         TFeatures::_1DSpeed(total_speed),
                         TFeatures::_1DAction(ACC),
                         TFeatures::_1DAction(DIRE),
        {nbXinter, road_width, nbZinter, total_speed, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION});
        QLearnGradient<State>::sfeaturedList p(f, nbXinter * nbZinter * TWorld::ACTIONS_ACC * QLearnGenCmplx::ACTIONS_DIRECTION );
        features->push_back(p);
    }

    {
        Feature<State> f(TFeatures::_1DAngle(total_angle),
                         TFeatures::_1DSpeed(total_speed),
                         TFeatures::_1DAction(ACC),
                         TFeatures::_1DAction(DIRE),
        {nbYinter, total_angle, nbZinter, total_speed, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC, QLearnGenCmplx::ACTIONS_DIRECTION, QLearnGenCmplx::ACTIONS_DIRECTION});
        QLearnGradient<State>::sfeaturedList p(f, nbYinter * nbZinter * TWorld::ACTIONS_ACC * QLearnGenCmplx::ACTIONS_DIRECTION );
        features->push_back(p);
    }


    unsigned int nbFeature = 0;
    for(QLearnGradient<State>::fLiterator flist = features->begin() ; flist != features->end(); ++flist) {
        QLearnGradient<State>::sfeaturedList sl = *flist;
        nbFeature += sl.second;
    }

    qlg = new QLearnGradient<State>(features, nbFeature, &ACTION_TEMPLATE, *TWorld::initialAction(&ACTION_TEMPLATE) );
}

QLearnGenCmplx::~QLearnGenCmplx()
{

}

void QLearnGenCmplx::decision()
{
    State st = *TWorld::observe(*this);
    DAction ac = *qlg->learn(st, reward, lrate,  epsilon, lamda, discount, false);
    LOG_DEBUG(" action " << ac << " recomp : " << reward << " " << car->_speed_x);
    applyActionOn(ac, car);
}

void QLearnGenCmplx::newRace(tCarElt* car, tSituation *s) {
    Driver::newRace(car,s);

    qlg->read("smile3.data");
}

void QLearnGenCmplx::endRace() {
    qlg->write("smile3.data");
}


void QLearnGenCmplx::applyActionOn(const DAction& ac, tCarElt* car) {

    car->ctrl.steer = TWorld::computeSteering(ac[DIRE], ACTIONS_DIRECTION, -0.4, 0.4);
    TWorld::applyAccelerationGear(*this, car, ac[ACC]);
}

