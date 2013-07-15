#include <test/Teacher.hpp>

namespace test {

Teacher::Teacher(MCar* prob):
    Environnement< TeacherState >(new StateTemplate( {FDB, POS, VEL, MOT}, {2,prob->getStates()->actionNumber(POS), prob->getStates()->actionNumber(VEL),
        prob->getActions()->actionNumber(MOT)
                                                                             }) ,
new ActionTemplate( {FDB}, {2}) ) , prob(prob), feedbacks(prob->getStates())
{

}

void Teacher::initState() {
    state->feedback = false;
    state->car = prob->getState();
    state->action = prob->getInitialAction()->get(MOT);
}

void Teacher::applyOn(const DAction& ac)
// Take action a, update state of car
{
    int a = ac[FDB];
    feedbacks(prob->getDState()) = a;
    
    MCarState car = prob->getState();
    DState dcar = prob->getDState();
    state->feedback = feedbacks(dcar);
    state->car.position = car.position;
    state->car.velocity = car.velocity;
//     state->action = prob->
}

double Teacher::reward() const {
    return prob->reward();
}

DAction* Teacher::getInitialAction() const {
    return new DAction(getActions(), 1);
}

bool Teacher::goal() const {
    return false;
}

unsigned int Teacher::maxStep() const {
    return 5000;
}

void Teacher::computeDState() {
    dstate->set(FDB, (int) state->feedback);
    dstate->set(POS, prob->getDState()[POS]);
    dstate->set(VEL, prob->getDState()[VEL]);
    dstate->set(MOT, (int) state->action);
}

}
