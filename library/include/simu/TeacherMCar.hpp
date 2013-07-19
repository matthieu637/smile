#ifndef TEACHER_HPP
#define TEACHER_HPP

#include "simu/Environnement.hpp"
#include "simu/MCar.hpp"
#include <sml/Q.hpp>
#include "simu/RLTable.hpp"

#define FDB "feedbacks"

namespace simu {

struct TeacherState {
//     bool feedback;
    MCarState car;
    int action;
};


class TeacherMCar : public Environnement<TeacherState> {

public:
    TeacherMCar(RLTable<MCarState>* learner):
        Environnement< TeacherState >(new StateTemplate( {POS, VEL, MOT}, {learner->getEnv()->getStates()->actionNumber(POS), learner->getEnv()->getStates()->actionNumber(VEL),
            learner->getEnv()->getActions()->actionNumber(MOT)
                                                                      }) ,
    new ActionTemplate( {FDB}, {2}) ) , prob((MCar*)learner->getEnv()), learner(learner)
    {
	// Compute the best policy of the learner for the teacher
        bib::Logger::getInstance()->enableBuffer();
        bib::Logger::getInstance()->setIgnoredBuffer(true);
        learner->run();
        int score = learner->keepRun(5000).min_step;
	best_policy = learner->get_best_policy()->copyPolicy();
	learner->reset();
        bib::Logger::getInstance()->setIgnoredBuffer(false);
        LOG(score);
	init();
    }

    double reward() const {
//         if(giveAdvise)
//             return -10;//prob->reward();
//         else 
	  return 1;
    }
    
    DAction* getInitialAction() const {
        return new DAction(getActions(), 1);
    }
    
    bool goal() const {
        return prob->goal();
    }
    
    unsigned int maxStep() const {
        return 5000;
    }
    
protected:
    void applyOn(const DAction& ac) {
        int a = ac[FDB];
        giveAdvise = a;

	DAction* learner_action = learner->step(prob->getDState(), prob->reward());
	
        if(giveAdvise == 1) {
	    DAction* best_action = best_policy->decision(prob->getDState());
   	    learner->get_policy()->should_done(prob->getDState(), *best_action);
	    delete best_action;
        }
        
        prob->apply(*learner_action);
	
// 	LOG_DEBUG(prob->getDState() << " " << *learner_action << " " << giveAdvise);

        MCarState car = prob->getState();
        DState dcar = prob->getDState();

        state->car.position = car.position;
        state->car.velocity = car.velocity;
        state->action = learner_action->get(MOT);
    }

    void computeDState() {
        dstate->set(POS, prob->getDState()[POS]);
        dstate->set(VEL, prob->getDState()[VEL]);
        dstate->set(MOT, (int) state->action);
    }

    void initState() {
	prob->init();
        state->car = prob->getState();
        state->action = prob->getInitialAction()->get(MOT);
    }

private:
    MCar* prob;
    RLTable<MCarState>* learner;
    Policy<DState>* best_policy;
    bool giveAdvise;
};


}

#endif

