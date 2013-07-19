#include "test/mcar_qlearn.hpp"
#include <simu/RLTable.hpp>
#include <sml/Utils.hpp>


void MCarQLearn::mcar_qltable_learner() {
    time_t s = Utils::srand_mili();
//     Utils::srand_mili(0);
    
    bib::Logger::getInstance()->enableBuffer();
    bib::Logger::getInstance()->setIgnoredBuffer(true);
    simu::RLTable<MCarState> m(simu::QL, new MCar(8, 12));
    m.run();
    int score = m.keepRun(10).min_step;
    bib::Logger::getInstance()->setIgnoredBuffer(false);
    LOG(score);

    srand(s);

    bib::Logger::getInstance()->setIgnoredBuffer(true);
    simu::RLTable<MCarState> m2(simu::QL_trace, new MCar(8, 12));
    m2.run();
    score = m2.keepRun(10).min_step;
    bib::Logger::getInstance()->setIgnoredBuffer(false);
    LOG(score);
    
    bib::Logger::getInstance()->flushBuffer();
}
