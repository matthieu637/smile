#ifndef MCAR_HPP
#define MCAR_HPP

#include "sml/Action.hpp"

///
///\file MCar.hpp
///\brief Simulation du MontainCar
///
///


using namespace std;
using namespace sml;

namespace simu {

class MCar {

#define POS "position"
#define VEL "velocity"
#define MOT "motor"

#define mcar_min_position -1.2
#define mcar_max_position 0.6
#define mcar_max_velocity 0.07            // the negative of this is also the minimum velocity
#define mcar_goal_position 0.5

public:
    struct State {
        float position;
        float velocity;
    };

    MCar(int nbPosStep, int nbVelStep);
    MCar();

    State& step(const DAction& ac);                 // update car state for given action

    State& getState() ;
    DState* computeDState() const;
    bool goal_p () const;                   // is car at goal?
    void init();                      // initialize car state

private:

    State st;
public:
    const static ActionTemplate ACTION_TEMPLATE;
    const StateTemplate* stempl;
};

}


#endif
