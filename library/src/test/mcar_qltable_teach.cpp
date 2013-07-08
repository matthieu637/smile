#include "test/mcar_qlearn.hpp"
#include <sml/QLearningLamb.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( MCarQLearn );

// Standard RL parameters:
#define epsilon 0.01                    // probability of random action
#define alpha 0.5                      // step size parameter
#define lambda 0.95                     // trace-decay parameters
#define gamma 0.8                        // discount-rate parameters

#define nbPosStep 8
#define nbVelStep 12

// 3 conditions to work : gamma < 1 | trace non-accumulative | reward depending of step

const StateTemplate MCarQLearn::t_stempl({POS, VEL, MOT},{nbPosStep, nbVelStep, 3});

DState* getTeachState(const DState& sl, const DAction& al){
  DState* ts = new DState(&MCarQLearn::t_stempl, {sl[POS], sl[VEL], al[MOT]});
  return ts;
}

int teach_reward(const DState& ts){
    return 1;
}

int mcar_qltable_teacher_run(MCar* prob, QLearningLamb* teacher) {
    DAction* ac = new DAction(&MCar::ACTION_TEMPLATE, 0);
    DAction* fac = ac;

    QLearning learner(prob->stempl, &MCar::ACTION_TEMPLATE , *ac, prob->getDState() );
    DState* ts;

    int step = 0;
    do
    {
        step++;
        prob->step(*ac);
        DState dst = prob->getDState();
        ac = learner.learn(dst, -1, alpha, epsilon, gamma);//here


	ts = getTeachState(dst, *ac);
	DAction* tac = teacher->learn(*ts, -1, alpha, epsilon, gamma, lambda, false);

	int aa = tac->get(MOT);
	if( aa != 3)
	  learner.should_done(dst, DAction(&MCar::ACTION_TEMPLATE, {aa}), 1, alpha);
	
        delete ts;
    }
    while(!prob->goal_p() && step < 50000);
    
     ts = getTeachState(prob->getDState(), *ac);
     teacher->learn(*ts, 3000-step, alpha, 0, 0, 1, false);
     delete ts;
     
     delete fac;

//      LOG_DEBUG("DONE WITH " << step );

    return step;
}



void MCarQLearn::mcar_qltable_teacher() {
    srand(time(NULL));
//     srand(0);

    MCar prob(nbPosStep, nbVelStep);
    ActionTemplate t_atempl({MOT}, {4});
    
    const DState& tmp = prob.getDState();
    DState fs(&MCarQLearn::t_stempl, {tmp[POS], tmp[VEL], 2});
    DAction fa(&t_atempl, 3);
    QLearningLamb teacher(&MCarQLearn::t_stempl, &t_atempl, fs, fa);

    int episod = 0;
    int score = 0;
    do
    {
        episod++;
        int step = mcar_qltable_teacher_run(&prob, &teacher);
	score += step;
	prob.init();
	teacher.clear_history(fs, fa);
	
	LOG_DEBUG("MOY : " << (float)score/episod << "\tstep : " << step );
    }
    while(episod < 20000);

    LOG_DEBUG("FINAL SCORE : " << (float)score/episod);
}




