
#ifndef SARSAFDB_HPP
#define SARSAFDB_HPP

#include "Driver.hpp"
#include <sml/Action.hpp>
#include <sml/SarsaLamb.hpp>
#include <sml/Q.hpp>
#include "TWorld.hpp"
#include "DriverFeedback.hpp"


using sml::DAction;
using sml::DState;
using sml::QTable;
using sml::SarsaLamb;

class SarsaFdb : public DriverFeedback
{

public:
    SarsaFdb(int index);
    void decision();
    ~SarsaFdb();
    void newRace(tCarElt* car, tSituation *s);
    void endRace();
    sml::LearnStat* getAlgo();

private:
    DState* discretize(const State& st);
    void applyActionOn(const DAction& ac, tCarElt* car);

private:
    static const int DECISION_EACH = 3;

    static const int ACTIONS_DIRECTION = 14;

    const double lambda = 0.95;
    const double lrate = 0.0001;
    const double discount = 0.75;
    const double epsilon = 0.001;

    static const int nbXinter = 14;
    static const int nbYinter = 16;
    static const int nbZinter = 10;
    
    static const bool learn = true;
    sml::LearnConfig conf = {true, 0.1, 2000};
    static const unsigned int simu_time = 15; //in minutes

    static const sml::ActionTemplate ACTION_TEMPLATE;
    static const sml::StateTemplate STATE_TEMPLATE;

    SarsaLamb* qlg;
    DAction* lastAction;
    bool activeTutor;
    int delay;
};

#endif // SARSAFDB_HPP

