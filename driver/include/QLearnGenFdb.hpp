
#ifndef QLEARNGENFDB_HPP
#define QLEARNGENFDB_HPP

#include <sml/Action.hpp>
#include <sml/QLearnGradient.hpp>
#include <sml/Q.hpp>
#include "TWorld.hpp"
#include "DriverFeedback.hpp"

using sml::DAction;
using sml::DState;
using sml::QTable;
using sml::QLearnGradient;

class QLearnGenFdb : public DriverFeedback
{

public:
    QLearnGenFdb(int index);
    void decision();
    ~QLearnGenFdb();
    void newRace(tCarElt* car, tSituation *s);
    void endRace();
    sml::LearnStat* getAlgo();

private:
    void applyActionOn(const DAction& ac, tCarElt* car);

private:
    static const int DECISION_EACH = 3;

    static const int ACTIONS_DIRECTION = 12;

    const double lamda = 0.95;
    const double lrate = 0.0001;
    const double discount = 0.75;
    const double epsilon = 0.01;

    static const double road_width;
    static const double total_angle;

    static const int nbXinter = 22;
    static const int nbYinter = 24;

    static const bool learn = false;
    sml::LearnConfig conf = {true, 0.1, 2000};
    static const unsigned int simu_time = 10; //in minutes

    static const sml::ActionTemplate ACTION_TEMPLATE;

    QLearnGradient<State>* qlg;
};

#endif

