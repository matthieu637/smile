#ifndef DRAFTTEST_HPP
#define DRAFTTEST_HPP

#include "bib/Logger.hpp"
#include <simu/RLTable.hpp>
#include <simu/Teacher.hpp>
#include <simu/GridWorldLS.hpp>
#include <simu/GridWorld.hpp>
#include <simu/Teacher.hpp>
#include <sml/Utils.hpp>
#include <boost/bind.hpp>

using namespace simu;

template<typename EnvState>
class Functor1D
{
public:
    Functor1D(int, float*, float*) {};
    double callPosition(const EnvState&, const DAction&) {
        return 0;
    }

    double callVelocity(const EnvState&, const DAction&) {
        return 0;
    }

    double callAction(const EnvState&, const DAction&) {
        return 0;
    }

    double callAction2(const EnvState&, const DAction&) {
        return 0;
    }

};

#define M 8
#define K 16


// sort(rand(1,8-1)*(0.14))
template<>
class Functor1D<MCarState>
{
public:
    Functor1D(int i, float* rp, float* rv):i(i), rp(rp), rv(rv) {};
    
//     double rp[K] = {0, 0.12990,   0.18150,   0.39213,   0.51785,   0.84513,   1.45134,   1.75680};
    double callPosition(const MCarState& st, const DAction& ac)
    {
        (void) ac;
        return sml::Utils::transform(st.position, -1.2, 0.6, 0., 1.8) + i*((1.8/K)/M);
// 	        return sml::Utils::transform(st.position, -1.2, 0.6, 0., 1.8) + rp[i];
    }

    double callVelocity(const MCarState& st, const DAction& ac)
    {
        (void) ac;
        return sml::Utils::transform(st.velocity, -0.07, 0.07, 0., 0.14) + i*((0.14/K)/M) ;
// 	  return sml::Utils::transform(st.velocity, -0.07, 0.07, 0., 0.14) + rv[i] ;
    }

    double callAction(const MCarState& st, const DAction& ac)
    {
        (void) st;
        return ((double)ac["motor"]) /*+ i*((3/3)/M)*/;
    }
    int i;
    float *rp, *rv;
};

template<>
class Functor1D<TeacherState<MCarState>>
{
public:
    Functor1D(int i, float*, float*):i(i) {};
    double callPosition(const TeacherState<MCarState>& st, const DAction& ac)
    {
        (void) ac;
        return sml::Utils::transform(st.learner_state.position, -1.2, 0.6, 0., 1.8) + i*((1.8/K)/M);
    }

    double callVelocity(const TeacherState<MCarState>& st, const DAction& ac)
    {
        (void) ac;
        return sml::Utils::transform(st.learner_state.velocity, -0.07, 0.07, 0., 0.14) + i*((0.14/K)/M) ;
    }

    double callAction(const TeacherState<MCarState>& st, const DAction& ac)
    {
        (void) ac;
        return ((double)st.learner_action->get("motor")) /*+ i*((3/3)/M)*/;
    }

    double callAction2(const TeacherState<MCarState>& st, const DAction& ac)
    {
        (void) st;
        return ((double)ac["feedbacks"]) /*+ i*((2/2)/M)*/;
    }
    int i;
};

class DraftTest
{

public:

    template<typename EnvState>
    featuredList<EnvState>* getMCarFeatures() {

        featuredList<EnvState> *features = new featuredList<EnvState>();
	float * rv = sml::Utils::genNrand(K, 0.14);
	float * rp = sml::Utils::genNrand(K, 1.8);
        for(int i=0; i<M; i++) {

            typename Feature<EnvState>::featuring1D fonctor1 = boost::bind(&Functor1D<EnvState>::callPosition, new Functor1D<EnvState>(i, rp, rv), _1, _2);
            typename Feature<EnvState>::featuring1D fonctor2 = boost::bind(&Functor1D<EnvState>::callVelocity, new Functor1D<EnvState>(i, rp, rv), _1, _2);
            typename Feature<EnvState>::featuring1D fonctor3 = boost::bind(&Functor1D<EnvState>::callAction, new Functor1D<EnvState>(i, rp, rv), _1, _2);


            Feature<EnvState> f( {fonctor1, fonctor2, fonctor3}, { K, 1.8, K, 0.14, 3, 3});
            features->push_back(f);
        }

        return features;
    }

    template<typename EnvState>
    featuredList<EnvState>* getTMCarFeatures() {

        featuredList<EnvState> *features = new featuredList<EnvState>();
        for(int i=0; i<M; i++) {

            typename Feature<EnvState>::featuring1D fonctor1 = boost::bind(&Functor1D<EnvState>::callPosition, new Functor1D<EnvState>(i, nullptr, nullptr), _1, _2);
            typename Feature<EnvState>::featuring1D fonctor2 = boost::bind(&Functor1D<EnvState>::callVelocity, new Functor1D<EnvState>(i, nullptr, nullptr), _1, _2);
            typename Feature<EnvState>::featuring1D fonctor3 = boost::bind(&Functor1D<EnvState>::callAction, new Functor1D<EnvState>(i, nullptr, nullptr), _1, _2);
            typename Feature<EnvState>::featuring1D fonctor4 = boost::bind(&Functor1D<EnvState>::callAction2, new Functor1D<EnvState>(i, nullptr, nullptr), _1, _2);


            Feature<EnvState> f( {fonctor1, fonctor2, fonctor3, fonctor4}, { K, 1.8, K, 0.14, 3, 3, 2, 2});
            features->push_back(f);
        }

        return features;
    }

    template<typename EnvState>
    void F_run_simple(Algo a, Environnement<EnvState>* env, RLParam p, int numberRun) {
        Utils::srand_mili();

        Simulation* s;
        featuredList<EnvState> *features;

        if(a == simu::QL_gen || a == simu::Sarsa_gen ) {
            unsigned int nbFeature = 0;
            features = getMCarFeatures<EnvState>();
            for(fLiterator<EnvState> flist = features->begin() ; flist != features->end(); ++flist) {
                nbFeature += flist->getSize();
            }
            LOG_DEBUG(nbFeature);
            s = new RLGradient<EnvState>(a, env, p, features, nbFeature);
        }
        else s = new RLTable<EnvState>(a, env, p);

	s->init();
        int step = s->run().nbStep;
        std::list<stats>* l = s->keepRun(numberRun);
        LOG_DEBUG(l->back().min_step);
        LOG_DEBUG(step);

        bib::Logger::getInstance()->enableBuffer();
        for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
            LOG(it->total_reward);
        bib::Logger::getInstance()->flushBuffer();

        delete s;
        delete l;
        delete features;
    }

    template<typename EnvState, typename PolicyReward>
    void T_run_simple(Algo algoLearn, Algo algoTeach, Environnement<EnvState>* learner_env, RLParam paramLearn,
                      RLParam paramTeach, bool same_rpr, AdviseStrategy as, StrategyEffectsAdvice sea, float cost, int numberRun) {
        if(algoLearn == simu::QL_gen || algoLearn == simu::Sarsa_gen )
            T_run_simple_DD<EnvState, PolicyReward>(algoLearn, algoTeach, learner_env, paramLearn, paramTeach, same_rpr, as, sea, cost, numberRun);
        else
            T_run_simple_CD<EnvState, PolicyReward>(algoLearn, algoTeach, learner_env, paramLearn, paramTeach, same_rpr, as, sea, cost, numberRun);
    }


    template<typename EnvState, typename PolicyState, typename StateType, typename PolicyReward, typename TeacherPolicyState>
    ATeacher<TeacherPolicyState> createTeacher(Algo algoLearn, Algo algoTeach, RLSimulation<EnvState, PolicyState, StateType>* learner,
            const StateTemplate& teacher_repr, AdviseStrategy as, StrategyEffectsAdvice sea, float cost) {
        if(algoTeach == simu::QL_gen || algoTeach == simu::Sarsa_gen ) {
            if(algoLearn == simu::QL_gen || algoLearn == simu::Sarsa_gen )
                return new CCTeacher<PolicyReward, EnvState, TeacherState<EnvState>>(learner, teacher_repr, cost, as, sea);
            else
                return new DCTeacher<PolicyReward, EnvState, TeacherState<EnvState>>(learner, teacher_repr, cost, as, sea);
        } else {
            if(algoLearn == simu::QL_gen || algoLearn == simu::Sarsa_gen )
                return new CDTeacher<PolicyReward, EnvState>(learner, teacher_repr, cost, as, sea);
            else
                return new DDTeacher<PolicyReward, EnvState>(learner, teacher_repr, cost, as, sea);
        }
    }

    template<typename EnvState, typename PolicyReward>
    void T_run_simple_DD(Algo algoLearn, Algo algoTeach, Environnement<EnvState>* learner_env, RLParam paramLearn,
                         RLParam paramTeach, bool same_rpr, AdviseStrategy as, StrategyEffectsAdvice sea, float cost, int numberRun) {
        Utils::srand_mili();

        RLTable<EnvState>* learner_agent = new RLTable<EnvState>(algoLearn, learner_env, paramLearn);


        StateTemplate teacher_repr(*learner_env->getStates());
        if(!same_rpr) {
            teacher_repr.setSize(POS, 16);
            teacher_repr.setSize(VEL, 24);
        }

        DDTeacher<PolicyReward, EnvState>* teach = new DDTeacher<PolicyReward, EnvState>(learner_agent, teacher_repr, cost, as, sea);
        RLTable<TeacherState<EnvState> > r(algoTeach, teach, paramTeach);
	r.init();
        r.run();
        std::list<stats>* l = r.keepRun(numberRun);

        bib::Logger::getInstance()->enableBuffer();
        for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
            LOG(it->nbStep);
        bib::Logger::getInstance()->flushBuffer();

        delete teach;
        delete learner_agent;
    }

    template<typename EnvState, typename PolicyReward>
    void T_run_simple_CD(Algo algoLearn, Algo algoTeach, Environnement<EnvState>* learner_env, RLParam paramLearn,
                         RLParam paramTeach, bool same_rpr, AdviseStrategy as, StrategyEffectsAdvice sea, float cost, int numberRun) {
        Utils::srand_mili();


        unsigned int nbFeature = 0;
        featuredList<EnvState> * features = getMCarFeatures<EnvState>();
        for(fLiterator<EnvState> flist = features->begin() ; flist != features->end(); ++flist) {
            nbFeature += flist->getSize();
        }
        RLGradient<EnvState>* learner_agent = new RLGradient<EnvState>(algoLearn, learner_env, paramLearn, features, nbFeature);


        StateTemplate teacher_repr(*learner_env->getStates());
        if(!same_rpr) {
            teacher_repr.setSize(POS, 16);
            teacher_repr.setSize(VEL, 24);
        }

        CDTeacher<PolicyReward, EnvState>* teach = new CDTeacher<PolicyReward, EnvState>(learner_agent, teacher_repr, cost, as, sea);
        RLTable<TeacherState<EnvState> > r(algoTeach, teach, paramTeach);
        r.init();
        teach->setTeacherPolicy(r.get_policy());

        r.run();

        bib::Logger::getInstance()->enableBuffer();


        std::list<stats>* l = r.keepRun(numberRun);
        bib::Logger::getInstance()->setIgnoredBuffer(true);
        for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
            LOG(it->nbStep);
        bib::Logger::getInstance()->flushBuffer();


        bib::Logger::getInstance()->setIgnoredBuffer(false);
        const list<Tstats>& s = teach->get_learner_stats();
        for(list<Tstats>::const_iterator it = s.cbegin(); it != s.cend(); ++it)
            LOG(it->lreward);
        bib::Logger::getInstance()->flushBuffer();


        delete teach;
        delete learner_agent;
        delete features;
    }

    void runme() {
        Environnement<MCarState> *learner_env = new MCar(8, 8);
        Utils::srand_mili();

        unsigned int nbFeature = 0;
        featuredList<MCarState> * features = getMCarFeatures<MCarState>();
        for(fLiterator<MCarState> flist = features->begin() ; flist != features->end(); ++flist) {
            nbFeature += flist->getSize();
        }
        unsigned int nbTFeature = 0;
        featuredList<TeacherState<MCarState>> * tfeatures = getTMCarFeatures<TeacherState<MCarState>>();
        for(fLiterator<TeacherState<MCarState>> flist = tfeatures->begin() ; flist != tfeatures->end(); ++flist) {
            nbTFeature += flist->getSize();
        }


        RLSimulation<MCarState, MCarState, ContinuousSelection>* learner_agent = new RLGradient<MCarState>(simu::QL_gen, learner_env, MCarParam, features, nbFeature);

        CCTeacher<CostlyAdvise, MCarState, TeacherState<MCarState>>* teach = new CCTeacher<CostlyAdvise, MCarState, TeacherState<MCarState>>(learner_agent, *learner_env->getStates(), 1.5, before, Max);
// 	CCTeacher<FavorAdvice, MCarState, TeacherState<MCarState>>* teach = new CCTeacher<FavorAdvice, MCarState, TeacherState<MCarState>>(learner_agent, *learner_env->getStates(), 1.5, before, Max);
	//         RLTable<TeacherState<MCarState> > r(QL_gen, teach, DefaultParam);

        RLGradient<TeacherState<MCarState>> r(QL_gen, teach, MCarParam, tfeatures, nbTFeature);
	r.init();
        teach->setTeacherPolicy(r.get_policy());

        r.run();

        bib::Logger::getInstance()->enableBuffer();


        std::list<stats>* l = r.keepRun(200);

	LOG("#");
        for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
            LOG(it->total_reward);
        bib::Logger::getInstance()->flushBuffer();

	LOG("#");
        const list<Tstats>& s = teach->get_learner_stats();
        for(list<Tstats>::const_iterator it = s.cbegin(); it != s.cend(); ++it)
            LOG(it->treward);
        bib::Logger::getInstance()->flushBuffer();
	
	
	for(list<Tstats>::const_iterator it = s.cbegin(); it != s.cend(); ++it)
            LOG(it->lreward);
        bib::Logger::getInstance()->flushBuffer();
	
	
	LOG("#");
	LOG(teach->get_given_advice());
	bib::Logger::getInstance()->flushBuffer();


        delete teach;
        delete learner_agent;
        delete features;

    }
};

#endif
