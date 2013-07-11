#include "simu/MCarTask.hpp"
#include "bib/Logger.hpp"

namespace simu {

MCarTask::MCarTask() {
    prob = new MCar(8,12);
    prob->getActions();
    fac = prob->getInitialAction();;
}

MCarTask::~MCarTask() {
    delete fac;
    delete prob;
}

void MCarTask::run() {
    this->createAgent(prob->getDState(), prob->getState(), *fac);

    DAction* ac;
    int step = 0;
    do
    {
        step++;
        prob->step(*ac);

   	ac = step(prob->getDState(), prob->getState(), prob->reward());
//         ac = ql.learn(prob.getDState(), -1, alpha, epsilon, gamma);

//      LOG_DEBUG("etat " << dst << " action " << *ac << " ");
    }
    while(!prob->goal_p());

    LOG(step);
}

}
