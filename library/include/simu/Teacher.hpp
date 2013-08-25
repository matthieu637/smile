#ifndef TEACHER_HPP
#define TEACHER_HPP

#include "simu/Environnement.hpp"
#include "simu/MCar.hpp"
#include <sml/Q.hpp>
#include <sml/QLearnGradient.hpp>
#include "simu/RLSimulation.hpp"
#include "GridWorldLS.hpp"

#define FDB "feedbacks"

#define STEP_REACH_RND_BEST_POL 2000
#define STEP_REACH_BEST_POL 2000
#define LEARNER_STEP 100
#define MAX_NUMBER_ADVICE 100

namespace simu {

template<typename EnvState>
struct TeacherState {
    EnvState learner_state;
    DAction* learner_action;
    int episod;
    int givenFdb;
};

enum AdviseStrategy {
    before, after
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
    int nbAdvice;
    float ratio_ad_ch;
};

template<typename TeacherPolicyState>
class ATeacher
{
    virtual void setTeacherPolicy(Policy<TeacherPolicyState>* tpol) = 0;
};


template<typename EnvState, typename PolicyState, typename StateType, typename TeacherPolicyState, typename TeacherStateType>
class Teacher : public Environnement < TeacherState < EnvState > >, private StateType, private TeacherStateType, public ATeacher<TeacherPolicyState> {

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
        learner->run();

        std::list<stats>* hist = learner->keepRun(STEP_REACH_RND_BEST_POL, true);
        delete hist;
        hist = learner->keepRun(STEP_REACH_BEST_POL, false);
        best_policy = learner->get_best_policy()->copyPolicy();
//         best_policy = learner->get_policy()->copyPolicy();
        best_policy_teacher = hist->back().min_step;

        LOG_DEBUG(best_policy_teacher << " after " << hist->back().index_min << " runs");

        delete hist;

        alreadyInit=false;
        this->init();
    }

    ~Teacher() {
        delete best_policy;
        delete this->state->learner_action;
    }

    void setTeacherPolicy(Policy<TeacherPolicyState>* tpol) {
        this->tpolicy = tpol;
    }

    double reward() const {
        if(goal())
            return 1;

        float based = -1.;
        if(!giveAdvise) { //don't give advice
            if(tookBestAction) //agent take the best action , no need to advice
                return 0.;
            return based;
        }
        else {
            return based*advice_cost;
        }
    }

    DAction* getInitialAction() const {
        return new DAction(this->getActions(), 1);
    }

    bool goal() const {
        return learner_reached_goal > (LEARNER_STEP-1);
    }

    unsigned int maxStep() const {
        return 500*LEARNER_STEP;
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
        int fdb = ac[FDB];
//         giveAdvise = sml::Utils::rand01() <= 80./100. ;
        giveAdvise = fdb;

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
                learner->get_policy()->should_do(state_learner, *best_action, prob->reward());

                tookBestAction = (*best_action == *this->state->learner_action);

                prob->apply(*best_action);
                delete best_action;
                nbAdvice++;
                advice_limit_per_ep--;
            }
            else
            {
                DAction* best_action = best_policy->decision(state_learner, false);
// 		take greeding in consideration
                DAction* la = this->state->learner_action;
                learner->get_policy()->had_choosed(state_learner, *la, prob->reward(), gotGreedy);

                tookBestAction = (*best_action == *la);
                delete best_action;

                prob->apply(*la);
            }

            Policy<PolicyState>* cp =  learner->get_policy()->copyPolicy();
            LearnReturn lr = cp->_learn(getState(prob), prob->reward());
            learner_next_action = new DAction(*lr.ac);
            gotGreedy = lr.gotGreedy;
            delete cp;
            break;
        }

        if(prob->goal() || prob->maxStep() < lstep) {
            run_stats.push_back( {lreward, lstep, learner_reached_goal, treward, nbAdvice - sumLastNbAvice, (float)(nbAdvice - sumLastNbAvice) / (float) lstep });
            sumLastNbAvice = nbAdvice;

            prob->init();
            DAction* ac = prob->getInitialAction();
            learner->get_policy()->clear_history(getState(prob), *ac);

	    //TODO: should clear_history of my own algo | try without
//             DAction* ia = getInitialAction();
//             tpolicy->clear_history(getTState(this), *ia);
//             delete ia;

            learner_reached_goal++;
            if(astart == before)
                delete learner_next_action;
            learner_next_action = new DAction(*ac);
            delete ac;

            lstep = 0;
            lreward = 0;
            treward = 0;
//             advice_limit_per_ep = MAX_NUMBER_ADVICE;
            gotGreedy = false;
        }

// 	LOG_DEBUG(prob->getDState() << " " << *learner_next_action << " " << giveAdvise << " " << *(this->state->learner_action));

        EnvState car = prob->getState();
        this->state->learner_state = car;
        this->state->episod = learner_reached_goal;
        this->state->givenFdb = nbAdvice - sumLastNbAvice;

        delete this->state->learner_action;
        if(astart == before)
            this->state->learner_action = learner_next_action;
        else this->state->learner_action = new DAction(*learner_next_action);
    }

    void computeDState(const TeacherState<EnvState>& s, DState* dst, const StateTemplate* repr) {
// 	const GridWorldLSState ss = ((const GridWorldLSState&)prob->getState());
// 	LOG_DEBUG("begin with : " << *dst << " copy first {" << ss.x << "," << ss.y << "," << ss.p << "} and then " << *s.learner_action);
        prob->computeDState(prob->getState(), dst, repr);
// 	LOG_DEBUG(*dst);
        dst->copyValuesOf(*s.learner_action);
// 	LOG_DEBUG(*dst);
    }

    void initState(bool random=false) {
        (void)random;

        learner->reset();
        learner->get_policy()->setAdviseStrat(sea);
        nbAdvice = 0;
        sumLastNbAvice=0;
        learner_reached_goal = 0;
        this->state->learner_state = prob->getState();
        this->state->episod = 0;
        this->state->givenFdb = 0;

        if(alreadyInit)
            delete this->state->learner_action;
        this->state->learner_action = prob->getInitialAction();
        lstep = 0;
        lreward = 0;
        treward = 0;
        advice_limit_per_ep = MAX_NUMBER_ADVICE;
        tookBestAction = false;
        gotGreedy = false;
        run_stats.clear();

        alreadyInit=true;
    }

private:
    Environnement<EnvState>* prob;
    RLSimulation<EnvState, PolicyState, StateType>* learner;
    Policy<PolicyState>* best_policy;

    Policy<TeacherPolicyState>* tpolicy;
    bool giveAdvise;
    bool tookBestAction;

    float advice_cost;
    int best_policy_teacher;
    int nbAdvice;
    int sumLastNbAvice;
    int lstep;
    int advice_limit_per_ep;
    double lreward;
    double treward;
    int learner_reached_goal;
    bool alreadyInit;
    bool gotGreedy;
    list<Tstats> run_stats;
    AdviseStrategy astart;
    StrategyEffectsAdvice sea;
};


template< typename EnvState>
using DDTeacher = Teacher<EnvState, DState, DiscretizeSelection, DState, DiscretizeTSelection>;

template< typename EnvState>
using CDTeacher = Teacher<EnvState, EnvState, ContinuousSelection, DState, DiscretizeTSelection>;

template< typename EnvState, typename TeacherPolicyState>
using DCTeacher = Teacher<EnvState, DState, DiscretizeSelection, TeacherPolicyState, ContinuousTSelection>;

template< typename EnvState, typename TeacherPolicyState>
using CCTeacher = Teacher<EnvState, EnvState, ContinuousSelection, TeacherPolicyState, ContinuousTSelection>;


}

#endif



