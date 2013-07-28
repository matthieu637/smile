#ifndef DRAFTTEST_HPP
#define DRAFTTEST_HPP

#include "bib/Logger.hpp"
#include <simu/RLTable.hpp>
#include <simu/Teacher.hpp>
#include <simu/GridWorldLS.hpp>
#include <simu/GridWorld.hpp>
#include <sml/Utils.hpp>
#include <boost/bind.hpp>

using namespace simu;

template<typename EnvState>
class Functor1D
{
public:
    Functor1D(int) {};
    double callPosition(const EnvState&, const DAction&) {
        return 0;
    }

    double callVelocity(const EnvState&, const DAction&) {
        return 0;
    }

    double callAction(const EnvState&, const DAction&) {
        return 0;
    }

};

template<>
class Functor1D<MCarState>
{
public:
    Functor1D(int i):i(i) {};
    double callPosition(const MCarState& st, const DAction& ac)
    {
        (void) ac;
        return sml::Utils::transform(st.position, -1.2, 0.6, 0., 1.8) + i*((1.8/8)/10);
    }

    double callVelocity(const MCarState& st, const DAction& ac)
    {
        (void) ac;
//     LOG_DEBUG(st.velocity << " " << sml::Utils::transform(st.velocity, -0.07, 0.07, 0., 0.14));
        return sml::Utils::transform(st.velocity, -0.07, 0.07, 0., 0.14) + i*((0.14/8)/10) ;
    }

    double callAction(const MCarState& st, const DAction& ac)
    {
        (void) st;
        return ((double)ac["motor"]) + i*((3/3)/10);
    }
    int i;
};

class DraftTest
{

public:

    template<typename EnvState>
    featuredList<EnvState>* getMCarFeatures() {

        featuredList<EnvState> *features = new featuredList<EnvState>();
        for(int i=0; i<10; i++) {

            typename Feature<EnvState>::featuring1D fonctor1 = boost::bind(&Functor1D<EnvState>::callPosition, new Functor1D<EnvState>(i), _1, _2);
            typename Feature<EnvState>::featuring1D fonctor2 = boost::bind(&Functor1D<EnvState>::callVelocity, new Functor1D<EnvState>(i), _1, _2);
            typename Feature<EnvState>::featuring1D fonctor3 = boost::bind(&Functor1D<EnvState>::callAction, new Functor1D<EnvState>(i), _1, _2);


            Feature<EnvState> f( {fonctor1, fonctor2, fonctor3}, { 8, 1.8, 8, 0.14, 3, 3});
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

        int step = s->run().nbStep;
        std::list<stats>* l = s->keepRun(numberRun);
        LOG_DEBUG(l->back().min_step);
        LOG_DEBUG(step);

        bib::Logger::getInstance()->enableBuffer();
        for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
            LOG(it->nbStep);
        bib::Logger::getInstance()->flushBuffer();
	
	delete s;
	delete l;
	delete features;
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
