
#ifndef QLEARNGENCMPLX_HPP
#define QLEARNGENCMPLX_HPP

#include "Driver.hpp"
#include <sml/Action.hpp>
#include <sml/QLearnGradient.hpp>
#include <sml/Q.hpp>
#include "TWorld.hpp"
#include "DriverFeedback.hpp"

using sml::DAction;
using sml::DState;
using sml::QTable;
using sml::QLearnGradient;

class QLearnGenCmplx : public DriverFeedback
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

    static const int ACTIONS_DIRECTION = 14;

    const double lambda = 0.95;
    const double lrate = 0.0001;
    const double discount = 0.75;
    const double epsilon = 0.001;

    static const double road_width;
    static const double total_angle;
    static const double total_speed;
    static const double max_straight;

    static const int nbXinter = 18;
    static const int nbYinter = 24;
    static const int nbZinter = 10;
    static const int nbUinter = 10;

    static const bool learn = true;
    sml::LearnConfig conf = {true, 0.1, 2000};
    static const unsigned int simu_time = 15; //in minutes

    static const sml::ActionTemplate ACTION_TEMPLATE;

    QLearnGradient<State>* qlg;
    DAction* lastAction;
    bool activeTutor;
    int delay;
};

#endif // QLEARNDISCR_HPP

