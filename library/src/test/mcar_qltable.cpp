#include "test/mcar_qlearn.hpp"
#include <test/MCarQLTable.hpp>
#include <sml/Utils.hpp>


void MCarQLearn::mcar_qltable_learner() {
    time_t s = Utils::srand_mili();
//     Utils::srand_mili(0);
    
//     bib::Logger::getInstance()->enableBuffer();

    test::MCarQLTable m(true);
    m.run();

    srand(s);

    test::MCarQLTable m2(false);
    m2.run();
    m2.keepRun(1000);
    
    bib::Logger::getInstance()->flushBuffer();
}
