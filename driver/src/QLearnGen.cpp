#include "QLearnGen.hpp"
#include <TWorld.hpp>
#include <TFeatures.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>
#include <boost/bind.hpp>
#include <sml/Feature.hpp>

using sml::Feature;

// const sml::ActionTemplate QLearnGen::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE, ACC}, {QLearnGen::ACTIONS_DIRECTION, TWorld::ACTIONS_ACC});
const sml::ActionTemplate QLearnGen::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE}, {QLearnGen::ACTIONS_DIRECTION});

const double QLearnGen::road_width = 12;
const double QLearnGen::total_angle = 2*M_PI;

const sml::RLParam QLearnGen::conf = {0.05, 0.01, 0.95, 0.95, false, 0., 8};

const double QLearnGen::nbXinter = 14; //14
const double QLearnGen::nbYinter = 16; //16

class Functor1DState1 : public Functor1D
{
public:
    Functor1DState1(int i):i(i) {
        float p = sml::Utils::randin(0.,(2.*QLearnGen::road_width)/QLearnGen::nbXinter) ;
        float v = sml::Utils::randin(0.,(2.*QLearnGen::total_angle)/QLearnGen::nbYinter) ;
        randomize.push_back(p);
        randomize.push_back(v);
    }

    double callMiddle(const State& st, const DAction&)
    {
        return sml::Utils::transform(st.distanceFromMiddle, -QLearnGen::road_width/2., QLearnGen::road_width/2., 0., QLearnGen::road_width) - (QLearnGen::road_width/QLearnGen::nbXinter) + randomize[0];
//         return sml::Utils::transform(st.position, -1.2, 0.6, 0., 1.7) - (1.7/K) +  randomize[0];
    }

    double callAngle(const State& st, const DAction&)
    {
//         return sml::Utils::transform(st.velocity, -0.07, 0.07, 0., 0.14) - (0.14/K) + randomize[1] ;
        return sml::Utils::transform(st.angle, -QLearnGen::total_angle/2., QLearnGen::total_angle/2., 0., QLearnGen::total_angle) - (QLearnGen::total_angle/QLearnGen::nbYinter) + randomize[1];
    }

    double callStuck(const State& st, const DAction&) {
        return st.stuck ? 1 : 0;
    }

    double callDirection(const State&, const DAction& ac)
    {
        return ac[DIRE];
    }

    double callAcc(const State&, const DAction& ac) {
        return ac[ACC];
    }

    double i;
};


template<>
f_crea<State> Factory::createFeature(int tiling) {

    Functor1DState1* inst_call = new Functor1DState1(tiling);
    typename Feature<State>::featuring1D fonctor1 = boost::bind(&Functor1DState1::callMiddle, inst_call, _1, _2);
    typename Feature<State>::featuring1D fonctor2 = boost::bind(&Functor1DState1::callAngle, inst_call, _1, _2);
    typename Feature<State>::featuring1D fonctor4 = boost::bind(&Functor1DState1::callDirection, inst_call, _1, _2);

    Feature<State>* f = new Feature<State>( {fonctor1, fonctor2,fonctor4},
    { QLearnGen::nbXinter, QLearnGen::road_width, QLearnGen::nbYinter, QLearnGen::total_angle, QLearnGen::ACTIONS_DIRECTION, QLearnGen::ACTIONS_DIRECTION}, inst_call);

    return {f, inst_call};
}

// template<>
// f_crea_list<State> Factory::additionnalFeature(RLParam param) {
//     f_crea_list<State> begin = {new featuredList<State>(), new list<Functor1D* >};
// 
//     for(int i=0; i < param.tiling; i++) {
//         Functor1DState1* inst_call = new Functor1DState1(i);
//         typename Feature<State>::featuring1D fonctor1 = boost::bind(&Functor1DState1::callMiddle, inst_call, _1, _2);
//         typename Feature<State>::featuring1D fonctor2 = boost::bind(&Functor1DState1::callAngle, inst_call, _1, _2);
//         typename Feature<State>::featuring1D fonctor4 = boost::bind(&Functor1DState1::callAcc, inst_call, _1, _2);
// 
//         Feature<State>* f = new Feature<State>( {fonctor1, fonctor2,fonctor4},
//         { QLearnGen::nbXinter, QLearnGen::road_width, QLearnGen::nbYinter, QLearnGen::total_angle, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC}, inst_call);
// 	begin.f->push_back(f);
//         begin.inst_call->push_back(inst_call);
//     }
//     
//     for(int i=0; i < param.tiling; i++) {
//         Functor1DState1* inst_call = new Functor1DState1(i);
//         typename Feature<State>::featuring1D fonctor1 = boost::bind(&Functor1DState1::callStuck, inst_call, _1, _2);
//         typename Feature<State>::featuring1D fonctor2 = boost::bind(&Functor1DState1::callDirection, inst_call, _1, _2);
//         Feature<State>* f = new Feature<State>( {fonctor1, fonctor2}, {2, 2, QLearnGen::ACTIONS_DIRECTION, QLearnGen::ACTIONS_DIRECTION}, inst_call);
//         begin.f->push_back(f);
//         begin.inst_call->push_back(inst_call);
//     }
//   
//     for(int i=0; i < param.tiling; i++) {
//         Functor1DState1* inst_call = new Functor1DState1(i);
//         typename Feature<State>::featuring1D fonctor1 = boost::bind(&Functor1DState1::callStuck, inst_call, _1, _2);
//         typename Feature<State>::featuring1D fonctor2 = boost::bind(&Functor1DState1::callAcc, inst_call, _1, _2);
//         Feature<State>* f = new Feature<State>( {fonctor1, fonctor2}, {2, 2, TWorld::ACTIONS_ACC, TWorld::ACTIONS_ACC}, inst_call);
//         begin.f->push_back(f);
//         begin.inst_call->push_back(inst_call);
//     }
// 
//     return begin;
// }



QLearnGen::QLearnGen(int index):Driver(index, DECISION_EACH, simu_time)
{
    featureData<State> features;
    unsigned int nbFeature = 0;
    features = Factory::createFeatures<State>(conf);
    for(fLiterator<State> flist = features.func->begin() ; flist != features.func->end(); ++flist) {
        nbFeature += (*flist)->getSize();
    }

    qlg = new SarsaGradient<State>(features.func, nbFeature, &ACTION_TEMPLATE, *TWorld::initialState(), *TWorld::initialAction(&ACTION_TEMPLATE), conf);
}

QLearnGen::~QLearnGen()
{

}



void QLearnGen::decision()
{
    State st = *TWorld::observe(*this);

    if(car->_speed_x < 15 && car->_distRaced < 15 && stime < 2.3) {
        LOG_DEBUG("begin " << stime);
        DAction* ac;
        ac = TWorld::initialAction(&ACTION_TEMPLATE);
//         ac->set(ACC, 3);
        applyActionOn(*ac, car);
// 	TODO:
//         qlg->update(*ac);
    }
    else {
        const DAction* ac;
        if(learn) {
            ac = qlg->learn(st, reward, false);
            LOG_DEBUG(" action " << *ac << " (" << ac->hash() << ") recomp : " << reward);
        }
        else ac = qlg->decision(st, false);

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
    writePerf("smile2.data");
}

void QLearnGen::applyActionOn(const DAction& ac, tCarElt* car) {
    car->ctrl.steer = TWorld::computeSteering(ac[DIRE], ACTIONS_DIRECTION, -0.3, 0.3);
//     TWorld::applyAcceleration(car, ac[ACC]);

    if(car->_speed_x  > 15) {
        car->ctrl.gear = 1;
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = 0;
    }
    else {
	car->ctrl.gear = 1;
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = 0.3;
    }

}

