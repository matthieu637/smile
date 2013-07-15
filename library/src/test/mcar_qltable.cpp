#include "test/mcar_qlearn.hpp"

// CPPUNIT_TEST_SUITE_REGISTRATION( MCarQLearn );

// Standard RL parameters:
#define epsilon 0.01                    // probability of random action
#define alpha 0.5                      // step size parameter
#define lambda 0.9                     // trace-decay parameters
#define gamma 1.5                        // discount-rate parameters


void MCarQLearn::mcar_qltable_learner() {
    srand(time(NULL));

    MCar prob(8,12);
    DAction* ac = new DAction(prob.getActions(), 0);
    DAction* fac = ac;

    QLearning ql(prob.getStates(), prob.getActions() , *ac, prob.getDState() );

    int step = 0;
    do
    {
        step++;
        prob.apply(*ac);
	
        ac = ql.learn(prob.getDState(), -1, alpha, epsilon, gamma);
        
//      LOG_DEBUG("etat " << dst << " action " << *ac << " ");
    }
    while(!prob.goal());

//     LOG_DEBUG("DONE WITH " << step );
    
    delete fac;

    LOG(step);
}
