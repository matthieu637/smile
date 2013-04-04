#include "QLearnGen.hpp"
#include <TWorld.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>

// const sml::ActionTemplate QLearnGen::ACTION_TEMPLATE = sml::ActionTemplate( {DIR, ACC}, {QLearnGen::ACTIONS_DIRECTION, QLearnGen::ACTIONS_ACC});
const sml::ActionTemplate QLearnGen::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE}, {QLearnGen::ACTIONS_DIRECTION});

const double xmax = 12;
const double ymax = 2*M_PI;
// const double zmax = 10;
// const double umax = 7;

const int nbtiling = 3;

const int nbXinter = 10;
const int nbYinter = 16;
// const int nbZinter = zmax;
// const int nbUinter = umax;

const double xwidth = xmax/(double)nbXinter;//increase by log
const double yheight = ymax/(double)nbYinter;
// const double zprof = zmax/(double)nbZinter;
// const double uprof = umax/(double)nbUinter;

using sml::Feature;

double featuring (const State& st, std::vector<double> params) {
    double x = sml::Utils::transform(st.distanceFromMiddle, -xmax/2., xmax/2., 0., xmax);
    double y = sml::Utils::transform(st.angle, - ymax/2., ymax/2., 0., ymax);
//     double z = ac[DIR];
//     double u = ac[ACC];

    double tiling = params[0];
    double xtile = params[1];
    double ytile = params[2];
//     double ztile = params[3];
//     double utile = params[4];

    x = x + tiling*xwidth/(double)nbtiling;
    y = y + tiling*yheight/(double)nbtiling;
//      z = z + tiling*zprof/(double)nbtiling;
//     u = u + tiling*uprof/(double)nbtiling;
// if(ztile == z && utile == u)

    if(xtile*xwidth <= x && x <= (xtile+1)*xwidth)
        if(ytile*yheight <=  y && y <= (ytile+1)*yheight)

//             if(ztile*zprof <=  z && z <= (ztile+1)*zprof)
// 	      if(utile*uprof <=  u && u <= (utile+1)*uprof)
            return 1;
    return 0;
}

QLearnGen::QLearnGen(int index):Driver(index, DECISION_EACH)
{
    QLearnGradient<State>::featuredList *features = new QLearnGradient<State>::featuredList();
    for(int i = 0; i<nbtiling; i++) {
        QLearnGradient<State>::sfeaturedList *sfeatures = new QLearnGradient<State>::sfeaturedList();
        for(int x = 0; x < nbXinter; x++)
            for(int y = 0; y < nbYinter; y++)
//                 for(int z = 0; z < nbZinter; z++)
//                     for(int u = 0; u < nbUinter; u++)
                sfeatures->push_back(Feature<State>(featuring, {(double)i, (double)x, (double)y/*, (double) z, (double) u*/}));
        features->push_back(*sfeatures);
    }
    qlg = new QLearnGradient<State>(features, nbtiling* nbXinter * nbYinter /* * nbZinter  * nbUinter*/, &ACTION_TEMPLATE );
}

QLearnGen::~QLearnGen()
{

}

void QLearnGen::decision()
{
    State st = *TWorld::observe(*this);

    DAction ac = *qlg->decision(st, reward, lrate,  epsilon, lamda, discount);
    //LOG_DEBUG(" action " << ac << " recomp : " << reward);
    applyActionOn(ac, car);
}

void QLearnGen::newRace(tCarElt* car, tSituation *s) {
    Driver::newRace(car,s);

    qlg->read("smile2.data");
}

void QLearnGen::endRace() {
    qlg->write("smile2.data");
}


void QLearnGen::applyActionOn(const DAction& ac, tCarElt* car) {
    car->ctrl.steer = TWorld::computeSteering(ac[DIRE], ACTIONS_DIRECTION, -0.4, 0.4);
    /*
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
        }*/


    car->ctrl.gear = 1;
    if( car->_speed_x > 10 ) {
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = 0;
    } else {
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = 0.7;
    }
}

