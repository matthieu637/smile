#ifndef TEACHER_HPP
#define TEACHER_HPP

#include "simu/Environnement.hpp"
#include "simu/MCar.hpp"
#include <sml/Q.hpp>
#include "simu/RLTable.hpp"
#include "GridWorldLS.hpp"

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
        if(!giveAdvise)
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
        if(!giveAdvise)
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
        learner->run();
        std::list<stats>* hist = learner->keepRun(10000);
        best_policy = learner->get_best_policy()->copyPolicy();
// 	best_policy = learner->get_policy()->copyPolicy();
        this->init();

        best_policy_teacher = hist->back().min_step;
	LOG_DEBUG(best_policy_teacher << " after " << hist->back().index_min << " runs");
// 	LOG_DEBUG(hist->back().nbStep);
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

        DAction* learner_next_action = nullptr;
        DState dstate_learner = prob->getDState();
	
        switch(astart) {
        case after:
            prob->apply(*this->state->learner_action);
            if(giveAdvise && sea != None) {
                DAction* best_action = best_policy->decision(dstate_learner, 0);
                learner->get_policy()->should_done(dstate_learner, *best_action);
                delete best_action;
            }
            
            learner_next_action = learner->computeNextAction(prob->getDState(), prob->reward());
	    
// 	    LOG_DEBUG("state : " << dstate_learner << " action : " << *this->state->learner_action << " advice : " << *best_policy->decision(dstate_learner, 0) << " have advice " << giveAdvise );
            break;
        case before:
            if(giveAdvise && sea != None) {
                DAction* best_action = best_policy->decision(dstate_learner, 0);
                learner->get_policy()->should_do(dstate_learner, *best_action);
                prob->apply(*best_action);
                delete best_action;
            }
            else
                prob->apply(*this->state->learner_action);


            learner_next_action = learner->computeNextAction(prob->getDState(), prob->reward());
            break;
        }


// 	LOG_DEBUG(prob->getDState() << " " << *learner_next_action << " " << giveAdvise << " " << *(this->state->learner_action));

        EnvState car = prob->getState();
        this->state->learner_state = car;
 	delete this->state->learner_action;
        this->state->learner_action = new DAction(*learner_next_action);
    }

    void computeDState(const TeacherState<EnvState>& s, DState* dst, const StateTemplate* repr) {
// 	const GridWorldLSState ss = ((const GridWorldLSState&)prob->getState());
// 	LOG_DEBUG("begin with : " << *dst << " copy first {" << ss.x << "," << ss.y << "," << ss.p << "} and then " << *s.learner_action);	
        prob->computeDState(prob->getState(), dst, repr);
// 	LOG_DEBUG(*dst);
        dst->copyValuesOf(*s.learner_action);
// 	LOG_DEBUG(*dst);
    }

    void initState() {
        learner->reset();
        learner->get_policy()->setAdviseStrat(sea);
        this->state->learner_state = prob->getState();
        this->state->learner_action = new DAction(*prob->getInitialAction());
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

