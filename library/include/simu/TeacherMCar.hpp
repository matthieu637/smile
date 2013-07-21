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
    MCarState learner_state;
    DAction* learner_action;
};

enum AdviseStrategy {
    before, after
};

class FavorAdvice {
protected:
    double policy_reward(bool giveAdvise, float cost) const {
        (void) cost;
        if(giveAdvise)
            return 10;
        else
            return -10;
    }
};

template<typename PolicyReward, typename EnvState>
class TeacherMCar : public Environnement<TeacherState>, protected PolicyReward {

    using PolicyReward::policy_reward;

public:
    TeacherMCar(RLTable<MCarState>* learner, const StateTemplate& st, float advice_cost, AdviseStrategy astart):
        Environnement< TeacherState >(new StateTemplate( st, *learner->getEnv()->getActions()) , new ActionTemplate( {FDB}, {2}) ) ,
                   prob(learner->getEnv()),
                   learner(learner),
                   advice_cost(advice_cost),
                   astart(astart)
    {
        // Compute the best policy of the learner for the teacher
// 	RLTable<MCarState> m(simu::QL_trace, new MCar(8, 12)); TODO: compute best policy in teacher representation
        learner->run();
        std::list<stats>* hist = learner->keepRun(10000);
        best_policy = learner->get_best_policy()->copyPolicy();
        init();

        best_policy_teacher = hist->back().min_step;
        delete hist;
    }

    double reward() const {
        return policy_reward(giveAdvise, advice_cost);
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

    int get_best_policy_teacher() const {
        return best_policy_teacher;
    }

protected:
    virtual void applyOn(const DAction& ac) {
        int a = ac[FDB];
        giveAdvise = a;


        DAction* learner_next_action;
        DState dstate_leaner = prob->getDState();
        switch(astart) {
        case after:
            prob->apply(*state->learner_action);

            if(giveAdvise) {
                DAction* best_action = best_policy->decision(dstate_leaner);
                learner->get_policy()->should_done(dstate_leaner, *best_action);
                delete best_action;
            }

            learner_next_action = learner->computeNextAction(prob->getDState(), prob->reward());
            break;
// 	  case before:
        default:
            if(giveAdvise) {
                DAction* best_action = best_policy->decision(dstate_leaner);
                learner->get_policy()->should_do(dstate_leaner, *best_action);
                prob->apply(*best_action);
                delete best_action;
            }
            else
                prob->apply(*state->learner_action);

            learner_next_action = learner->get_policy()->decision(prob->getDState());
            break;
        }


// 	LOG_DEBUG(prob->getDState() << " " << *learner_action << " " << giveAdvise);

        MCarState car = prob->getState();
        state->learner_state = car;
        state->learner_action = learner_next_action;
    }

    void computeDState(const TeacherState& s, DState* dst, const StateTemplate* repr) {
	prob->computeDState(prob->getState(), dst, repr);
        dst->copyValuesOf(*s.learner_action);
    }

    void initState() {
        learner->reset();
        state->learner_state = prob->getState();
        state->learner_action = prob->getInitialAction();
    }

private:
    Environnement<MCarState>* prob;
    RLTable<MCarState>* learner;
    Policy<DState>* best_policy;
    bool giveAdvise;
    float advice_cost;
    int best_policy_teacher;
    AdviseStrategy astart;
};


}

#endif
