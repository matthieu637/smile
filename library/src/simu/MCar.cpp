#include <iostream>
#include <math.h>
#include "sml/Utils.hpp"
#include "simu/MCar.hpp"

using namespace std;
using namespace sml;

namespace simu {

MCar::MCar(int nbPosStep, int nbVelStep) : Environnement< MCarState >(new StateTemplate( {POS, VEL}, {nbPosStep, nbVelStep}), new ActionTemplate( {MOT}, {3}))
{
    init();
}

void MCar::initState(bool random) {
    if(!random) {
        state->position = -0.5;
        state->velocity = 0.0;
//         state->position = -1.2;
//         state->velocity = -0.07;
//         state->position = 0.59;
//         state->velocity = 0.069;
    } else {
        state->position = sml::Utils::randin(mcar_min_position, mcar_max_position);
        state->velocity = sml::Utils::randin(-mcar_max_velocity, mcar_max_velocity);
    }
}

void MCar::applyOn(const DAction& ac)
// Take action a, update state of car
{
    int a = ac[MOT];
    state->velocity += (a-1)*0.001 + cos(3*state->position)*(-0.0025);

    if (state->velocity > mcar_max_velocity)
        state->velocity = mcar_max_velocity;
    if (state->velocity < -mcar_max_velocity)
        state->velocity = -mcar_max_velocity;
    state->position += state->velocity;

    if (state->position > mcar_max_position)
        state->position = mcar_max_position;
    if (state->position < mcar_min_position)
        state->position = mcar_min_position;
    if (state->position==mcar_min_position && state->velocity<0)
        state->velocity = 0;
}

double MCar::reward() const {
    return -1.;
}

DAction* MCar::getInitialAction() const {
    return new DAction(getActions(), 1);
}

bool MCar::goal() const {
    return state->position >= mcar_goal_position;
}

unsigned int MCar::maxStep() const {
    return 500;
}

void MCar::computeDState(const MCarState& s, DState* dst, const ActionTemplate* repr) {
    dst->set(POS, round(Utils::transform(s.position, mcar_min_position, mcar_max_position, 0, repr->actionNumber(POS) -1)));
    dst->set(VEL, round(Utils::transform(s.velocity, -mcar_max_velocity, mcar_max_velocity, 0, repr->actionNumber(VEL) -1)));
}

}


