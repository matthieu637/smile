#ifndef MCARQLTABLE_HPP
#define MCARQLTABLE_HPP

#include "simu/MCar.hpp"
#include "simu/RLSimulation.hpp"
#include <sml/QLearning.hpp>
#include <sml/QLearningLamb.hpp>
#include <sml/Sarsa.hpp>
#include <sml/QLearnGradient.hpp>
#include "sml/Feature.hpp"
#include "simu/Environnement.hpp"

namespace simu {

enum Algo {
    QL, QL_trace, QL_gen, Sarsa_, Sarsa_trace, Sarsa_gen
};

template <typename EnvState>
class RLTable : public RLSimulation<EnvState, DState, DiscretizeSelection>
{
public:
    RLTable(Algo algo, Environnement<EnvState>* p, RLParam rlp, bool no_learn_knowledge) : 
    RLSimulation<EnvState, DState, DiscretizeSelection>(p, no_learn_knowledge), type(algo),rlp(rlp) {}

    Policy<DState>* createAgent(const DState&, const DAction&) {

        switch(type) {
        case QL:
//             return new QLearning(this->prob->getStates(), this->prob->getActions(), dst, a, rlp);
        case QL_trace:
//             return new QLearningLamb(this->prob->getStates(), this->prob->getActions(), dst, a);
        case Sarsa_:
//             return new Sarsa(this->prob->getStates(), this->prob->getActions(), dst, a);
        case Sarsa_trace:
        default:
            LOG_ERROR("wrong param");
        }

        return nullptr;
    }

    Algo type;
    RLParam rlp;
};

template<typename EnvState>
class RLGradient : public RLSimulation<EnvState, EnvState, ContinuousSelection>
{
public:
    RLGradient(Algo algo, Environnement<EnvState>* p, RLParam rlp, featuredList<EnvState>* features, int nbFeature, bool no_learn_knowledge, StrategyEffectsAdvice sea) : 
    RLSimulation<EnvState, EnvState, ContinuousSelection>(p, no_learn_knowledge), type(algo),rlp(rlp), features(features), sea(sea),nbFeature(nbFeature) {}

    Policy<EnvState>* createAgent(const EnvState& s, const DAction& a) {

        switch(type) {
        case QL_gen:
            return new QLearnGradient<EnvState>(features, nbFeature, this->prob->getActions(), s, a, rlp, sea);
        default:
            LOG_ERROR("wrong param");
        }

        return nullptr;
    }

    Algo type;
    RLParam rlp;
    featuredList<EnvState>* features;
    StrategyEffectsAdvice sea;
    int nbFeature;
};


}

#endif

