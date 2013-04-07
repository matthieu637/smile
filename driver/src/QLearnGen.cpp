#include "QLearnGen.hpp"
#include <TWorld.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>

const sml::ActionTemplate QLearnGen::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE, ACC}, {QLearnGen::ACTIONS_DIRECTION, TWorld::ACTIONS_ACC});
//const sml::ActionTemplate QLearnGen::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE}, {QLearnGen::ACTIONS_DIRECTION});

const double xmax = 12;
const double ymax = 2*M_PI;

// const int nbtiling = 9;

const int nbXinter = 14;
const int nbYinter = 16;

const double xwidth = xmax/(double)nbXinter;//increase by log
const double yheight = ymax/(double)nbYinter;

//sort(rand(1,4-1)*(12/14))
const double xtiling[] = {0., 0.10817,   0.16954,   0.26241};
const double xtiling2[] = {0., 0.11645,   0.22497,   0.55615};
//sort(rand(1,4-1)*(2*3.14/16))
const double ytiling[] = {0., 0.048702,   0.178274,   0.185312};
const double ytiling2[] = {0., 0.075859,   0.078161,   0.170480} ;


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

double featuring2 (const State& st, std::vector<double> params) {
    double x = sml::Utils::transform(st.distanceFromMiddle, -xmax/2., xmax/2., 0., xmax);

    int tiling = (int)params[0];
    double xtile = params[1];

    x = x + xtiling2[tiling];

    if(xtile*xwidth <= x && x <= (xtile+1)*xwidth)
        return 1;
    return 0;
}

double featuring3 (const State& st, std::vector<double> params) {
    double y = sml::Utils::transform(st.angle, - ymax/2., ymax/2., 0., ymax);

    int tiling = (int)params[0];
    double ytile = params[2];

    y = y + ytiling2[tiling];

    if(ytile*yheight <=  y && y <= (ytile+1)*yheight)
        return 1;
    return 0;
}

QLearnGen::QLearnGen(int index):Driver(index, DECISION_EACH)
{
    QLearnGradient<State>::featuredList *features = new QLearnGradient<State>::featuredList();
    for(int i = 0; i<3; i++) {
        QLearnGradient<State>::sfeaturedList *sfeatures = new QLearnGradient<State>::sfeaturedList();
        for(int x = 0; x < nbXinter; x++)
            for(int y = 0; y < nbYinter; y++)
                sfeatures->push_back(Feature<State>(featuring, {(double)i, (double)x, (double)y/*, (double) z, (double) u*/}));
        features->push_back(*sfeatures);
    }
    for(int i = 0; i<1; i++) {
        QLearnGradient<State>::sfeaturedList *sfeatures = new QLearnGradient<State>::sfeaturedList();
        for(int x = 0; x < nbXinter; x++)
            for(int y = 0; y < nbYinter; y++)
                sfeatures->push_back(Feature<State>(featuring2, {(double)i, (double)x, (double)y}));
        features->push_back(*sfeatures);
    }
    for(int i = 0; i<1; i++) {
        QLearnGradient<State>::sfeaturedList *sfeatures = new QLearnGradient<State>::sfeaturedList();
        for(int x = 0; x < nbXinter; x++)
            for(int y = 0; y < nbYinter; y++)
                sfeatures->push_back(Feature<State>(featuring3, {(double)i, (double)x, (double)y}));
        features->push_back(*sfeatures);
    }

    qlg = new QLearnGradient<State>(features, 5 * nbXinter * nbYinter, &ACTION_TEMPLATE, *TWorld::initialAction(&ACTION_TEMPLATE) );

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

    car->ctrl.steer = TWorld::computeSteering(ac[DIRE], ACTIONS_DIRECTION, -0.4, 0.4);
    TWorld::applyAcceleration(car, ac[ACC]);
}

