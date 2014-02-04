#ifndef DRAFTTEST_HPP
#define DRAFTTEST_HPP

#include <boost/bind.hpp>
#include <simu/RLTable.hpp>
#include <simu/Teacher.hpp>
#include <simu/GridWorldLS.hpp>
#include <simu/GridWorld.hpp>
#include <simu/Teacher.hpp>
#include <sml/Utils.hpp>
#include "bib/Logger.hpp"
#include "FeaturesImpl.hpp"
#include "TeachingBudget.hpp"
#include "sml/Feature.hpp"

using namespace simu;


class DraftTest
{

public:

    template<typename EnvState>
    void F_run_simple(Algo a, Environnement<EnvState>* env, RLParam p, int numberRun, bool no_learn_knowledge) {
        Utils::srand_mili();

        Simulation* s;
        featureData<EnvState> features;

        if(a == simu::QL_gen || a == simu::Sarsa_gen ) {
            unsigned int nbFeature = 0;
            features = Factory::createFeatures<EnvState>(p);
            for(fLiterator<EnvState> flist = features.func->begin() ; flist != features.func->end(); ++flist) {
                nbFeature += (*flist)->getSize();
            }
            LOG_DEBUG(nbFeature);
            s = new RLGradient<EnvState>(a, env, p, features.func, nbFeature, no_learn_knowledge, None);
        }
        else s = new RLTable<EnvState>(a, env, p, no_learn_knowledge);

        s->init();
        std::list<stats>* l = s->keepRun(numberRun);
        LOG_DEBUG(l->back().min_step);

        bib::Logger::getInstance()->enableBuffer();
        for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
            LOG(it->total_reward);
        bib::Logger::getInstance()->flushBuffer();

        LOG("#2");
        for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
            LOG(it->nbStep);
        bib::Logger::getInstance()->flushBuffer();

        if(a == simu::QL_gen || a == simu::Sarsa_gen) {
            for(fLiterator<EnvState> it = features.func->begin() ; it != features.func->end(); ++it)
                delete *it;
            for(list<Functor1D*>::iterator it = features.inst->begin(); it != features.inst->end() ; ++it)
                delete *it;
            delete features.func;
            delete features.inst;
        }

        delete s;
        delete l;

    }

//     template<typename EnvState, typename PolicyReward>
//     void T_run_simple(Algo algoLearn, Algo algoTeach, Environnement<EnvState>* learner_env, RLParam paramLearn,
//                       RLParam paramTeach, bool same_rpr, AdviseStrategy as, StrategyEffectsAdvice sea, float cost, int numberRun) {
//         if(algoLearn == simu::QL_gen || algoLearn == simu::Sarsa_gen )
//             T_run_simple_DD<EnvState, PolicyReward>(algoLearn, algoTeach, learner_env, paramLearn, paramTeach, same_rpr, as, sea, cost, numberRun);
//         else
//             T_run_simple_CD<EnvState, PolicyReward>(algoLearn, algoTeach, learner_env, paramLearn, paramTeach, same_rpr, as, sea, cost, numberRun);
//     }


//     template<typename EnvState, typename PolicyState, typename StateType, typename TeacherPolicyState>
//     ATeacher<TeacherPolicyState> createTeacher(Algo algoLearn, Algo algoTeach, RLSimulation<EnvState, PolicyState, StateType>* learner,
//             const StateTemplate& teacher_repr, AdviseStrategy as, StrategyEffectsAdvice sea, float cost) {
//         if(algoTeach == simu::QL_gen || algoTeach == simu::Sarsa_gen ) {
//             if(algoLearn == simu::QL_gen || algoLearn == simu::Sarsa_gen )
//                 return new CCTeacher<EnvState, TeacherState<EnvState>>(learner, teacher_repr, cost, as, sea);
//             else
//                 return new DCTeacher<EnvState, TeacherState<EnvState>>(learner, teacher_repr, cost, as, sea);
//         } else {
//             if(algoLearn == simu::QL_gen || algoLearn == simu::Sarsa_gen )
//                 return new CDTeacher<EnvState>(learner, teacher_repr, cost, as, sea);
//             else
//                 return new DDTeacher<EnvState>(learner, teacher_repr, cost, as, sea);
//         }
//     }


    template<typename EnvState>
    void T_run_simple(Environnement<EnvState>* learner_env, RLParam paramLearn, RLParam paramTeach, AdviseStrategy as,
                      StrategyEffectsAdvice sea, float cost, int numberRun, bool learn_knowledge, Algo algo) {
        Utils::srand_mili();
// 	Utils::srand_mili(true);

        unsigned int nbFeature = 0;
        featureData<EnvState> features = Factory::createFeatures<EnvState>(paramLearn);
        for(fLiterator<EnvState> flist = features.func->begin() ; flist != features.func->end(); ++flist) {
            nbFeature += (*flist)->getSize();
        }
        unsigned int nbTFeature = 0;
        featureData<TeacherState<EnvState>> tfeatures = Factory::createFeatures<TeacherState<EnvState>>(paramTeach);
        for(fLiterator<TeacherState<EnvState>> flist = tfeatures.func->begin() ; flist != tfeatures.func->end(); ++flist) {
            nbTFeature += (*flist)->getSize();
        }
//

        RLSimulation<EnvState, EnvState, ContinuousSelection>* learner_agent = new RLGradient<EnvState>(algo, learner_env, paramLearn, features.func, nbFeature, false, sea);
        learner_agent->init();

        CCTeacher< EnvState, TeacherState<EnvState>>* teach = new CCTeacher< EnvState, TeacherState<EnvState>>(learner_agent, *learner_env->getStates(), cost, as, sea);

        RLGradient<TeacherState<EnvState>> r(algo, teach, paramTeach, tfeatures.func, nbTFeature, learn_knowledge, None);
        r.init();
        teach->setTeacherPolicy(r.get_policy());

        bib::Logger::getInstance()->enableBuffer();

        std::list<stats>* l = r.keepRun(numberRun, false, true);

        LOG("#1");
        for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
            LOG(it->nbStep);
        bib::Logger::getInstance()->flushBuffer();

        LOG("#7");
        for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
            LOG(it->total_reward);
        bib::Logger::getInstance()->flushBuffer();

        delete l;


        for(int i=0; i<30; i++) {
            r.run_best(0);

            LOG("#2");
            const list<Tstats>& s = teach->get_learner_stats();
            for(list<Tstats>::const_iterator it = s.cbegin(); it != s.cend(); ++it)
                LOG(it->treward);
            bib::Logger::getInstance()->flushBuffer();

            LOG("#3");
            for(list<Tstats>::const_iterator it = s.cbegin(); it != s.cend(); ++it)
                LOG(it->lstep);
            bib::Logger::getInstance()->flushBuffer();

            LOG("#4");
            for(list<Tstats>::const_iterator it = s.cbegin(); it != s.cend(); ++it)
                LOG(it->nbAdvice);
            bib::Logger::getInstance()->flushBuffer();

            LOG("#5");
            for(list<Tstats>::const_iterator it = s.cbegin(); it != s.cend(); ++it)
                LOG(it->ratio_ad_ch);
            bib::Logger::getInstance()->flushBuffer();

            LOG("#6");
            LOG(teach->get_given_advice());
            bib::Logger::getInstance()->flushBuffer();

        }

        delete teach;
        delete learner_agent;


        for(fLiterator<EnvState> it = features.func->begin() ; it != features.func->end(); ++it)
            delete *it;
        for(list<Functor1D*>::iterator it = features.inst->begin(); it != features.inst->end() ; ++it)
            delete *it;
        delete features.func;
        delete features.inst;


        for(fLiterator<TeacherState<EnvState>> it = tfeatures.func->begin() ; it != tfeatures.func->end(); ++it)
            delete *it;
        for(list<Functor1D*>::iterator it = tfeatures.inst->begin(); it != tfeatures.inst->end() ; ++it)
            delete *it;
        delete tfeatures.func;
        delete tfeatures.inst;
    }

    template<typename EnvState>
    void runTeachingBudget(Environnement<EnvState>* learner_env, RLParam paramLearn, tb_strategy stra, int numberRun,
                           bool learn_knowledge, int numberAdvice, StrategyEffectsAdvice sea, Algo algo, TBparam tparam) {
        Utils::srand_mili();
// 	Utils::srand_mili(true);

        unsigned int nbFeature = 0;
        featureData<EnvState> features = Factory::createFeatures<EnvState>(paramLearn);
        for(fLiterator<EnvState> flist = features.func->begin() ; flist != features.func->end(); ++flist) {
            nbFeature += (*flist)->getSize();
        }

        RLSimulation<EnvState, EnvState, ContinuousSelection>* learner_agent = new RLGradient<EnvState>(algo, learner_env, paramLearn, features.func, nbFeature, false, sea);
        learner_agent->init();

        TeachingBudget<EnvState, EnvState, ContinuousSelection>* teacher = new TeachingBudget<EnvState, EnvState, ContinuousSelection>(learner_agent, numberAdvice, stra, learn_knowledge, tparam);
        std::list<statsTB>* l = teacher->keepRun(numberRun);

        bib::Logger::getInstance()->enableBuffer();

//         LOG("#1");
        for(std::list<statsTB>::iterator it = l->begin(); it != l->end(); ++it)
            LOG(it->total_reward);
        bib::Logger::getInstance()->flushBuffer();

        LOG("#2");
        for(std::list<statsTB>::iterator it = l->begin(); it != l->end(); ++it)
            LOG(it->nbAdvice);
        bib::Logger::getInstance()->flushBuffer();

        LOG("#3");
        for(std::list<statsTB>::iterator it = l->begin(); it != l->end(); ++it)
            LOG(it->nbStep);
        bib::Logger::getInstance()->flushBuffer();


        delete l;

        delete teacher;
        delete learner_agent;


        for(fLiterator<EnvState> it = features.func->begin() ; it != features.func->end(); ++it)
            delete *it;
        for(list<Functor1D*>::iterator it = features.inst->begin(); it != features.inst->end() ; ++it)
            delete *it;
        delete features.func;
        delete features.inst;
    }
};

#endif


