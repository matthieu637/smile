#ifndef TEACHER_HPP
#define TEACHER_HPP

#include "simu/Environnement.hpp"
#include "simu/MCar.hpp"
#include <sml/Q.hpp>
#include <sml/QLearnGradient.hpp>
#include "simu/RLSimulation.hpp"
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
	float based = -1.;
        (void) reward;
        if(!giveAdvise)
            return based;
        else
            return based*cost;
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


class DiscretizeTSelection {
protected:
    template<typename EnvState>
    const DState& getTState(Environnement<EnvState>* env)
    {
        return env->getDState();
    }
};

class ContinuousTSelection {
protected:
    template<typename EnvState>
    const EnvState& getTState(Environnement<EnvState>* env)
    {
        return env->getState();
    }
};

struct Tstats {
    double lreward;
    int lstep;
    int lepisod;
    double treward;
};

template<typename TeacherPolicyState>
class ATeacher
{
  virtual void setTeacherPolicy(Policy<TeacherPolicyState>* tpol) = 0;
};


template<typename PolicyReward, typename EnvState, typename PolicyState, typename StateType, typename TeacherPolicyState, typename TeacherStateType>
class Teacher : public Environnement < TeacherState < EnvState > >, protected PolicyReward, private StateType, private TeacherStateType, public ATeacher<TeacherPolicyState> {

    using PolicyReward::policy_reward;
    using StateType::getState;
    using TeacherStateType::getTState;

public:
    Teacher(RLSimulation<EnvState, PolicyState, StateType>* learner, const StateTemplate& st, float advice_cost, AdviseStrategy astart, StrategyEffectsAdvice sea):
        Environnement< TeacherState < EnvState > >(new StateTemplate( st, *learner->getEnv()->getActions()) , new ActionTemplate( {FDB}, {2}) ) ,
                   prob(learner->getEnv()),
                   learner(learner),
                   advice_cost(advice_cost),
                   astart(astart),
                   sea(sea)
    {
        // Compute the best policy of the learner for the teacher
	learner->init();
        learner->run();
        std::list<stats>* hist = learner->keepRun(5000);
        best_policy = learner->get_best_policy()->copyPolicy();
// 	best_policy = learner->get_policy()->copyPolicy();
        this->init();

        best_policy_teacher = hist->back().min_step;
        LOG_DEBUG(best_policy_teacher << " after " << hist->back().index_min << " runs");
// 	LOG_DEBUG(hist->back().nbStep);
        delete hist;
    }

    void setTeacherPolicy(Policy<TeacherPolicyState>* tpol) {
        this->tpolicy = tpol;
    }

    double reward() const {
        return policy_reward(giveAdvise, advice_cost, prob->reward());
    }

    DAction* getInitialAction() const {
        return new DAction(this->getActions(), 1);
    }

    bool goal() const {
        return learner_reached_goal > 101;
    }

    unsigned int maxStep() const {
        return 500*100;
    }

    int get_best_policy_teacher() const {
        return best_policy_teacher;
    }

    int get_given_advice() const {
        return nbAdvice;
    }

    const list<Tstats>& get_learner_stats() const {
        return run_stats;
    }

protected:
    virtual void applyOn(const DAction& ac) {
        int a = ac[FDB];
        giveAdvise = a;

        lstep ++;
        lreward += prob->reward();
	treward += reward();

        DAction* learner_next_action = nullptr;
        PolicyState state_learner = getState(prob);

        switch(astart) {
        case after:
            prob->apply(*this->state->learner_action);
            if(giveAdvise && sea != None) {
                DAction* best_action = best_policy->decision(state_learner, false);
                learner->get_policy()->should_done(state_learner, *best_action);
                delete best_action;
                nbAdvice++;
            }

            learner_next_action = learner->computeNextAction(getState(prob), prob->reward());

// 	    LOG_DEBUG("state : " << state_learner << " action : " << *this->state->learner_action << " advice : " << *best_policy->decision(state_learner, 0) << " have advice " << giveAdvise );
            break;
        case before:
            if(giveAdvise && sea != None && advice_limit_per_ep > 0) {
                DAction* best_action = best_policy->decision(state_learner, false);
                prob->apply(*best_action);

                learner->get_policy()->should_do(state_learner, *best_action, prob->reward());
                delete best_action;
                nbAdvice++;
		advice_limit_per_ep--;
            }
            else {
                DAction* la = learner->computeNextAction(state_learner, prob->reward());
                prob->apply(*la);
            }

            Policy<PolicyState>* cp =  learner->get_policy()->copyPolicy();
            learner_next_action = cp->learn(getState(prob), prob->reward());
            delete cp;
            break;
        }

        //TODO: should clear_history of my own algo
        if(prob->goal() || prob->maxStep() < lstep) {
            run_stats.push_back( {lreward, lstep, learner_reached_goal, treward});

            prob->init();
            learner->get_policy()->clear_history(getState(prob), *prob->getInitialAction());

            this->state->learner_state = prob->getState();
            this->state->learner_action = new DAction(*prob->getInitialAction());
            tpolicy->clear_history(getTState(this), *getInitialAction());

            learner_reached_goal++;
            learner_next_action = prob->getInitialAction();

            lstep = 0;
            lreward = 0;
	    treward = 0;
	    advice_limit_per_ep = 100;
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
        nbAdvice = 0;
        learner_reached_goal = 0;
        this->state->learner_state = prob->getState();
        this->state->learner_action = new DAction(*prob->getInitialAction());
        lstep = 0;
        lreward = 0;
	treward = 0;
	advice_limit_per_ep = 100;
        run_stats.clear();
    }

private:
    Environnement<EnvState>* prob;
    RLSimulation<EnvState, PolicyState, StateType>* learner;
    Policy<PolicyState>* best_policy;

    Policy<TeacherPolicyState>* tpolicy;
    bool giveAdvise;
    float advice_cost;
    int best_policy_teacher;
    int nbAdvice;
    int lstep;
    int advice_limit_per_ep;
    double lreward;
    double treward;
    int learner_reached_goal;
    list<Tstats> run_stats;
    AdviseStrategy astart;
    StrategyEffectsAdvice sea;
};


template<typename PolicyReward, typename EnvState>
using DDTeacher = Teacher<PolicyReward, EnvState, DState, DiscretizeSelection, DState, DiscretizeTSelection>;

template<typename PolicyReward, typename EnvState>
using CDTeacher = Teacher<PolicyReward, EnvState, EnvState, ContinuousSelection, DState, DiscretizeTSelection>;

template<typename PolicyReward, typename EnvState, typename TeacherPolicyState>
using DCTeacher = Teacher<PolicyReward, EnvState, DState, DiscretizeSelection, TeacherPolicyState, ContinuousTSelection>;

template<typename PolicyReward, typename EnvState, typename TeacherPolicyState>
using CCTeacher = Teacher<PolicyReward, EnvState, EnvState, ContinuousSelection, TeacherPolicyState, ContinuousTSelection>;


}

#endif

