#include "test/mcar_qlearn.hpp"
#include <sml/QLearningLamb.hpp>

// CPPUNIT_TEST_SUITE_REGISTRATION( MCarQLearn );

// Standard RL parameters:
#define epsilon 0.01                    // probability of random action
#define alpha 0.5                      // step size parameter
#define lambda 0.95                     // trace-decay parameters
#define gamma 0.8                        // discount-rate parameters

#define nbPosStep 8
#define nbVelStep 12

// 3 conditions to work : gamma < 1 | trace non-accumulative | reward depending of step

// const StateTemplate MCarQLearn::t_stempl({POS, VEL, MOT},{nbPosStep, nbVelStep, 3});

DState* getTeachState_annonce(const DState& sl, const DAction& al) {
    DState* ts = new DState(&MCarQLearn::t_stempl, {sl[POS], sl[VEL], al[MOT]});
    return ts;
}

pair<int, int>*  mcar_qltable_teacher_annonce_run(MCar* prob, QLearningLamb* teacher, float cost) {
    DAction* ac = new DAction(prob->getActions(), 0);
    DAction* fac = ac;

    QLearning learner(prob->getStates(), prob->getActions() , *ac, prob->getDState() );
    DState* ts;

    int step = 0;
    int nb_advise = 0;
    bool del_ac = false;
    bool have_advise = false;
    do
    {
        step++;
        prob->apply(*ac);
        DState dst = prob->getDState();

        if(del_ac)
            delete ac;

        ac = learner.decision(dst);
        del_ac = false;

        ts = getTeachState_annonce(dst, *ac);
        DAction* tac = teacher->learn(*ts, have_advise?-10*(1 + cost):-10, alpha, epsilon, gamma, lambda, false);

        int aa = tac->get(MOT);
        if( aa != 3 && aa != ac->get(MOT)) {
            learner.should_do(dst, DAction(prob->getActions(), {aa}));
            delete ac;
            ac = tac;
            nb_advise++;
            have_advise = true;
        } else {
            del_ac = true;
            have_advise = false;
        }

        delete ts;
    }
    while(!prob->goal() && step < 10000);

    ts = getTeachState_annonce(prob->getDState(), *ac);
    teacher->learn(*ts, 3000-step, alpha, 0, 0, 1, false);
    delete ts;

    delete fac;

    if(del_ac)
        delete ac;

//      LOG_DEBUG("DONE WITH " << step << " advice : " << nb_advise );

    return new pair<int, int> (step, nb_advise);
}



void MCarQLearn::mcar_qltable_teacher_annonce(float cost) {
    srand(time(NULL));
//     srand(0);

    MCar prob(nbPosStep, nbVelStep);
    ActionTemplate t_atempl( {MOT}, {4});

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
        pair<int, int> stat = *mcar_qltable_teacher_annonce_run(&prob, &teacher, cost);
        int step = stat.first;
        advicePerStep = (float)stat.first/stat.second;
        score += step;
        prob.init();
        teacher.clear_history(fs, fa);

// 	LOG_DEBUG("MOY : " << (float)score/episod << "\tstep : " << step );
    }
    while(episod < 5000);

    LOG((float)score/episod << " " << cost << " " << advicePerStep);
}




