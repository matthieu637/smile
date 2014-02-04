#ifndef TEACHINGBUDGET_H
#define TEACHINGBUDGET_H

#include "simu/Teacher.hpp"
#include "simu/RLSimulation.hpp"
#include <sml/Policy.hpp>

using namespace simu;

struct statsTB
{
    int nbStep;
    double total_reward;
    int min_step;
    int index_min;
    int nbAdvice;
};


enum tb_strategy { mistake_correction, importance_advice, early_advice, pourcentage};

struct TBparam{
    float ia_threshold;
    float mc_threshold;
};

template<typename EnvState, typename PolicyState, typename StateType>
class TeachingBudget : private StateType
{
    using StateType::getState;

public:
    TeachingBudget(RLSimulation<EnvState, PolicyState, StateType>* learner, int n, tb_strategy str, bool no_teach_knowledge, TBparam tparam):
        learner(learner), n(n), nn(n), str(str), no_teach_knowledge(no_teach_knowledge),tparam(tparam)
    {
        best_policy = learner->search_best_policy(100);

        learner->reset();
        //         learner->get_policy()->setAdviseStrat(sea);

        fac = learner->getEnv()->getInitialAction();
        prob = learner->getEnv();
        agent = learner->get_policy();
    };

    ~TeachingBudget() {
        delete fac;
        delete best_policy;
    }

    std::list<statsTB>* keepRun(int additional_step, bool random_init=false) {
        std::list<statsTB>* stats_history = new std::list<statsTB>;

        additional_step--;
        statsTB s = local_run(0, random_init);
        int min_step = s.min_step;
        int index_min = 0;
        int index = 1;
        stats_history->push_back(s);

        do {
            additional_step--;
            statsTB s = local_run(index, random_init);

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

    statsTB local_run(int index, bool random_init) {
        prob->init(random_init);
        agent->startEpisode(getState(prob), *fac);
        if(no_teach_knowledge) {
            local_run_l(index);
            prob->init(random_init);
            agent->startEpisode(getState(prob), *fac);
            return local_run_nl(index);
        }

        return local_run_l(index);
    }

    statsTB local_run_l(int index) {
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
            LearnReturn lr = cp->_learn(getState(prob), prob->reward(), prob->goal());
            ac = new DAction(*lr.ac);
            delete cp;

            DAction* best_action = best_policy->decision(getState(prob), false);
// 	    LOG_DEBUG(!(*best_action == *ac) << " " << (nn > 0) << " " << best_policy->getStateImportance(getState(prob)) );

            bool gonna_advice = false;

            switch(str) {
            case early_advice:
                gonna_advice = nn > 0;
                break;
            case mistake_correction:
                gonna_advice = !(*best_action == *ac) && nn > 0 && best_policy->getStateImportance(getState(prob)) > tparam.mc_threshold ;
                break;
            case importance_advice:
                gonna_advice = nn > 0 && best_policy->getStateImportance(getState(prob)) > tparam.ia_threshold ;
                break;
            case pourcentage:
                gonna_advice = sml::Utils::rand01(0.1);
                break;
            }

            if(gonna_advice) {
                delete ac;
                ac = new DAction(*best_action);
                nn--;
                agent->should_do(getState(prob), *ac, prob->reward(), prob->goal());
            } else {
                agent->had_choosed(getState(prob), *ac, prob->reward(), lr.gotGreedy, prob->goal());
            }
            delete best_action;

        }
        while(!prob->done());

        delete ac;

        return {prob->step, total_reward, prob->step, index, n - nn};
    }

    statsTB local_run_nl(int index) {
        DAction* ac = fac;
        double total_reward = 0;
        do
        {
            prob->apply(*ac);
            total_reward += prob->reward();

            ac = agent->decision(getState(prob), false);
        }
        while(!prob->done());

        return {prob->step, total_reward, prob->step, index, n - nn};
    }

protected:
    RLSimulation<EnvState, PolicyState, StateType>* learner;
    Environnement<EnvState>* prob;
    Policy<PolicyState>* best_policy, *agent;
    DAction* fac;
    int n, nn;
    tb_strategy str;
    bool no_teach_knowledge;
    TBparam tparam;
};

#endif // TEACHINGBUDGET_H
