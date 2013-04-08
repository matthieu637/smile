
#ifndef QLEARNGEN_HPP
#define QLEARNGEN_HPP

#include "Driver.hpp"
#include <sml/Action.hpp>
#include <sml/QLearnGradient.hpp>
#include <sml/Q.hpp>
#include "TWorld.hpp"

using sml::DAction;
using sml::DState;
using sml::QTable;
using sml::QLearnGradient;

class QLearnGen : public Driver
{

public:
    QLearnGen(int index);
    void decision();
    ~QLearnGen();
    void newRace(tCarElt* car, tSituation *s);
    void endRace();

    static double featuring1 (const State& st, const DAction& ac, std::vector<double> params);
    static double featuring2 (const State& st, const DAction& ac, std::vector<double> params);
    static double featuring3 (const State& st, const DAction& ac, std::vector<double> params);
    static double featuring4 (const State& st, const DAction& ac, std::vector<double> params);    
    
    static double featuring3D1 (const State& st, const DAction& ac, std::vector<double> params);
    static double featuring3D2 (const State& st, const DAction& ac, std::vector<double> params);
    static double featuring3D3 (const State& st, const DAction& ac, std::vector<double> params);
    static double featuring3D4 (const State& st, const DAction& ac, std::vector<double> params);

private:
    void applyActionOn(const DAction& ac, tCarElt* car);

private:
    static const int DECISION_EACH = 5;

    static const int ACTIONS_DIRECTION = 8;

    const double lamda = 0.95;
    const double lrate = 0.001;
    const double discount = 0.75;
    const double epsilon = 0.05;

    static const double xmax;
    static const double ymax;

// const int nbtiling = 9;

    static const int nbXinter = 14;
    static const int nbYinter = 16;

    static const double xwidth;//increase by log
    static const double yheight;

    static const double xtiling[];
    static const double ytiling[];
    static const double atiling[];
    static const double dtiling[];

    static const sml::ActionTemplate ACTION_TEMPLATE;

    QLearnGradient<State>* qlg;
};

#endif // QLEARNDISCR_HPP

