#ifndef TEACHER_HPP
#define TEACHER_HPP

#include "simu/Environnement.hpp"
#include "simu/MCar.hpp"
#include <sml/Q.hpp>
#include <sml/QLearnGradient.hpp>
#include "simu/RLSimulation.hpp"
#include "GridWorldLS.hpp"

#define FDB "feedbacks"

#define LEARNER_STEP 100
// #define MAX_NUMBER_ADVICE 100
#define MAX_NUMBER_ADVICE 50

namespace simu {

template<typename EnvState>
struct TeacherState {
    EnvState learner_state;
    DAction* learner_action;
    bool learner_take_ba;
    int episod;
    int lstep;
    int givenFdb;
    float state_importance;
    bool gaveAdvise;
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
        best_policy = learner->search_best_policy(100);

        learner->reset();

        alreadyInit=false;
        this->init();
        limitedActions.push_back((int)false);
    }

    ~Teacher() {
        delete best_policy;
        delete this->state->learner_action;
    }

    void setTeacherPolicy(Policy<TeacherPolicyState>* tpol) {
        this->tpolicy = tpol;
    }

    double reward() const {
        if(prob->goal())
            return sml::Utils::transform(- prob->step, -500, 0, 50, 500);
	
//         float based = -2.;
//         if(!giveAdvise) { //don't give advice
//             if(tookBestAction) //agent take the best action , no need to advice
//                 return 0;
// //             return based;
// 
//         }
// //         return based*advice_cost;
	return -1;
    }

    DAction* getInitialAction() const {
        return new DAction(this->getActions(), (int) true);
    }

    RAction restrictedAction() {
        if(advice_budget > 0)
            return {false, nullptr};
        else
            return {true, &limitedActions};
    }

    bool goal() const {
        return this->state->episod > (LEARNER_STEP-1);
    }

    unsigned int maxStep() const {
        return prob->maxStep()*LEARNER_STEP;
    }

    int get_given_advice() const {
        return nbAdvice;
    }

    const list<Tstats>& get_learner_stats() const {
        return run_stats;
    }

protected:
    virtual void applyOn(const DAction& ac) {
//         giveAdvise = sml::Utils::rand01() <= 10./100. ;
        giveAdvise = ac[FDB];
//         giveAdvise = advice_budget > 0;
// 	giveAdvise = false;

        if(prob->done()) {
            run_stats.push_back( {lreward, prob->step, this->state->episod, treward, nbAdvice - sumLastNbAvice, (float)(nbAdvice - sumLastNbAvice) / (float) prob->step });
            sumLastNbAvice = nbAdvice;

            this->state->episod++;

            //called last time to get a +1
            if(astart == before)
                learner->computeNextAction(getState(prob), prob->reward(), prob->goal());

            learner_prob_init();
            giveAdvise = true ; //TODO:ok?
//             if(astart == before)
//                 delete learner_next_action;
//             learner_next_action = computePossibleNextAction();;
        } else {
            lreward += prob->reward();
            treward += reward();


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

// 	    LOG_DEBUG("state : " << state_learner << " action : " << *this->state->learner_action << " advice : " << *best_policy->decision(state_learner, 0) << " have advice " << giveAdvise );
                break;
            case before:
                if(giveAdvise && sea != None && advice_budget > 0) {
                    DAction* best_action = best_policy->decision(state_learner, false);
                    learner->get_policy()->should_do(state_learner, *best_action, prob->reward(), prob->goal());

                    tookBestAction = (*best_action == *this->state->learner_action);

                    prob->apply(*best_action);
                    delete best_action;
                    nbAdvice++;
                    advice_budget--;
                }
                else
                {
// 		take greeding in consideration
                    DAction* la = this->state->learner_action;
                    learner->get_policy()->had_choosed(state_learner, *la, prob->reward(), gotGreedy, prob->goal());

                    prob->apply(*la);
                }

                break;
            }
        }

// 	LOG_DEBUG(prob->getDState() << " " << *learner_next_action << " " << giveAdvise << " " << *(this->state->learner_action));

        EnvState car = prob->getState();
        this->state->lstep = prob->step;
        this->state->learner_state = car;
        this->state->givenFdb = nbAdvice - sumLastNbAvice;

        delete this->state->learner_action;
        if(astart == before)
            this->state->learner_action = computePossibleNextAction();
        else //after
            this->state->learner_action = new DAction(*learner->computeNextAction(getState(prob), prob->reward(), prob->goal()));

        setStatelearnerTakeBestAction();
    }

    void learner_prob_init() {
        prob->init();
        DAction* ac = prob->getInitialAction();
        learner->get_policy()->startEpisode(getState(prob), *ac);

        //TODO: should startEpisode of my own algo | try without
//             DAction* ia = getInitialAction();
//             tpolicy->startEpisode(getTState(this), *ia);
//             delete ia;
        prob->apply(*ac);
        delete ac;

        lreward = prob->reward();
        this->state->lstep = prob->step;
        treward = reward();
        gotGreedy = false;
    }

    DAction* computePossibleNextAction() {
        DAction* learner_next_action;
        Policy<PolicyState>* cp =  learner->get_policy()->copyPolicy();
        LearnReturn lr = cp->_learn(getState(prob), prob->reward(), prob->goal());
        learner_next_action = new DAction(*lr.ac);
        gotGreedy = lr.gotGreedy;
        delete cp;
        return learner_next_action;
    }

    //need to be call after state->learner_action is well updated
    void setStatelearnerTakeBestAction() {
        DAction* best_action = best_policy->decision(getState(prob), false);
        this->state->learner_take_ba = *best_action == *this->state->learner_action;
        this->state->state_importance = best_policy->getStateImportance(getState(prob));
        delete best_action;
    }

    void computeDState(const TeacherState<EnvState>& s, DState* dst, const StateTemplate* repr) {
// 	const GridWorldLSState ss = ((const GridWorldLSState&)prob->getState());
// 	LOG_DEBUG("begin with : " << *dst << " copy first {" << ss.x << "," << ss.y << "," << ss.p << "} and then " << *s.learner_action);
        prob->computeDState(prob->getState(), dst, repr);
// 	LOG_DEBUG(*dst);
        dst->copyValuesOf(*s.learner_action);
// 	LOG_DEBUG(*dst);
    }

    void initState(bool) {
        nbAdvice = 0;
        sumLastNbAvice=0;
        giveAdvise = false;

        this->state->episod = 0;
        this->state->givenFdb = 0;

        learner->reset();

        this->state->learner_state = prob->getState();

        if(alreadyInit)
            delete this->state->learner_action;
        this->state->learner_action = computePossibleNextAction();
        setStatelearnerTakeBestAction();

        advice_budget = MAX_NUMBER_ADVICE;
        tookBestAction = false;
        gotGreedy = false;
        run_stats.clear();

        learner_prob_init();

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
    int nbAdvice;
    int sumLastNbAvice;
    int advice_budget;
    double lreward;
    double treward;
    bool alreadyInit;
    bool gotGreedy;
    list<Tstats> run_stats;
    list<int> limitedActions;
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






