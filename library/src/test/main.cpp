#include <test/DraftTest.hpp>
#include "bib/Logger.hpp"
#include "simu/Teacher.hpp"
#include <simu/GridWorld.hpp>

using namespace std;

// Algo parseAlgo(char*c, int start) {
//     if(c[start]=='Q' && c[start+1]=='1') {
//         return simu::QL;
//     } else if(c[start]=='Q' && c[start+1]=='2') {
//         return simu::QL;
//     } else if(c[start]=='Q' && c[start+1]=='3') {
//         return simu::QL_gen;
//     } else if(c[start]=='S' && c[start+1]=='1') {
//         return simu::Sarsa_;
//     } else if(c[start]=='S' && c[start+1]=='2') {
//         return simu::Sarsa_trace;
//     } else if(c[start]=='S' && c[start+1]=='3') {
//         return simu::Sarsa_gen;
//     }
//     return simu::QL_gen;
// }

AdviseStrategy parseAdviseStrat(char*c, int start) {
    if(c[start]=='B') {
        return AdviseStrategy::before;
    } else if(c[start]=='A') {
        return AdviseStrategy::after;
    }
    return AdviseStrategy::after;
}

StrategyEffectsAdvice parseStrategyEffectsAdvice(char* c, int start) {
    if(c[start]=='F')
        return StrategyEffectsAdvice::Fixed;
    else if(c[start]=='M')
        return StrategyEffectsAdvice::Max;
    else if(c[start]=='I')
        return StrategyEffectsAdvice::InformedExploration;
    else if(c[start]=='D')
        return StrategyEffectsAdvice::Decreased;
    else if(c[start]=='N')
        return StrategyEffectsAdvice::None;

    return StrategyEffectsAdvice::None;
}


tb_strategy parseTBStrategyAdvice(char* c, int start) {
    if(c[start]=='E' && c[start+1]=='A')
        return early_advice;
    else if(c[start]=='I' && c[start+1]=='A')
        return importance_advice;
    else if(c[start]=='M' && c[start+1]=='C')
        return mistake_correction;
    else if(c[start]=='P' && c[start+1]=='P')
        return pourcentage;

    return tb_strategy::early_advice;
}

int main(int argc, char* argv[])
{
    DraftTest m;
    Environnement<GridWorldState>* grid = new GridWorld();
    Environnement<GridWorldLSState>* gridls = new GridWorldLS();
    Environnement<MCarState>* car = new MCar(8, 8);

    Algo learnAlgo = simu::QL_gen/*, teachAlgo = QL_gen*/;
    StrategyEffectsAdvice sea = Max;
    bool learn_knowledge = false;
    AdviseStrategy as = simu::before;
    tb_strategy tbs = early_advice;
    float cost = 2;
    int numberRun = 100;
    int numberAdvice = 100;

    bool handled = true;
    if(argc > 1 ) {
        if(argc > 2)
            numberRun = atoi(argv[2]);
        if(argc > 3)
            learn_knowledge = atoi(argv[3]);

        // additional params
        if(argv[1][0] == 'F') {
//             learnAlgo = parseAlgo(argv[1], 2);
        } else if(argv[1][0] == 'T' && argv[1][1] != 'B') {
            as = parseAdviseStrat(argv[1], 2);
            sea = parseStrategyEffectsAdvice(argv[1], 3);

            if(argc > 4)
                cost = atof(argv[4]);

        } else if(argv[1][0] == 'T' && argv[1][1] == 'B') {
            tbs = parseTBStrategyAdvice(argv[1], 3);
	    sea = sml::InformedExploration;

            if(argc > 4)
                numberAdvice = atoi(argv[4]);
	    if(argc > 5)
		sea = parseStrategyEffectsAdvice(argv[5], 0);
        }
        else handled = false;

// 	start case
// 	FM
        if(argv[1][0] == 'F') {
            if (argv[1][1] == 'M') {
                m.F_run_simple<MCarState>(learnAlgo, car, MCarParam, numberRun, learn_knowledge);
            } else if(argv[1][1] == 'G') {
                m.F_run_simple<GridWorldState>(learnAlgo, grid, GridWorldParam, numberRun, learn_knowledge);
            } else if(argv[1][1] == 'L') {
                m.F_run_simple<GridWorldLSState>(learnAlgo, gridls, GridWorldParam, numberRun, learn_knowledge);
            }
            else handled = false;
        }
//      TM
        else if(argv[1][0] == 'T') {
            if (argv[1][1] == 'M')
                m.T_run_simple<MCarState>(car, MCarParam, DefaultParam, as, sea, cost, numberRun, learn_knowledge);
            else if(argv[1][1] == 'G')
                m.T_run_simple<GridWorldState>(grid, GridWorldParam, DefaultParam, as, sea, cost, numberRun, learn_knowledge);
            else if(argv[1][1] == 'L')
                m.T_run_simple<GridWorldLSState>(gridls, GridWorldLSParam, DefaultParam, as, sea, cost, numberRun, learn_knowledge);
            else if(argv[1][1] == 'B') {
                if (argv[1][2] == 'M')
                    m.runTeachingBudget<MCarState>(car, MCarParam, tbs, numberRun, learn_knowledge, numberAdvice, sea);
                else if(argv[1][2] == 'G')
                    m.runTeachingBudget<GridWorldState>(grid, GridWorldParam, tbs, numberRun, learn_knowledge, numberAdvice, sea);
                else if(argv[1][2] == 'L')
                    m.runTeachingBudget<GridWorldLSState>(gridls, GridWorldLSParam, tbs, numberRun, learn_knowledge, numberAdvice, sea);
                else handled = false;
            }
            else handled = false;
        }
        else
            handled = false;
    }
    else
        handled = false;

//     if(argc > 1 ) {
//         if(argv[1]==string("Z1")) {
//             m.T_run_simple<MCarState>(new MCar(1,1), MCarParam, MCarParam, simu::before, sml::Max, 2, 100);
//         } else if(argv[1] == string("TBMC")) {
//             m.runTeachingBudget(new MCar(1,1), MCarParam, mistake_correction, 100);
//         } else if(argv[1]==string("TBEA")) {
//             m.runTeachingBudget(new MCar(1,1), MCarParam, early_advice, 100);
//         } else if(argv[1]==string("TBIA")) {
//             m.runTeachingBudget(new MCar(1,1), MCarParam, importance_advice, 100);
//         } else if(argv[1]==string("FMQ3")) {
//             m.F_run_simple<MCarState>(simu::QL_gen, new MCar(1, 1), MCarParam, 300, false);
// 	} else if(argv[1]==string("FMQ3_N")) {
//             m.F_run_simple<MCarState>(simu::QL_gen, new MCar(1, 1), MCarParam, 300, true);
//         } else handled = false;
//
//     } else handled = false;


    if(!handled) {
        LOG_ERROR("Cannot handle your test");

// 	ActionTemplate* at = new ActionTemplate({"move"}, {4});
// 	GridWorldLS* gg = new GridWorldLS;
// 	LOG(gg->getDState() << " " << gg->reward());
// 	gg->apply(DAction(at, 1));
// 	LOG(gg->getDState() << " " << gg->reward());
// 	gg->apply(DAction(at, 1));
// 	LOG(gg->getDState() << " " << gg->reward());
// 	gg->apply(DAction(at, 2));
// 	LOG(gg->getDState() << " " << gg->reward());
// 	gg->apply(DAction(at, 1));
// 	LOG(gg->getDState() << " " << gg->reward());
// 	gg->apply(DAction(at, 2));
// 	LOG(gg->getDState() << " " << gg->reward());
// 	gg->apply(DAction(at, 2));
// 	LOG(gg->getDState() << " " << gg->reward());

// 	m.F_run_simple<MCarState>(simu::QL_gen, new MCar(8,8), MCarParam, 100);
// 	m.T_run_simple<MCarState, CostlyAdvise>(simu::QL_gen, simu::QL, new MCar(8,8), MCarParam, DefaultParam, true, simu::before, None, 5, 1);


// 	m.T_run_simple<MCarState>(new MCar(1,1), MCarParam, MCarParam, simu::before, sml::Max, 2, 0);

        m.runTeachingBudget(new MCar(1,1), MCarParam, pourcentage, 100, false, 150, Max);
// 	m.runTeachingBudget(new MCar(1,1), MCarParam, pourcentage, 100, false, 150, InformedExploration);
	
// 	m.F_run_simple<MCarState>(simu::QL_gen, new MCar(1, 1), MCarParam, 300, false);


//         m.F_run_simple<GridWorldState>(simu::QL_gen, grid, GridWorldParam, 1000, false);
//         m.runTeachingBudget<GridWorldState>(grid, GridWorldParam, mistake_correction, 200, learn_knowledge, 150, sea);

// 	m.F_run_simple<MCarState>(simu::QL, new MCar(8,8), MCarParam, 1000);
//         m.T_run_simple<GridWorldLSState, FavorAdvice>(simu::Sarsa_, simu::QL, new GridWorldLS(), GridWorldLSParam, DefaultParam, true, after, sml::Max, 5, 1000);
// 	m.F_run_simple<GridWorldLSState>(simu::QL, new GridWorldLS, GridWorldLSParam, 10000);
    }


    bib::Logger::endInstance();
    return 0;
}
