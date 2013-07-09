#include "test/mcar_qlearn.hpp"
#include <sml/QLearningLamb.hpp>

// CPPUNIT_TEST_SUITE_REGISTRATION( MCarQLearn );

// Standard RL parameters:
#define epsilon 0.01                    // probability of random action
#define alpha 0.5                      // step size parameter
#define lambda 0.95                     // trace-decay parameters
#define gamma 0.8                        // discount-rate parameters

// #define cost 0.0001

#define nbPosStep 8
#define nbVelStep 12

// 3 conditions to work : gamma < 1 | trace non-accumulative | reward depending of step

const StateTemplate MCarQLearn::t_stempl({POS, VEL, MOT},{nbPosStep, nbVelStep, 3});

DState* getTeachState(const DState& sl, const DAction& al){
  DState* ts = new DState(&MCarQLearn::t_stempl, {sl[POS], sl[VEL], al[MOT]});
  return ts;
}

pair<int, int>* mcar_qltable_teacher_run(MCar* prob, QLearningLamb* teacher, float cost) {
    DAction* ac = new DAction(&MCar::ACTION_TEMPLATE, 0);
    DAction* fac = ac;

    QLearning learner(prob->stempl, &MCar::ACTION_TEMPLATE , *ac, prob->getDState() );
    DState* ts;

    int step = 0;
    bool have_advise = false;
    int nb_advise = 0;
    do
    {
        step++;
        prob->step(*ac);
        DState dst = prob->getDState();
        ac = learner.learn(dst, -1, alpha, epsilon, gamma);//here


	ts = getTeachState(dst, *ac);
	DAction* tac = teacher->learn(*ts, have_advise?-10*(1 + cost):-10, alpha, epsilon, gamma, lambda, false);

	int aa = tac->get(MOT);
	if( aa != 3){
	  learner.should_done(dst, DAction(&MCar::ACTION_TEMPLATE, {aa}), 1, alpha);
	  have_advise = true;
	  nb_advise++;
	} else have_advise = false;
	
        delete ts;
    }
    while(!prob->goal_p() && step < 10000);
    
     ts = getTeachState(prob->getDState(), *ac);
     teacher->learn(*ts, (3000-step), alpha, 0, 0, 1, false);
     delete ts;
     
     delete fac;

//      LOG_DEBUG("DONE WITH " << step << "\tadvice : " << nb_advise );

    return new pair<int, int> (step, nb_advise);
}



void MCarQLearn::mcar_qltable_teacher(float cost) {
//     srand(0);
    srand(time(NULL));
  
    MCar prob(nbPosStep, nbVelStep);
    ActionTemplate t_atempl({MOT}, {4});
    
    const DState& tmp = prob.getDState();
    DState fs(&MCarQLearn::t_stempl, {tmp[POS], tmp[VEL], 2});
    DAction fa(&t_atempl, 3);
    QLearningLamb teacher(&MCarQLearn::t_stempl, &t_atempl, fs, fa);

    int episod = 0;
    int score = 0;
    float advicePerStep = 0L;
    do
    {
        episod++;
        pair<int, int> stat = *mcar_qltable_teacher_run(&prob, &teacher, cost);
	int step = stat.first;
	score += step;
	advicePerStep = (float)stat.first/stat.second;
	prob.init();
	teacher.clear_history(fs, fa);
	
// 	LOG_DEBUG("MOY : " << (float)score/episod << "\tstep : " << step );
    }
    while(episod < 5000);

    LOG((float)score/episod << " " << cost << " " << advicePerStep);
}

