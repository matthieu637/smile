
#ifndef QLEARNGENCMPLX_HPP
#define QLEARNGENCMPLX_HPP

#include "Driver.hpp"
#include <sml/Action.hpp>
#include <sml/QLearnGradient.hpp>
#include <sml/Q.hpp>
#include "TWorld.hpp"

using sml::DAction;
using sml::DState;
using sml::QTable;
using sml::QLearnGradient;

class QLearnGenCmplx : public Driver
{

public:
    QLearnGenCmplx(int index);
    void decision();
    ~QLearnGenCmplx();
    void newRace(tCarElt* car, tSituation *s);
    void endRace();
    sml::LearnStat* getAlgo();

private:
    void applyActionOn(const DAction& ac, tCarElt* car);

private:
    static const int DECISION_EACH = 3;

    static const int ACTIONS_DIRECTION = 8;

    const double lamda = 0.95;
    const double lrate = 0.0001;
    const double discount = 0.75;
    const double epsilon = 0.001;

    static const double road_width;
    static const double total_angle;
    static const double total_speed;

    static const int nbXinter = 14;
    static const int nbYinter = 16;
    static const int nbZinter = 8;

    static const bool learn = true;
    sml::LearnConfig conf = {true, 0.2, 3000};
    static const unsigned int simu_time = 10; //in minutes

    static const sml::ActionTemplate ACTION_TEMPLATE;

    QLearnGradient<State>* qlg;
};

#endif // QLEARNDISCR_HPP

