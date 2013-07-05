#include "test/mcar_qlearn.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION( MCarQLearn );

// Standard RL parameters:
#define epsilon 0.01                    // probability of random action
#define alpha 0.5                      // step size parameter
#define lambda 0.9                     // trace-decay parameters
#define gamma 1.5                        // discount-rate parameters

#define nbPosStep 8
#define nbVelStep 12

StateTemplate t_stempl({POS, VEL, MOT},{nbPosStep, nbVelStep, 3});

DState* getTeachState(const DState& sl, const DAction& al){
  DState* ts = new DState(&t_stempl, {sl[POS], sl[VEL], al[MOT]});
  return ts;
}

int teach_reward(const DState& ts){
    return -1;
}

int mcar_qltable_teacher_run(MCar* prob, QLearning* teacher) {
    DAction* ac = new DAction(&prob->ACTION_TEMPLATE, 0);
    DAction* tac;

    QLearning learner(prob->stempl, &MCar::ACTION_TEMPLATE , *ac, *prob->computeDState() );

    int step = 0;
    do
    {
        step++;
        prob->step(*ac);
        DState* dst = prob->computeDState();
        ac = learner.learn(*dst, -1, alpha, epsilon, gamma);
        //ac = new  DAction(&prob->ACTION_TEMPLATE, 2);
        //LOG_DEBUG("etat " << *dst << " action " << *ac << " " << s.velocity );
	
	DState* ts = getTeachState(*dst, *ac);
 	tac = teacher->learn(*ts, teach_reward(*ts), alpha, epsilon, gamma);
	
	if( (*tac)[MOT] != 3)
	  learner.should_done(*dst, *tac, 10, alpha);
	
	//LOG_DEBUG("etat " << *dst << " action " << *tac << " " );
	
        //delete dst;
    }
    while(!prob->goal_p());
    
    DState* dst = prob->computeDState();
    DState* ts = getTeachState(*dst, *ac);
    tac = teacher->learn(*ts, 10, alpha, epsilon, gamma);

    LOG_DEBUG("DONE WITH " << step );

    return step;
}



void MCarQLearn::mcar_qltable_teacher() {
    srand(time(NULL));
    MCar prob(nbPosStep, nbVelStep);
    ActionTemplate t_atempl({MOT}, {4});
    
    DState tmp = *prob.computeDState();//mem
    QLearning teacher(&t_stempl, &t_atempl , DAction(&t_atempl, 3), DState(&t_stempl, {tmp[POS], tmp[VEL], 3}) );

    srand(time(NULL));
    int episod = 0;
    int score = 0;
    do
    {
        episod++;
        score += mcar_qltable_teacher_run(&prob, &teacher);
	prob.init();
    }
    while(episod < 1000);

    LOG_DEBUG("FINAL SCORE : " << (float)score/episod);
}




