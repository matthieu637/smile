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
            return new QLearning(this->prob->getStates(), this->prob->getActions(), dst, a, rlp);
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
    RLGradient(Algo algo, Environnement<EnvState>* p, RLParam rlp, featuredList<EnvState>* features, int nbFeature) : 
    RLSimulation<EnvState, EnvState, ContinuousSelection>(p), type(algo),rlp(rlp), features(features), nbFeature(nbFeature) {}

    Policy<EnvState>* createAgent(const EnvState&, const DAction& a) {

        switch(type) {
        case QL_gen:
            return new QLearnGradient<EnvState>(features, nbFeature, this->prob->getActions(), a, rlp);
        default:
            LOG_ERROR("wrong param");
        }

        return nullptr;
    }

    Algo type;
    RLParam rlp;
    featuredList<EnvState>* features;
    int nbFeature;
};


}

#endif

