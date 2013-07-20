#ifndef MCARQLTABLE_HPP
#define MCARQLTABLE_HPP

#include "simu/MCar.hpp"
#include "simu/RLSimulation.hpp"
#include <sml/QLearning.hpp>
#include <sml/QLearningLamb.hpp>
#include <sml/Sarsa.hpp>
#include "simu/Environnement.hpp"

// Standard RL parameters:
#define epsilon 0.05                    // probability of random action
#define alpha 0.08                      // step size parameter
#define lambda 0.9                     // trace-decay parameters
#define gamma 1.                        // discount-rate parameters
#define accumu false

namespace simu {

enum Algo {
    QL, QL_trace, Sarsa_, Sarsa_trace
};

template <typename EnvState>
class RLTable : public RLSimulation<EnvState, DState, DiscretizeSelection>
{
public:
    RLTable(Algo algo, Environnement<EnvState>* p) : RLSimulation<EnvState, DState, DiscretizeSelection>(p), type(algo) {}

    Policy<DState>* createAgent(const DState& dst, const DAction& a) {

        switch(type) {
        case QL:
            return new QLearning(this->prob->getStates(), this->prob->getActions(), dst, a);
        case QL_trace:
            return new QLearningLamb(this->prob->getStates(), this->prob->getActions(), dst, a);
        case Sarsa_:
//             return new Sarsa(prob->getStates(), prob->getActions(), dst, a);
        case Sarsa_trace:
            break;
        }

        return nullptr;
    }

    DAction* computeNextAction(const DState& dst, double reward) {

        switch(type) {
        case QL:
            return ((QLearning*)this->agent)->learn(dst, reward, alpha, epsilon, gamma);
        case QL_trace:
            return ((QLearningLamb*)this->agent)->learn(dst, reward, alpha, epsilon, gamma, lambda, accumu);
        case Sarsa_:
        case Sarsa_trace:
            break;
        }

        return nullptr;
    }

    Algo type;
};

}

#endif

