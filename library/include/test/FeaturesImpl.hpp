#ifndef FEATURES_IMPL_HPP
#define FEATURES_IMPL_HPP

#include "bib/Logger.hpp"
#include <simu/RLTable.hpp>
#include <simu/Teacher.hpp>
#include <simu/GridWorldLS.hpp>
#include <simu/GridWorld.hpp>
#include <sml/Utils.hpp>
#include <boost/bind.hpp>

using namespace simu;

#define M 8.
#define K 16.
// #define K 8.
// #define M 16.

#define G_K1 10.
#define G_K2 5.
#define G_M  1.

static const RLParam MCarParam = {0.05, 0.08, 0.9, 1., true, -250., (int) M, 40., 40.};
static const RLParam GridWorldParam= {0.05, 0.08, 0.9, 0.8, false, -2., (int) G_M, 22., 18.85};

static const RLParam GridWorldLSParam= {0.05, 0.08, 0.9, 0.6, false, 0., (int) M, 0., 0.};
static const RLParam DefaultParam= {0.05, 0.08, 0.9, 0.6, false, 0., (int) M, 0., 0.};

class Functor1D
{
};


class Functor1DMCarState : public Functor1D
{
public:
    Functor1DMCarState(int i):i(i) {};

    double callPosition(const MCarState& st, const DAction& ac)
    {
        (void) ac;
//         return sml::Utils::transform(st.position, -1.2, 0.6, 0., 1.8) - (1.8/K) + i*(((2.*1.8)/K)/M);
	   return sml::Utils::transform(st.position, -1.2, 0.6, 0., 1.8) - (1.8/K) + p;
    }

    double callVelocity(const MCarState& st, const DAction& ac)
    {
        (void) ac;
//         return sml::Utils::transform(st.velocity, -0.07, 0.07, 0., 0.14) - (0.14/K) + i*(((2.*0.14)/K)/M) ;
	return sml::Utils::transform(st.velocity, -0.07, 0.07, 0., 0.14) - (0.14/K) + v ;
    }

    double callAction(const MCarState& st, const DAction& ac)
    {
        (void) st;
        return ((double)ac["motor"]);
    }
    double i;
    float p = sml::Utils::randin(0.,(2.*1.8)/K) ;
    float v = sml::Utils::randin(0.,(2.*0.14)/K) ;
};



class Functor1GridWorldState : public Functor1D
{
public:
    Functor1GridWorldState(int i):i(i) {};

    double callX(const GridWorldState& st, const DAction& ac)
    {
        (void) ac;
        return st.x /*+ i/G_M*/;
    }

    double callY(const GridWorldState& st, const DAction& ac)
    {
        (void) ac;
        return st.y /*+ i/G_M*/ ;
    }

    double callGoal(const GridWorldState& st, const DAction& ac)
    {
        (void) ac;
        return st.currentGoal /*+ i/G_M*/ ;
    }

    double callAction(const GridWorldState& st, const DAction& ac)
    {
        (void) st;
        return ((double)ac["move"]) /*+ i*((3/3)/M)*/;
    }
    double i;
};

class Functor1DTeacherStateMCarState : public Functor1D
{
public:
    Functor1DTeacherStateMCarState(int i):i(i) {};
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
        return ((double)st.learner_action->get("motor")) + i*((3/3)/M);
    }

    double callAction2(const TeacherState<MCarState>& st, const DAction& ac)
    {
        (void) st;
        return ((double)ac["feedbacks"]) /*+ i*((2/2)/M)*/;
    }

    double callEpisod(const TeacherState<MCarState>& st, const DAction& ac)
    {
        (void) ac;
        return st.episod + i*((100./30.)/M);
    }

    double callGivenFdb(const TeacherState<MCarState>& st, const DAction& ac)
    {
        (void) ac;
        return st.givenFdb + i*((100./30.)/M);;
    }

    int i;
};

// class Functor1TeacherGridWorldState : public Functor1D
// {
// public:
//     Functor1GridWorldState(int i):i(i) {};
// 
//     double callX(const GridWorldState& st, const DAction& ac)
//     {
//         (void) ac;
//         return st.x /*+ i/G_M*/;
//     }
// 
//     double callY(const GridWorldState& st, const DAction& ac)
//     {
//         (void) ac;
//         return st.y /*+ i/G_M*/ ;
//     }
// 
//     double callGoal(const GridWorldState& st, const DAction& ac)
//     {
//         (void) ac;
//         return st.currentGoal /*+ i/G_M*/ ;
//     }
// 
//     double callAction(const GridWorldState& st, const DAction& ac)
//     {
//         (void) st;
//         return ((double)ac["move"]) /*+ i*((3/3)/M)*/;
//     }
//     double i;
// };
// 

template<typename EnvState>
struct f_crea {
    Feature<EnvState>* f;
    Functor1D* inst_call;
};

template<typename EnvState>
struct featureData {
    featuredList<EnvState>* func;
    list< Functor1D* >* inst;
};

class Factory {

public:

    template<typename EnvState>
    static f_crea<EnvState> createFeature(int) {
        LOG_ERROR("env fonctions not implemented");
        return {nullptr, nullptr};
    }
};

template<>
f_crea<MCarState> Factory::createFeature(int tiling) {
    Functor1DMCarState* inst_call = new Functor1DMCarState(tiling);
    typename Feature<MCarState>::featuring1D fonctor1 = boost::bind(&Functor1DMCarState::callPosition, inst_call, _1, _2);
    typename Feature<MCarState>::featuring1D fonctor2 = boost::bind(&Functor1DMCarState::callVelocity, inst_call, _1, _2);
    typename Feature<MCarState>::featuring1D fonctor3 = boost::bind(&Functor1DMCarState::callAction, inst_call, _1, _2);


    Feature<MCarState>* f = new Feature<MCarState>( {fonctor1, fonctor2, fonctor3}, { K, 1.8, K, 0.14, 3, 3});
    return {f, inst_call};
}

template<>
f_crea<GridWorldState> Factory::createFeature(int tiling) {
    Functor1GridWorldState* inst_call = new Functor1GridWorldState(tiling);
    typename Feature<GridWorldState>::featuring1D fonctor1 = boost::bind(&Functor1GridWorldState::callX, inst_call, _1, _2);
    typename Feature<GridWorldState>::featuring1D fonctor2 = boost::bind(&Functor1GridWorldState::callY, inst_call, _1, _2);
    typename Feature<GridWorldState>::featuring1D fonctor3 = boost::bind(&Functor1GridWorldState::callGoal, inst_call, _1, _2);
    typename Feature<GridWorldState>::featuring1D fonctor4 = boost::bind(&Functor1GridWorldState::callAction, inst_call, _1, _2);

    Feature<GridWorldState>* f = new Feature<GridWorldState>( {fonctor1, fonctor2, fonctor3, fonctor4}, { G_K1, 10, G_K1, 10, G_K2, 5, 4, 4 });
    return {f, inst_call};
}

template<>
f_crea<TeacherState<MCarState>> Factory::createFeature(int tiling) {

    Functor1DTeacherStateMCarState* inst_call = new Functor1DTeacherStateMCarState(tiling);
    typename Feature<TeacherState<MCarState>>::featuring1D fonctor1 = boost::bind(&Functor1DTeacherStateMCarState::callPosition, inst_call, _1, _2);
    typename Feature<TeacherState<MCarState>>::featuring1D fonctor2 = boost::bind(&Functor1DTeacherStateMCarState::callVelocity, inst_call, _1, _2);
    typename Feature<TeacherState<MCarState>>::featuring1D fonctor3 = boost::bind(&Functor1DTeacherStateMCarState::callAction, inst_call, _1, _2);
    typename Feature<TeacherState<MCarState>>::featuring1D fonctor4 = boost::bind(&Functor1DTeacherStateMCarState::callAction2, inst_call, _1, _2);
    /*            typename Feature<TeacherState<MCarState>>::featuring1D fonctor5 = boost::bind(&Functor1DTeacherStateMCarState::callEpisod, inst_call, _1, _2);
                typename Feature<TeacherState<MCarState>>::featuring1D fonctor6 = boost::bind(&Functor1DTeacherStateMCarState::callGivenFdb, inst_call, _1, _2);
    */

    Feature<TeacherState<MCarState>>* f = new Feature<TeacherState<MCarState>>( {fonctor1, fonctor2, fonctor3, fonctor4/*, fonctor5, fonctor6*/}, { K, 1.8, K, 0.14, 3, 3, 2, 2/*, 30, 100, 30, 100*/});
    return {f, inst_call};
}

// template<>
// f_crea<GridWorldState> Factory::createFeature(int tiling) {
//     Functor1GridWorldState* inst_call = new Functor1GridWorldState(tiling);
//     typename Feature<GridWorldState>::featuring1D fonctor1 = boost::bind(&Functor1GridWorldState::callX, inst_call, _1, _2);
//     typename Feature<GridWorldState>::featuring1D fonctor2 = boost::bind(&Functor1GridWorldState::callY, inst_call, _1, _2);
//     typename Feature<GridWorldState>::featuring1D fonctor3 = boost::bind(&Functor1GridWorldState::callGoal, inst_call, _1, _2);
//     typename Feature<GridWorldState>::featuring1D fonctor4 = boost::bind(&Functor1GridWorldState::callAction, inst_call, _1, _2);
// 
//     Feature<GridWorldState>* f = new Feature<GridWorldState>( {fonctor1, fonctor2, fonctor3, fonctor4}, { G_K1, 10, G_K1, 10, G_K2, 5, 4, 4 });
//     return {f, inst_call};
// }

#endif
