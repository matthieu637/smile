#ifndef MCARQLTABLE_HPP
#define MCARQLTABLE_HPP

#include "simu/MCar.hpp"
#include "simu/RLSimulation.hpp"
#include <sml/QLearning.hpp>
#include <sml/QLearningLamb.hpp>
#include <sml/Sarsa.hpp>
#include "simu/Environnement.hpp"

namespace simu {

struct RLParam {
    float epsilon;
    float alpha;
    float lambda;
    float gamma;
    bool accumu;
};

enum Algo {
    QL, QL_trace, Sarsa_, Sarsa_trace
};

static const RLParam MCarParam = {0.05, 0.08, 0.9, 1., false};
static const RLParam GridWorldLSParam= {0.05, 0.08, 0.9, 0.6, false};
static const RLParam GridWorldParam= {0.05, 0.08, 0.9, 0.6, false};
static const RLParam DefaultParam= {0.05, 0.08, 0.9, 0.6, false};

template <typename EnvState>
class RLTable : public RLSimulation<EnvState, DState, DiscretizeSelection>
{
public:
    RLTable(Algo algo, Environnement<EnvState>* p, RLParam rlp) : RLSimulation<EnvState, DState, DiscretizeSelection>(p), type(algo),rlp(rlp) {}

    Policy<DState>* createAgent(const DState& dst, const DAction& a) {

        switch(type) {
        case QL:
            return new QLearning(this->prob->getStates(), this->prob->getActions(), dst, a);
        case QL_trace:
            return new QLearningLamb(this->prob->getStates(), this->prob->getActions(), dst, a);
        case Sarsa_:
            return new Sarsa(this->prob->getStates(), this->prob->getActions(), dst, a);
        case Sarsa_trace:
            break;
        }

        return nullptr;
    }

    DAction* computeNextAction(const DState& dst, double reward) {

        switch(type) {
        case QL:
            return ((QLearning*)this->agent)->learn(dst, reward, rlp.alpha, rlp.epsilon, rlp.gamma);
        case QL_trace:
            return ((QLearningLamb*)this->agent)->learn(dst, reward, rlp.alpha, rlp.epsilon, rlp.gamma, rlp.lambda, rlp.accumu);
        case Sarsa_:
            return ((Sarsa*)this->agent)->learn(dst, reward, rlp.alpha, rlp.epsilon, rlp.gamma);
        case Sarsa_trace:
            break;
        }

        return nullptr;
    }

    Algo type;
    RLParam rlp;
};

}

#endif

