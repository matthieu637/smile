#include "test/mcar_qlearn.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION( MCarQLearn );

// Standard RL parameters:
#define epsilon 0.01                    // probability of random action
#define alpha 0.5                      // step size parameter
#define lambda 0.9                     // trace-decay parameters
#define gamma 1.5                        // discount-rate parameters


int mcar_qltable_run() {

    MCar prob(8,12);
    DAction* ac = new DAction(&prob.ACTION_TEMPLATE, 0);

    QLearning ql(prob.stempl, &MCar::ACTION_TEMPLATE , *ac, *prob.computeDState() );


    int step = 0;
    do
    {
        step++;
        prob.step(*ac);
        DState* dst = prob.computeDState();
        ac = ql.learn(*dst, -1, alpha, epsilon, gamma);
        //ac = new  DAction(&prob.ACTION_TEMPLATE, 2);
        //LOG_DEBUG("etat " << *dst << " action " << *ac << " " << s.velocity );
        //delete dst;
    }
    while(!prob.goal_p());

//    LOG_DEBUG("DONE WITH " << step );

    return step;
}

void MCarQLearn::mcar_qltable_learner() {
    srand(time(NULL));
    int episod = 0;
    int score = 0;
    do
    {
        episod++;
        score += mcar_qltable_run();
    }
    while(episod < 1000);

    LOG_DEBUG("FINAL SCORE : " << (float)score/episod);
    
}

