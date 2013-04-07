#include "QLearnGenCmplx.hpp"
#include <TWorld.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>

using sml::Feature;

const sml::ActionTemplate QLearnGenCmplx::ACTION_TEMPLATE = sml::ActionTemplate( {DIRE, ACC}, {QLearnGenCmplx::ACTIONS_DIRECTION, TWorld::ACTIONS_ACC});

const double QLearnGenCmplx::xmax = 12;
const double QLearnGenCmplx::ymax = 2*M_PI;
const double QLearnGenCmplx::zmax = 80;

// const int nbtiling = 9;

// const int QLearnGenCmplx::nbXinter = 14;
// const int QLearnGenCmplx::nbYinter = 16;

const double QLearnGenCmplx::xwidth = xmax/(double)nbXinter;//increase by log
const double QLearnGenCmplx::yheight = ymax/(double)nbYinter;
const double QLearnGenCmplx::zheight = zmax/(double)nbZinter;

//sort(rand(1,3-1)*(12/14))
const double QLearnGenCmplx::xtiling[] = {0., 0.13181,   0.52260};
//sort(rand(1,3-1)*(2*3.14/32))
const double QLearnGenCmplx::ytiling[] = {0., 0.073144,   0.147884};
//sort(rand(1,3-1)*(80/10))
const double QLearnGenCmplx::ztiling[] = {0., 3.9516,   7.6738};

double QLearnGenCmplx::featuring (const State& st, std::vector<double> params) {
    double x = sml::Utils::transform(st.distanceFromMiddle, -xmax/2., xmax/2., 0., xmax);
    double y = sml::Utils::transform(st.angle, - ymax/2., ymax/2., 0., ymax);
    double z = sml::Utils::transform(st.speed,  -10, zmax - 10., 0., zmax);

    int tiling = (int)params[0];
    double xtile = params[1];
    double ytile = params[2];
    double ztile = params[3];

    x = x + xtiling[tiling];
    y = y + ytiling[tiling];
    z = z + ztiling[tiling];

    if(xtile*xwidth <= x && x <= (xtile+1)*xwidth)
        if(ytile*yheight <=  y && y <= (ytile+1)*yheight)
            if(ytile*yheight <=  y && y <= (ytile+1)*yheight)
                if(ztile*zheight <=  z && z <= (ztile+1)*zheight)
                    return 1;
    return 0;
}

double QLearnGenCmplx::featuring2 (const State& st, std::vector<double> params) {
    double x = sml::Utils::transform(st.distanceFromMiddle, -xmax/2., xmax/2., 0., xmax);

    double xtile = params[0];

    if(xtile*xwidth <= x && x <= (xtile+1)*xwidth)
        return 1;
    return 0;
}

double QLearnGenCmplx::featuring3 (const State& st, std::vector<double> params) {
    double y = sml::Utils::transform(st.angle, - ymax/2., ymax/2., 0., ymax);

    double ytile = params[0];

    if(ytile*yheight <=  y && y <= (ytile+1)*yheight)
        return 1;
    return 0;
}

double QLearnGenCmplx::featuring4 (const State& st, std::vector<double> params) {
    double z = sml::Utils::transform(st.speed,  -10, zmax - 10., 0., zmax);

    double ztile = params[0];

    if(ztile*zheight <=  z && z <= (ztile+1)*zheight)
        return 1;
    return 0;
}


QLearnGenCmplx::QLearnGenCmplx(int index):Driver(index, DECISION_EACH, 2)
{
//     list<int> sizes;
    QLearnGradient<State>::featuredList *features = new QLearnGradient<State>::featuredList();
    for(int i = 0; i<3; i++) {
        QLearnGradient<State>::sfeaturedList *sfeatures = new QLearnGradient<State>::sfeaturedList();
        for(int x = 0; x < nbXinter; x++)
            for(int y = 0; y < nbYinter; y++)
                for(int z = 0; z < nbZinter; z++)
                    sfeatures->push_back(Feature<State>(this->featuring, {(double)i, (double)x, (double)y, (double) z}));
        features->push_back(*sfeatures);
// 	sizes.push_back(nbXinter * nbYinter * nbZinter);
    }

    QLearnGradient<State>::sfeaturedList *sfeatures = new QLearnGradient<State>::sfeaturedList();
    for(int x = 0; x < nbXinter; x++)
        sfeatures->push_back(Feature<State>(this->featuring2, {(double)x}));
    features->push_back(*sfeatures);
//     sizes.push_back(nbXinter);

    sfeatures = new QLearnGradient<State>::sfeaturedList();
    for(int y = 0; y < nbYinter; y++)
        sfeatures->push_back(Feature<State>(this->featuring3, {(double)y}));
    features->push_back(*sfeatures);
//     sizes.push_back(nbYinter);

    sfeatures = new QLearnGradient<State>::sfeaturedList();
    for(int z = 0; z < nbZinter; z++)
        sfeatures->push_back(Feature<State>(this->featuring4, {(double)z}));
    features->push_back(*sfeatures);


    qlg = new QLearnGradient<State>(features, nbXinter + nbYinter + nbZinter + 3 * nbXinter * nbYinter * nbZinter, &ACTION_TEMPLATE, *TWorld::initialAction(&ACTION_TEMPLATE) );
}

QLearnGenCmplx::~QLearnGenCmplx()
{

}

void QLearnGenCmplx::decision()
{
    State st = *TWorld::observe(*this);
    LOG_DEBUG(st.rightDistance);

    DAction ac = *qlg->decision(st, reward, lrate,  epsilon, lamda, discount);
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

