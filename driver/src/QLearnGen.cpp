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

//sort(rand(1,4-1)*(12/14))
const double QLearnGen::xtiling[] = {0., 0.10817,   /*0.16954,*/   0.26241};
//sort(rand(1,4-1)*(2*3.14/16))
const double QLearnGen::ytiling[] = {0., /*0.048702, */  0.178274,   0.185312};
//sort(rand(1,3-1))
const double QLearnGen::atiling[] = {0., 0.22172,   0.73218};
//sort(rand(1,3-1))
const double QLearnGen::dtiling[] = {0., 0.43401,   0.77456};

double QLearnGen::featuring1 (const State& st, const DAction& ac, std::vector<double> params) {
    int acc = ac[ACC];
    double x = sml::Utils::transform(st.distanceFromMiddle, -xmax/2., xmax/2., 0., xmax);

    int tiling = (int)params[0];
    double a = params[1];
    double xtile = params[2];

    x = x + xtiling[tiling];
    acc += atiling[tiling];

    if(a <= acc && acc <= (a+1))
        if(xtile*xwidth <= x && x <= (xtile+1)*xwidth)
            return 1;
    return 0;
}

double QLearnGen::featuring2 (const State& st, const DAction& ac, std::vector<double> params) {
    int acc = ac[ACC];
    double y = sml::Utils::transform(st.angle, - ymax/2., ymax/2., 0., ymax);

    int tiling = (int)params[0];
    double a = params[1];
    double ytile = params[2];

    y = y + ytiling[tiling];
    acc += atiling[tiling];

    if(a <= acc && acc <= (a+1))
        if(ytile*yheight <=  y && y <= (ytile+1)*yheight)
            return 1;
    return 0;
}

double QLearnGen::featuring3 (const State& st, const DAction& ac, std::vector<double> params) {
    int dir = ac[DIRE];
    double x = sml::Utils::transform(st.distanceFromMiddle, -xmax/2., xmax/2., 0., xmax);

    int tiling = (int)params[0];
    double d = params[1];
    double xtile = params[2];

    x = x + xtiling[tiling];
    dir += dtiling[tiling];

    if(d <=  dir && dir <= (d+1))
        if(xtile*xwidth <= x && x <= (xtile+1)*xwidth)
            return 1;
    return 0;
}

double QLearnGen::featuring4 (const State& st, const DAction& ac, std::vector<double> params) {
    int dir = ac[DIRE];
    double y = sml::Utils::transform(st.angle, - ymax/2., ymax/2., 0., ymax);

    int tiling = (int)params[0];
    double d = params[1];
    double ytile = params[2];

    y = y + ytiling[tiling];
    dir += dtiling[tiling];

    if(d <=  dir && dir <= (d+1))
        if(ytile*yheight <=  y && y <= (ytile+1)*yheight)
            return 1;
    return 0;
}

double QLearnGen::featuring3D1 (const State& st, const DAction& ac, std::vector<double> params) {
    int acc = ac[ACC];
    double x = sml::Utils::transform(st.distanceFromMiddle, -xmax/2., xmax/2., 0., xmax);
    double y = sml::Utils::transform(st.angle, - ymax/2., ymax/2., 0., ymax);

    int tiling = (int)params[0];
    double a = params[1];
    double xtile = params[2];
    double ytile = params[3];

    acc += atiling[tiling];
    x = x + xtiling[tiling];
    y = y + ytiling[tiling];

    if(a <= acc && acc <= (a+1))
        if(xtile*xwidth <= x && x <= (xtile+1)*xwidth)
            if(ytile*yheight <=  y && y <= (ytile+1)*yheight)
                return 1;
    return 0;
}

double QLearnGen::featuring3D2 (const State& st, const DAction& ac, std::vector<double> params) {
    int dir = ac[DIRE];
    double x = sml::Utils::transform(st.distanceFromMiddle, -xmax/2., xmax/2., 0., xmax);
    double y = sml::Utils::transform(st.angle, - ymax/2., ymax/2., 0., ymax);

    int tiling = (int)params[0];
    double d = params[1];
    double xtile = params[2];
    double ytile = params[3];

    dir += dtiling[tiling];
    x = x + xtiling[tiling];
    y = y + ytiling[tiling];

    if(d <=  dir && dir <= (d+1))
        if(xtile*xwidth <= x && x <= (xtile+1)*xwidth)
            if(ytile*yheight <=  y && y <= (ytile+1)*yheight)
                return 1;
    return 0;
}

double QLearnGen::featuring3D3 (const State& st, const DAction& ac, std::vector<double> params) {
    int acc = ac[ACC];
    int dir = ac[DIRE];
    double x = sml::Utils::transform(st.distanceFromMiddle, -xmax/2., xmax/2., 0., xmax);

    int tiling = (int)params[0];
    double a = params[1];
    double d = params[2];
    double xtile = params[3];

    acc += atiling[tiling];
    dir += dtiling[tiling];
    x = x + xtiling[tiling];

    if(a <= acc && acc <= (a+1))
        if(d <=  dir && dir <= (d+1))
            if(xtile*xwidth <= x && x <= (xtile+1)*xwidth)
                return 1;
    return 0;
}

double QLearnGen::featuring3D4 (const State& st, const DAction& ac, std::vector<double> params) {
    int acc = ac[ACC];
    int dir = ac[DIRE];
    double y = sml::Utils::transform(st.angle, - ymax/2., ymax/2., 0., ymax);

    int tiling = (int)params[0];
    double a = params[1];
    double d = params[2];
    double ytile = params[3];

    acc += atiling[tiling];
    dir += dtiling[tiling];
    y = y + ytiling[tiling];

    if(a <= acc && acc <= (a+1))
        if(d <=  dir && dir <= (d+1))
            if(ytile*yheight <=  y && y <= (ytile+1)*yheight)
                return 1;
    return 0;
}


QLearnGen::QLearnGen(int index):Driver(index, DECISION_EACH, 2)
{
    const int nbTiling = 1;
    QLearnGradient<State>::featuredList *features = new QLearnGradient<State>::featuredList();
    /*
    for(int i = 0; i<nbTiling; i++) {
        QLearnGradient<State>::sfeaturedList *sfeatures = new QLearnGradient<State>::sfeaturedList();
        for(int a = 0; a < TWorld::ACTIONS_ACC; a++)
            for(int x = 0; x < nbXinter; x++)
                sfeatures->push_back(Feature<State>(this->featuring1, {(double)i, (double)a, (double)x}));
        features->push_back(*sfeatures);
    }

    for(int i = 0; i<nbTiling; i++) {
        QLearnGradient<State>::sfeaturedList *sfeatures = new QLearnGradient<State>::sfeaturedList();
        for(int a = 0; a < TWorld::ACTIONS_ACC; a++)
            for(int y = 0; y < nbYinter; y++)
                sfeatures->push_back(Feature<State>(this->featuring2, {(double)i, (double)a, (double)y}));
        features->push_back(*sfeatures);
    }

    for(int i = 0; i<nbTiling; i++) {
        QLearnGradient<State>::sfeaturedList *sfeatures = new QLearnGradient<State>::sfeaturedList();
        for(int d = 0; d < QLearnGen::ACTIONS_DIRECTION; d++)
            for(int x = 0; x < nbXinter; x++)
                sfeatures->push_back(Feature<State>(this->featuring3, {(double)i, (double)d, (double)x}));
        features->push_back(*sfeatures);
    }

    for(int i = 0; i<nbTiling; i++) {
        QLearnGradient<State>::sfeaturedList *sfeatures = new QLearnGradient<State>::sfeaturedList();
        for(int d = 0; d < QLearnGen::ACTIONS_DIRECTION; d++)
            for(int y = 0; y < nbYinter; y++)
                sfeatures->push_back(Feature<State>(this->featuring4, {(double)i, (double)d, (double)y}));
        features->push_back(*sfeatures);
    }

*/
    for(int i = 0; i<nbTiling; i++) {
        QLearnGradient<State>::sfeaturedList *sfeatures = new QLearnGradient<State>::sfeaturedList();
        for(int a = 0; a < TWorld::ACTIONS_ACC; a++)
            for(int x = 0; x < nbXinter; x++)
                for(int y = 0; y < nbYinter; y++)
                    sfeatures->push_back(Feature<State>(this->featuring3D1, {(double)i, (double)a, (double)x, (double) y}));
        features->push_back(*sfeatures);
    }

    for(int i = 0; i<nbTiling; i++) {
        QLearnGradient<State>::sfeaturedList *sfeatures = new QLearnGradient<State>::sfeaturedList();
        for(int d = 0; d < QLearnGen::ACTIONS_DIRECTION; d++)
            for(int x = 0; x < nbXinter; x++)
                for(int y = 0; y < nbYinter; y++)
                    sfeatures->push_back(Feature<State>(this->featuring3D2, {(double)i, (double)d,(double)x, (double)y}));
        features->push_back(*sfeatures);
    }

    for(int i = 0; i<nbTiling; i++) {
        QLearnGradient<State>::sfeaturedList *sfeatures = new QLearnGradient<State>::sfeaturedList();
        for(int a = 0; a < TWorld::ACTIONS_ACC; a++)
            for(int d = 0; d < QLearnGen::ACTIONS_DIRECTION; d++)
                for(int x = 0; x < nbXinter; x++)
                    sfeatures->push_back(Feature<State>(this->featuring3D3, {(double)i, (double)a, (double)d, (double)x}));
        features->push_back(*sfeatures);
    }

    for(int i = 0; i<nbTiling; i++) {
        QLearnGradient<State>::sfeaturedList *sfeatures = new QLearnGradient<State>::sfeaturedList();
        for(int a = 0; a < TWorld::ACTIONS_ACC; a++)
            for(int d = 0; d < QLearnGen::ACTIONS_DIRECTION; d++)
                for(int y = 0; y < nbYinter; y++)
                    sfeatures->push_back(Feature<State>(this->featuring3D4, {(double)i, (double)a, (double)d, (double)y}));
        features->push_back(*sfeatures);
    }

    int nbFeature =/* nbTiling * TWorld::ACTIONS_ACC * nbXinter +
                    nbTiling * TWorld::ACTIONS_ACC * nbYinter +
                    nbTiling * QLearnGen::ACTIONS_DIRECTION * nbXinter +
                    nbTiling * QLearnGen::ACTIONS_DIRECTION * nbYinter +
*/
                    nbTiling * TWorld::ACTIONS_ACC * nbXinter * nbYinter +
                    nbTiling * QLearnGen::ACTIONS_DIRECTION * nbXinter * nbYinter +
                    nbTiling * TWorld::ACTIONS_ACC * QLearnGen::ACTIONS_DIRECTION * nbXinter +
                    nbTiling * TWorld::ACTIONS_ACC * QLearnGen::ACTIONS_DIRECTION * nbYinter ;

    qlg = new QLearnGradient<State>(features, nbFeature, &ACTION_TEMPLATE, *TWorld::initialAction(&ACTION_TEMPLATE) );
}

QLearnGen::~QLearnGen()
{

}

void QLearnGen::decision()
{


    if(car->_speed_x < 15 && car->_distRaced < 15) {
        DAction ac;
        ac = *TWorld::initialAction(&ACTION_TEMPLATE);
        ac.set(ACC, 3);
        applyActionOn(ac, car);
    }
    else {
        const DAction* ac;
        State st = *TWorld::observe(*this);
        ac = qlg->decision(st, reward, lrate,  epsilon, lamda, discount);
        LOG_DEBUG(" action " << *ac << " recomp : " << reward);
        applyActionOn(*ac, car);
    }


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

