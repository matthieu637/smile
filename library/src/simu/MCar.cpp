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

double MCar::reward() const {
    return -1.;
}

const sml::ActionTemplate* MCar::getActions() const{
    return &MCar::ACTION_TEMPLATE;
}

DAction* MCar::getInitialAction() const{
    return new DAction(getActions(), 1);
}

bool MCar::goal() const{
    return st.position >= mcar_goal_position;
}

unsigned int MCar::maxStep() const{
    return 5000;
}

void MCar::computeDState() {
    dst->set(POS, round(Utils::transform(st.position, mcar_min_position, mcar_max_position, 0, stempl->actionNumber(POS) -1)));
    dst->set(VEL, round(Utils::transform(st.velocity, -mcar_max_velocity, mcar_max_velocity, 0, stempl->actionNumber(VEL) -1)));
}

}
