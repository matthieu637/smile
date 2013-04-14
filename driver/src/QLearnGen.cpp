#include "QLearnGen.hpp"
#include <TWorld.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>

using sml::Feature;

const sml::ActionTemplate QLearnGen::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE, ACC}, {QLearnGen::ACTIONS_DIRECTION, TWorld::ACTIONS_ACC});
//const sml::ActionTemplate QLearnGen::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE}, {QLearnGen::ACTIONS_DIRECTION});

const double QLearnGen::xmax = 12;
const double QLearnGen::ymax = 2*M_PI;

// const int nbtiling = 9;

// const int QLearnGen::nbXinter = 14;
// const int QLearnGen::nbYinter = 16;

const double QLearnGen::xwidth = xmax/(double)nbXinter;//increase by log
const double QLearnGen::yheight = ymax/(double)nbYinter;

//sort(rand(1,7-1)*(12/14))
const double QLearnGen::xtiling[] = {0., 0.092406,   0.161428,   0.253100,   0.301486,   0.493252,   0.758365};
//sort(rand(1,7-1)*(2*3.14/16))
const double QLearnGen::ytiling[] = {0.,  0.085714,   0.094104,   0.189928,   0.224569,   0.231825,   0.332045};

double QLearnGen::featuring (const State& st, std::vector<double> params) {
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

double QLearnGen::featuring2 (const State& st, std::vector<double> params) {
    double x = sml::Utils::transform(st.distanceFromMiddle, -xmax/2., xmax/2., 0., xmax);

    double xtile = params[0];

    if(xtile*xwidth <= x && x <= (xtile+1)*xwidth)
        return 1;
    return 0;
}

double QLearnGen::featuring3 (const State& st, std::vector<double> params) {
    double y = sml::Utils::transform(st.angle, - ymax/2., ymax/2., 0., ymax);

    double ytile = params[0];

    if(ytile*yheight <=  y && y <= (ytile+1)*yheight)
        return 1;
    return 0;
}

QLearnGen::QLearnGen(int index):Driver(index, DECISION_EACH, 2)
{
    QLearnGradient<State>::featuredList *features = new QLearnGradient<State>::featuredList();
    for(int i = 0; i<7; i++) {
        QLearnGradient<State>::sfeaturedList *sfeatures = new QLearnGradient<State>::sfeaturedList();
        for(int x = 0; x < nbXinter; x++)
            for(int y = 0; y < nbYinter; y++)
                sfeatures->push_back(Feature<State>(this->featuring, {(double)i, (double)x, (double)y/*, (double) z, (double) u*/}));
        features->push_back(*sfeatures);
    }

    QLearnGradient<State>::sfeaturedList *sfeatures = new QLearnGradient<State>::sfeaturedList();
    for(int x = 0; x < nbXinter; x++)
        sfeatures->push_back(Feature<State>(this->featuring2, {(double)x}));
    features->push_back(*sfeatures);


    sfeatures = new QLearnGradient<State>::sfeaturedList();
    for(int y = 0; y < nbYinter; y++)
        sfeatures->push_back(Feature<State>(this->featuring3, {(double)y}));
    features->push_back(*sfeatures);


    qlg = new QLearnGradient<State>(features, nbXinter + nbYinter + 7 * nbXinter * nbYinter, &ACTION_TEMPLATE, *TWorld::initialAction(&ACTION_TEMPLATE) );

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

