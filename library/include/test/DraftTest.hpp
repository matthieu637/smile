#ifndef DRAFTTEST_HPP
#define DRAFTTEST_HPP

#include "bib/Logger.hpp"
#include <simu/RLTable.hpp>
#include <simu/DTeacher.hpp>
#include <simu/GridWorldLS.hpp>
#include <simu/GridWorld.hpp>
#include <sml/Utils.hpp>

using namespace simu;

class DraftTest
{

public:

    template<typename EnvState>
    void F_run_simple(Algo a, Environnement<EnvState>* env, RLParam p, int numberRun) {
        Utils::srand_mili();

        simu::RLTable<EnvState> m(a, env, p);
        int step = m.run().nbStep;
        std::list<stats>* l = m.keepRun(numberRun);
        LOG_DEBUG(l->back().min_step);
        LOG_DEBUG(step);

        bib::Logger::getInstance()->enableBuffer();
        for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
            LOG(it->nbStep);
        bib::Logger::getInstance()->flushBuffer();
    }

    template<typename EnvState, typename PolicyReward>
    void T_run_simple(Algo algoLearn, Algo algoTeach, Environnement<EnvState>* learner_env, RLParam paramLearn,
                      RLParam paramTeach, bool same_rpr, AdviseStrategy as, StrategyEffectsAdvice sea, float cost, int numberRun) {
        Utils::srand_mili();

        RLTable<EnvState>* learner_agent = new RLTable<EnvState>(algoLearn, learner_env, paramLearn);


        StateTemplate teacher_repr(*learner_env->getStates());
        if(!same_rpr) {
            teacher_repr.setSize(POS, 16);
            teacher_repr.setSize(VEL, 24);
        }

        DTeacher<PolicyReward, EnvState>* teach = new DTeacher<PolicyReward, EnvState>(learner_agent, teacher_repr, cost, as, sea);
        RLTable<TeacherState<EnvState> > r(algoTeach, teach, paramTeach);
        r.run();
        std::list<stats>* l = r.keepRun(numberRun);

        bib::Logger::getInstance()->enableBuffer();
        for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
            LOG(it->nbStep);
        bib::Logger::getInstance()->flushBuffer();
	
	delete teach;
	delete learner_agent;
    }
};

#endif
