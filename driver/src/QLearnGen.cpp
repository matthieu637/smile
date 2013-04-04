#include "QLearnGen.hpp"
#include <TWorld.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>

const sml::ActionTemplate QLearnGen::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE, ACC}, {QLearnGen::ACTIONS_DIRECTION, QLearnGen::ACTIONS_ACC});
//const sml::ActionTemplate QLearnGen::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE}, {QLearnGen::ACTIONS_DIRECTION});

const double xmax = 12;
const double ymax = 2*M_PI;

const int nbtiling = 4;

const int nbXinter = 10;
const int nbYinter = 16;

const double xwidth = xmax/(double)nbXinter;//increase by log
const double yheight = ymax/(double)nbYinter;

//sort(rand(1,4-1)*(12/10))
const double xtiling[nbtiling] = {0., 0.51708,   0.89841,   0.92744};
//sort(rand(1,4-1)*(2*3.14/16))
const double ytiling[nbtiling] = {0., 0.053834,   0.140523,   0.280546};

using sml::Feature;

double featuring (const State& st, std::vector<double> params) {
    double x = sml::Utils::transform(st.distanceFromMiddle, -xmax/2., xmax/2., 0., xmax);
    double y = sml::Utils::transform(st.angle, - ymax/2., ymax/2., 0., ymax);

    int tiling = (int)params[0];
    double xtile = params[1];
    double ytile = params[2];

    x = x + xtiling[tiling];
    y = y + ytiling[tiling];

    if(xtile*xwidth <= x && x <= (xtile+1)*xwidth)
        if(ytile*yheight <=  y && y <= (ytile+1)*yheight)
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
                sfeatures->push_back(Feature<State>(featuring, {(double)i, (double)x, (double)y/*, (double) z, (double) u*/}));
        features->push_back(*sfeatures);
    }
    qlg = new QLearnGradient<State>(features, nbtiling * nbXinter * nbYinter, &ACTION_TEMPLATE, TWorld::initialState() );
    
}

QLearnGen::~QLearnGen()
{

}

void QLearnGen::decision()
{
    State st = *TWorld::observe(*this);
    LOG_DEBUG(st.rightDistance);

    DAction ac = *qlg->decision(st, reward, lrate,  epsilon, lamda, discount);
    LOG_DEBUG(" action " << ac << " recomp : " << reward);
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
    car->ctrl.steer = TWorld::computeSteering(ac[DIRE], ACTIONS_DIRECTION, -0.3, 0.3);

    unsigned int accel = ac[ACC];

    switch(accel)
    {
    case 0:
        car->ctrl.gear = -1;
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = 1;
        break;
    case 1:
        car->ctrl.brakeCmd = 1;
        car->ctrl.accelCmd = 0;
        break;
    case 2:
        car->ctrl.brakeCmd = 1;
        car->ctrl.accelCmd = 0;
        break;
    case 3:
        car->ctrl.gear = 1;
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = 1;
        break;
    }
}

