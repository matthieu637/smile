#ifndef MCAR_QLEARN_HPP
#define MCAR_QLEARN_HPP

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "sml/QLearning.hpp"
#include "simu/MCar.hpp"

using namespace simu;
using namespace sml;

class MCarQLearn : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MCarQLearn);
    //CPPUNIT_TEST(mcar_qltable_learner);
    CPPUNIT_TEST(mcar_qltable_teacher);

    CPPUNIT_TEST_SUITE_END();

public:

    void mcar_qltable_learner();
    void mcar_qltable_teacher();
};

#endif
