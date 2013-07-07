#include <iostream>
#include <math.h>
#include "sml/Utils.hpp"
#include "simu/MCar.hpp"

using namespace std;
using namespace sml;

namespace simu {

const ActionTemplate MCar::ACTION_TEMPLATE = ActionTemplate( {MOT}, {3});

MCar::MCar(int nbPosStep, int nbVelStep)
{
    stempl = new StateTemplate( {POS, VEL}, {nbPosStep, nbVelStep});
    dst = new DState(stempl, 0);
//     LOG_DEBUG(stempl->actionNumber(VEL));
    
    init();
}

MCar::MCar()
{
    init();
}

MCar::~MCar(){
    delete stempl;
    delete dst;
}

void MCar::init()
{
    st.position = -0.5;
    st.velocity = 0.0;
    computeDState();
}

void MCar::step(const DAction& ac)
// Take action a, update state of car
{
    int a = ac[MOT];
    st.velocity += (a-1)*0.001 + cos(3*st.position)*(-0.0025);

    if (st.velocity > mcar_max_velocity)
        st.velocity = mcar_max_velocity;
    if (st.velocity < -mcar_max_velocity)
        st.velocity = -mcar_max_velocity;
    st.position += st.velocity;

    if (st.position > mcar_max_position)
        st.position = mcar_max_position;
    if (st.position < mcar_min_position)
        st.position = mcar_min_position;
    if (st.position==mcar_min_position && st.velocity<0)
        st.velocity = 0;

    computeDState();
}

const DState& MCar::getDState() const {
    return *dst;
}

void MCar::computeDState() {
    dst->set(POS, round(Utils::transform(st.position, mcar_min_position, mcar_max_position, 0, stempl->actionNumber(POS) -1)));
    dst->set(VEL, round(Utils::transform(st.velocity, -mcar_max_velocity, mcar_max_velocity, 0, stempl->actionNumber(VEL) -1)));
}

const MCar::State& MCar::getState() const{
  return st;
}

bool MCar::goal_p () const
// Is Car within goal region?
{
    return st.position >= mcar_goal_position;
}

}
