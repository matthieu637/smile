#ifndef TEACHER_HPP
#define TEACHER_HPP

#include "simu/Environnement.hpp"
#include "simu/MCar.hpp"
#include <sml/Q.hpp>
#include "simu/RLTable.hpp"

#define FDB "feedbacks"

namespace simu {

template<typename EnvState>
struct TeacherState {
    EnvState learner_state;
    DAction* learner_action;
};

enum AdviseStrategy {
    before, after
};

class FavorAdvice {
protected:
    double policy_reward(bool giveAdvise, float cost, double reward) const {
        (void) cost;
        (void) reward;
        if(giveAdvise)
            return 10;
        else
            return -10;
    }
};

class CostlyAdvise {
protected:
    double policy_reward(bool giveAdvise, float cost, double reward) const {
        (void) reward;
        if(giveAdvise)
            return -1;
        else
            return -1*cost;
    }
};

class LearnerAdvise {
protected:
    double policy_reward(bool giveAdvise, float cost, double reward) const {
        (void) cost;
        (void) giveAdvise;
        return reward;
    }
};

class CostlyLearnerAdvise {//TODO: should only works with negative reward
protected:
    double policy_reward(bool giveAdvise, float cost, double reward) const {
        if(giveAdvise)
            return reward;
        else
            return reward*cost;
    }
};

template<typename PolicyReward, typename EnvState>
class DTeacher : public Environnement < TeacherState < EnvState > >, protected PolicyReward {

    using PolicyReward::policy_reward;

public:
    DTeacher(RLTable<EnvState>* learner, const StateTemplate& st, float advice_cost, AdviseStrategy astart, StrategyEffectsAdvice sea):
        Environnement< TeacherState < EnvState > >(new StateTemplate( st, *learner->getEnv()->getActions()) , new ActionTemplate( {FDB}, {2}) ) ,
                   prob(learner->getEnv()),
                   learner(learner),
                   advice_cost(advice_cost),
                   astart(astart),
                   sea(sea)
    {
        // Compute the best policy of the learner for the teacher
// 	RLTable<EnvState> m(simu::QL_trace, new MCar(8, 12)); TODO: compute best policy in teacher representation ??
        learner->run();
        std::list<stats>* hist = learner->keepRun(10000);
        best_policy = learner->get_best_policy()->copyPolicy();
        this->init();

        best_policy_teacher = hist->back().min_step;
        delete hist;
    }

    double reward() const {
        return policy_reward(giveAdvise, advice_cost, prob->reward());
    }

    DAction* getInitialAction() const {
        return new DAction(this->getActions(), 1);
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
            prob->apply(*this->state->learner_action);

            if(giveAdvise) {
                DAction* best_action = best_policy->decision(dstate_leaner);
                learner->get_policy()->should_done(dstate_leaner, *best_action);
                delete best_action;
            }

            learner_next_action = learner->computeNextAction(prob->getDState(), prob->reward());
            break;
        case before:
            if(giveAdvise) {
                DAction* best_action = best_policy->decision(dstate_leaner);
                learner->get_policy()->should_do(dstate_leaner, *best_action);
                prob->apply(*best_action);
                delete best_action;
            }
            else
                prob->apply(*this->state->learner_action);


            learner_next_action = learner->computeNextAction(prob->getDState(), prob->reward());
            break;
        }


// 	LOG_DEBUG(prob->getDState() << " " << *learner_action << " " << giveAdvise);

        EnvState car = prob->getState();
        this->state->learner_state = car;
        this->state->learner_action = learner_next_action;
    }

    void computeDState(const TeacherState<EnvState>& s, DState* dst, const StateTemplate* repr) {
        prob->computeDState(prob->getState(), dst, repr);
        dst->copyValuesOf(*s.learner_action);
    }

    void initState() {
        learner->reset();
        learner->get_policy()->setAdviseStrat(sea);
        this->state->learner_state = prob->getState();
        this->state->learner_action = prob->getInitialAction();
    }

private:
    Environnement<EnvState>* prob;
    RLTable<EnvState>* learner;
    Policy<DState>* best_policy;
    bool giveAdvise;
    float advice_cost;
    int best_policy_teacher;
    AdviseStrategy astart;
    StrategyEffectsAdvice sea;
};


}

#endif

