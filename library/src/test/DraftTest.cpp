#include "test/DraftTest.hpp"
#include <simu/RLTable.hpp>
#include "simu/DTeacher.hpp"
#include <simu/GridWorld.hpp>
#include <sml/Utils.hpp>

using namespace simu;
// using namespace test;

void DraftTest::FMQ1() {
    time_t s = Utils::srand_mili();
//     Utils::srand_mili(0);

    bib::Logger::getInstance()->enableBuffer();
    simu::RLTable<MCarState> m(simu::QL, new MCar(8, 12), MCarParam);
    int step = m.run().nbStep;
    std::list<stats>* l = m.keepRun(10000);
    LOG_DEBUG(l->back().min_step);
    LOG_DEBUG(step);

//     bib::Logger::getInstance()->enableBuffer();
//     for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
//         LOG(it->nbStep);
//     bib::Logger::getInstance()->flushBuffer();

    srand(s);

    simu::RLTable<MCarState> m2(simu::QL_trace, new MCar(8, 12), MCarParam);
    step = m2.run().nbStep;
    l = m2.keepRun(1000);
    LOG_DEBUG(l->back().min_step);
    LOG_DEBUG(step);

    bib::Logger::getInstance()->flushBuffer();

//     bib::Logger::getInstance()->enableBuffer();
//     for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
//         LOG(it->nbStep);
//     bib::Logger::getInstance()->flushBuffer();
}

void DraftTest::FGQ1() {
    time_t s = Utils::srand_mili();
//     Utils::srand_mili(0);

    bib::Logger::getInstance()->enableBuffer();
    RLTable<GridWorldState> m(simu::QL, new GridWorld, GridWorldParam);
    int step = m.run().nbStep;
    std::list<stats>* l = m.keepRun(10000);
    LOG_DEBUG(l->back().min_step);
    LOG_DEBUG(step);

//     bib::Logger::getInstance()->enableBuffer();
//     for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
//         LOG(it->nbStep);
//     bib::Logger::getInstance()->flushBuffer();

    srand(s);

    RLTable<GridWorldState> m2(simu::QL_trace, new GridWorld, GridWorldParam);
    step = m2.run().nbStep;
    l = m2.keepRun(1000);
    LOG_DEBUG(l->back().min_step);
    LOG_DEBUG(step);

//     bib::Logger::getInstance()->enableBuffer();
//     for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
//         LOG(it->nbStep);
//     bib::Logger::getInstance()->flushBuffer();
}


void DraftTest::mcar_qltable_teacher(float cost) {
//     srand(time(NULL));
    srand(0);

    MCar* leaner_env = new MCar(8, 12);
    RLTable<MCarState>* leaner_agent = new RLTable<MCarState>(simu::QL, leaner_env, MCarParam);


    StateTemplate teacher_repr(*leaner_env->getStates());
    teacher_repr.setSize(POS, 8);
    teacher_repr.setSize(VEL, 12);

    DTeacher<FavorAdvice, MCarState>* teach = new DTeacher<FavorAdvice, MCarState>(leaner_agent, teacher_repr, cost, simu::after, sml::FixedNMax);
    RLTable<TeacherState<MCarState> > r(simu::QL_trace, teach, MCarParam);
    r.run();
    std::list<stats>* l = r.keepRun(1000);

    bib::Logger::getInstance()->enableBuffer();
    for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
        LOG(it->nbStep);
    bib::Logger::getInstance()->flushBuffer();
}


void DraftTest::TGQ1Q2(float cost) {
//     srand(time(NULL));
    srand(0);

    GridWorld* leaner_env = new GridWorld();
    RLTable<GridWorldState>* leaner_agent = new RLTable<GridWorldState>(simu::QL, leaner_env, GridWorldParam);


    StateTemplate teacher_repr(*leaner_env->getStates());
//     teacher_repr.setSize(POS, 8);
//     teacher_repr.setSize(VEL, 12);

    DTeacher<FavorAdvice, GridWorldState>* teach = new DTeacher<FavorAdvice, GridWorldState>(leaner_agent, teacher_repr, cost, simu::after, sml::FixedNMax);
    RLTable< TeacherState<GridWorldState> > r(simu::QL_trace, teach, DefaultParam);
    r.run();
    std::list<stats>* l = r.keepRun(4000);

    bib::Logger::getInstance()->enableBuffer();
    for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
        LOG(it->nbStep);
    bib::Logger::getInstance()->flushBuffer();
    LOG_DEBUG(l->back().min_step);
    LOG_DEBUG(teach->get_best_policy_teacher());
}
