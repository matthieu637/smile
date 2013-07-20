#include "test/mcar_qlearn.hpp"
#include <simu/RLTable.hpp>
#include "simu/TeacherMCar.hpp"
#include <sml/Utils.hpp>

using namespace simu;
// using namespace test;

void MCarQLearn::mcar_qltable_learner() {
    time_t s = Utils::srand_mili();
//     Utils::srand_mili(0);

    bib::Logger::getInstance()->enableBuffer();
    simu::RLTable<MCarState> m(simu::QL, new MCar(8, 12));
    m.run();
    int score = m.keepRun(10)->back().min_step;
    LOG(score);

    srand(s);

    simu::RLTable<MCarState> m2(simu::QL_trace, new MCar(8, 12));
    m2.run();
    score = m2.keepRun(10)->back().min_step;
    LOG(score);

    bib::Logger::getInstance()->flushBuffer();
}


void MCarQLearn::mcar_qltable_teacher(float cost) {
    srand(time(NULL));

    TeacherMCar<FavorAdvice, MCarState>* teach = new TeacherMCar<FavorAdvice, MCarState>(new RLTable<MCarState>(simu::QL, new MCar(8, 12)), cost, simu::before);
    RLTable<TeacherState> r(simu::QL_trace, teach);
    r.run();
    std::list<stats>* l = r.keepRun(10000);
    
    bib::Logger::getInstance()->enableBuffer();
    for(std::list<stats>::iterator it = l->begin(); it != l->end(); ++it)
        LOG(it->nbStep);
    LOG_DEBUG(l->back().min_step << " / " << teach->get_best_policy_teacher());
    bib::Logger::getInstance()->flushBuffer();
}
