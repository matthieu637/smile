#ifndef TEACHINGBUDGET_H
#define TEACHINGBUDGET_H

#include "simu/Teacher.hpp"
#include "simu/RLSimulation.hpp"
#include <sml/Policy.hpp>

using namespace simu;

#define THRESHOLD 40.

struct statsTB
{
    int nbStep;
    double total_reward;
    int min_step;
    int index_min;
    int nbAdvice;
};

template<typename EnvState, typename PolicyState, typename StateType>
class TeachingBudget : private StateType
{
    using StateType::getState;

public:
    TeachingBudget(RLSimulation<EnvState, PolicyState, StateType>* learner, int n):
        learner(learner), n(n), nn(n)
    {
        learner->run();

        std::list<stats>* hist = learner->keepRun(STEP_REACH_RND_BEST_POL, true);
        delete hist;
        hist = learner->keepRun(STEP_REACH_BEST_POL, false);
        best_policy = learner->get_best_policy()->copyPolicy();
        delete hist;

        learner->reset();
        //         learner->get_policy()->setAdviseStrat(sea);

        fac = learner->getEnv()->getInitialAction();
        prob = learner->getEnv();
        agent = learner->get_policy();
    };
    
    ~TeachingBudget(){
	delete fac;
	delete best_policy;
    }

    std::list<statsTB>* keepRun(int additional_step, bool random_init=false) {
        std::list<statsTB>* stats_history = new std::list<statsTB>;

        additional_step--;
        prob->init(random_init);
        agent->clear_history(getState(prob), *fac);
        statsTB s = local_run(0);
        int min_step = s.min_step;
        int index_min = 0;
        int index = 1;
        stats_history->push_back(s);

        do {
            additional_step--;
            prob->init(random_init);
            agent->clear_history(getState(prob), *fac);
            statsTB s = local_run(index);

            if(s.nbStep <= min_step) {
                min_step = s.nbStep;
                index_min = index;
            }
            s.min_step = min_step;
            s.index_min = index_min;
            stats_history->push_back( s);
            index++;
        }
        while(additional_step > 0);

        return stats_history;
    }

    statsTB local_run(int index) {
        DAction* ac = new DAction(*fac);
        int step = 0;
        double total_reward = 0;
        do
        {
            step++;
            prob->apply(*ac);
	    delete ac;
            total_reward += prob->reward();


            ///
            Policy<PolicyState>* cp =  agent->copyPolicy();
            LearnReturn lr = cp->_learn(getState(prob), prob->reward());
            ac = new DAction(*lr.ac);
            delete cp;

            DAction* best_action = best_policy->decision(getState(prob), false);
// 	    LOG_DEBUG(!(*best_action == *ac) << " " << (nn > 0) << " " << best_policy->getStateImportance(getState(prob)) );
	    
            if(!(*best_action == *ac) && nn > 0 && best_policy->getStateImportance(getState(prob)) > THRESHOLD ) {
		delete ac;
                ac = new DAction(*best_action);
                nn--;
		agent->should_do(getState(prob), *ac, prob->reward());
            } else {
                agent->had_choosed(getState(prob), *ac, prob->reward(), lr.gotGreedy);
            }
            delete best_action;

        }
        while(!prob->goal() && step < (int)prob->maxStep());

	delete ac;
	
        return {step, total_reward, step, index, n - nn};
    }

protected:
    RLSimulation<EnvState, PolicyState, StateType>* learner;
    Environnement<EnvState>* prob;
    Policy<PolicyState>* best_policy, *agent;
    DAction* fac;
    int n, nn;
};

#endif // TEACHINGBUDGET_H
