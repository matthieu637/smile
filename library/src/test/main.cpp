#include <test/DraftTest.hpp>
#include "bib/Logger.hpp"
#include "simu/Teacher.hpp"
#include <simu/GridWorld.hpp>

// FNV-1a constants
static constexpr unsigned long long basis = 14695981039346656037ULL;
static constexpr unsigned long long prime = 1099511628211ULL;

// compile-time hash helper function
constexpr unsigned long long hash_one(char c, const char* remain, unsigned long long value)
{
    return c == 0 ? value : hash_one(remain[0], remain + 1, (value ^ c) * prime);
}

// compile-time hash
constexpr unsigned long long hash_(const char* str)
{
    return hash_one(str[0], str + 1, basis);
}

// run-time hash
unsigned long long hash_rt(const char* str) {
    unsigned long long hash = basis;
    while (*str != 0) {
        hash ^= str[0];
        hash *= prime;
        ++str;
    }
    return hash;
}

constexpr long long operator"" _hash( const char* str, size_t n ) {
    return hash_( str );
}

Algo parseAlgo(char*c, int start) {
    if(c[start]=='Q' && c[start+1]=='1') {
        return simu::QL;
    } else if(c[start]=='Q' && c[start+1]=='2') {
        return simu::QL;
    } else if(c[start]=='Q' && c[start+1]=='3') {
	return simu::QL_gen;
    } else if(c[start]=='S' && c[start+1]=='1') {
        return simu::Sarsa_;
    } else if(c[start]=='S' && c[start+1]=='2') {
        return simu::Sarsa_trace;
    } else if(c[start]=='S' && c[start+1]=='3') {
	return simu::Sarsa_gen;
    }
    return simu::QL;
}

AdviseStrategy parseAdviseStrat(char*c, int start) {
    if(c[start]=='B') {
        return AdviseStrategy::before;
    } else if(c[start]=='A') {
        return AdviseStrategy::after;
    }
    return AdviseStrategy::after;
}

StrategyEffectsAdvice parseStrategyEffectsAdvice(char* c, int start) {
    if(c[start]=='O') {
        return StrategyEffectsAdvice::FixedNExploration;
    } else if(c[start]=='M') {
        return StrategyEffectsAdvice::Max;
    } else if(c[start]=='I') {
        return StrategyEffectsAdvice::FixedNMax;
    } else if(c[start]=='N') {
        return StrategyEffectsAdvice::None;
    }
    return StrategyEffectsAdvice::None;
}

int main(int argc, char* argv[])
{
    DraftTest m;

    bool handled = true;
    if(argc > 1 ) {
        float cost = 5;
        int numberRun = 1000;

        if(argc > 2)
            cost = atof(argv[2]);
        if(argc > 3)
            numberRun = atoi(argv[3]);

        Environnement<GridWorldState>* grid = new GridWorld();
        Environnement<GridWorldLSState>* gridls = new GridWorldLS();
        Environnement<MCarState>* car = new MCar(8, 12);

        Algo learnAlgo = QL, teachAlgo = QL;
        StrategyEffectsAdvice sea = None;
        bool same_state_rpr = true;
        AdviseStrategy as = after;

        if(argv[1][0] == 'F') {
            learnAlgo = parseAlgo(argv[1], 2);
        } else if(argv[1][0] == 'T') {
            learnAlgo = parseAlgo(argv[1], 2);
            teachAlgo = parseAlgo(argv[1], 4);
            sea = parseStrategyEffectsAdvice(argv[1], 7);
            same_state_rpr = argv[1][8] == 'T';
            as = parseAdviseStrat(argv[1], 9);
        }
        else handled = false;

        if(argv[1][0] == 'F') {
            if (argv[1][1] == 'M') {
                m.F_run_simple<MCarState>(learnAlgo, car, MCarParam, numberRun);
            } else if(argv[1][1] == 'G') {
                m.F_run_simple<GridWorldState>(learnAlgo, grid, GridWorldParam, numberRun);
            } else if(argv[1][1] == 'L') {
                m.F_run_simple<GridWorldLSState>(learnAlgo, gridls, GridWorldParam, numberRun);
            }
            else handled = false;
        }
        else if(argv[1][0] == 'T') {
            if (argv[1][1] == 'M') {
                if(argv[1][6]== 'A') {
                    m.T_run_simple<MCarState, FavorAdvice>(learnAlgo, teachAlgo, car, MCarParam, DefaultParam, same_state_rpr, as, sea, cost, numberRun);
                } else if(argv[1][6]== 'C') {
                    m.T_run_simple<MCarState, CostlyAdvise>(learnAlgo, teachAlgo, car, MCarParam, DefaultParam, same_state_rpr, as, sea, cost, numberRun);
                } else if(argv[1][6]== 'W') {
                    m.T_run_simple<MCarState, LearnerAdvise>(learnAlgo, teachAlgo, car, MCarParam, DefaultParam, same_state_rpr, as, sea, cost, numberRun);
                } else if(argv[1][6]== 'M') {
                    m.T_run_simple<MCarState, CostlyLearnerAdvise>(learnAlgo, teachAlgo, car, MCarParam, DefaultParam, same_state_rpr, as, sea, cost, numberRun);
                }
                else handled = false;
            }
            else if(argv[1][1] == 'G') {
                if(argv[1][6]== 'A') {
                    m.T_run_simple<GridWorldState, FavorAdvice>(learnAlgo, teachAlgo, grid, GridWorldParam, DefaultParam, same_state_rpr, as, sea, cost, numberRun);
                } else if(argv[1][6]== 'C') {
                    m.T_run_simple<GridWorldState, CostlyAdvise>(learnAlgo, teachAlgo, grid, GridWorldParam, DefaultParam, same_state_rpr, as, sea, cost, numberRun);
                } else if(argv[1][6]== 'W') {
                    m.T_run_simple<GridWorldState, LearnerAdvise>(learnAlgo, teachAlgo, grid, GridWorldParam, DefaultParam, same_state_rpr, as, sea, cost, numberRun);
                } else if(argv[1][6]== 'M') {
                    m.T_run_simple<GridWorldState, CostlyLearnerAdvise>(learnAlgo, teachAlgo, grid, GridWorldParam, DefaultParam, same_state_rpr, as, sea, cost, numberRun);
                }
                else handled = false;
            }
            else if(argv[1][1] == 'L') {
                if (argv[1][6]== 'A') {
                    m.T_run_simple<GridWorldLSState, FavorAdvice>(learnAlgo, teachAlgo, gridls, GridWorldLSParam, DefaultParam, same_state_rpr, as, sea, cost, numberRun);
                } else if(argv[1][6]== 'C') {
                    m.T_run_simple<GridWorldLSState, CostlyAdvise>(learnAlgo, teachAlgo, gridls, GridWorldLSParam, DefaultParam, same_state_rpr, as, sea, cost, numberRun);
                } else if(argv[1][6]== 'W') {
                    m.T_run_simple<GridWorldLSState, LearnerAdvise>(learnAlgo, teachAlgo, gridls, GridWorldLSParam, DefaultParam, same_state_rpr, as, sea, cost, numberRun);
                } else if(argv[1][6]== 'M') {
                    m.T_run_simple<GridWorldLSState, CostlyLearnerAdvise>(learnAlgo, teachAlgo, gridls, GridWorldLSParam, DefaultParam, same_state_rpr, as, sea, cost, numberRun);
                }
                else handled = false;
            }
            else handled = false;
        }
        else
            handled = false;
    }
    else
        handled = false;


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

// 	m.F_run_simple<MCarState>(simu::QL_gen, new MCar(1,1), MCarParam, 1000);
	m.T_run_simple<MCarState, CostlyAdvise>(simu::QL_gen, simu::QL, new MCar(8,8), MCarParam, DefaultParam, true, simu::before, None, 5, 1);
	

	
// 	m.F_run_simple<MCarState>(simu::QL, new MCar(8,8), MCarParam, 1000);
//         m.T_run_simple<GridWorldLSState, FavorAdvice>(simu::Sarsa_, simu::QL, new GridWorldLS(), GridWorldLSParam, DefaultParam, true, after, sml::Max, 5, 1000);
// 	m.F_run_simple<GridWorldLSState>(simu::QL, new GridWorldLS, GridWorldLSParam, 10000);
    }

    return 0;
}
