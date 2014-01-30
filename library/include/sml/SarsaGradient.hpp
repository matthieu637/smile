#ifndef SARSAGRADIENT_HPP
#define SARSAGRADIENT_HPP

///
///\file SarsaGradient.hpp
///\brief Algorithme par descente de gradient de QLearning avec des fonctions d'approximation
///

#include <boost/numeric/ublas/vector.hpp>
#include <sml/RLGradientDescent.hpp>
#include <simu/MCar.hpp>

namespace sml {


template <class State>
class SarsaGradient : public RLGradientDescent<State>
{
public:

///
///\brief Constructeur
///\param features : les quadrillages
///       nbFeature : le nombre total de rectagle de tous les quadrillages
/// 	  atmp : le modèle d'action
///       initial : l'action initiale
///       conf : la configuration d'apprentissage
    SarsaGradient(featuredList<State>* features, unsigned int nbFeature, const ActionTemplate* atmp, const State& s,
                  const DAction& a, RLParam param, StrategyEffectsAdvice sea) :
        RLGradientDescent<State>(features, nbFeature/*12288*/, atmp, s, a, param, sea)
    {
        this->startEpisode(s, a);
    }

//     SarsaGradient(const SarsaGradient& q) : RLGradientDescent<State>(q)
//     {
//
//     }

    ~SarsaGradient() {

    }



///
///\brief Retourner l'action à faire selon cet algorithme
///\param state : l'état présent
/// 	  r : la récompense
///       lrate : le taux d'apprentissage
///	  epsilon : politique "epsilon-greedy"
///	  lambda : importance de l'historique
///	  discount : importance du prochain état de la récompense
///	  accumulative : si les traces est accumulative ou non

    LearnReturn _learn(const State& state, double r, bool, bool goal)
    {
        DAction* a = this->lastAction;

        float delta = r - this->Qa(*a);

        // For all a in A(s')
        this->computeQa(state);


        DAction* ap = this->Qa.argmax();
        bool gotGreedy = false;
        if( sml::Utils::rand01(this->param.epsilon)) {
            delete ap;
            ap = new DAction(this->atmpl, rand() % this->atmpl->sizeNeeded());
            gotGreedy = true;
        }

        if(!goal)
            delta = delta + this->param.gamma * this->Qa(*ap);

        this->updateWeights(delta);

        this->decayTraces();

        this->addTraces(state, *ap);

        //take action a, observe reward, and next state
        delete this->lastAction;
        this->lastAction = ap;
        return {ap, gotGreedy};
    }

    void _startEpisode(const State& s, const DAction& a) {
        this->decayTraces();

        this->addTraces(s, a);
    }


    void had_choosed(const State& state, const DAction& ba, double r, bool, bool, bool goal) {
        DAction* a = this->lastAction;

        float delta = r - this->Qa(*a);

        // For all a in A(s')
        this->computeQa(state);

        DAction* ap = new DAction(ba);
        if(!goal)
            delta = delta + this->param.gamma * this->Qa(*ap);

        this->updateWeights(delta);

        this->decayTraces();

        this->addTraces(state, *ap);

        //take action a, observe reward, and next state
        delete this->lastAction;
        this->lastAction = ap;
    }

    void should_do(const State& state, const DAction& ba, double r, bool, bool goal) {
        DAction* a = this->lastAction;
        float delta = r - this->Qa(*a);

        // For all a in A(s')
        this->computeQa(state);


        DAction* ap = new DAction(ba);
        if(!goal)
            delta = delta + this->param.gamma * this->Qa(*ap);

	this->updateWeights(delta);

        this->decayTraces();

        this->addTraces(state, *ap);

        //take action a, observe reward, and next state
        delete this->lastAction;
        this->lastAction = ap;
    }


    Policy<State>* copyPolicy() {
        return new SarsaGradient(*this);
    }


private:

    void adviceMax(const State& state, const DAction& ba) {
        this->computeQa(state);
        DAction* amax = this->Qa.argmax();
        list<int>* f_amax = this->extractFeatures(state, *amax);
        list<int>* f_ba = this->extractFeatures(state, ba);

// 	LOG_DEBUG(f_amax->size() << " " << f_ba->size());
// 	bib::Logger::PRINT_ELEMENTS<list<int>>(*f_amax);
// 	bib::Logger::PRINT_ELEMENTS<list<int>>(*f_ba);
        assert(f_amax->size() == f_ba->size());

//         float tot = Qa(*amax) - Qa(ba);
//         tot = tot / f_ba->size();

// 	for(list<int>::iterator it2=f_ba->begin(); it2 != f_ba->end(); ++it2) {
//             teta[*it2] = teta[*it2] + tot + 0.0001;
//         }

//         list<int>::iterator it=f_ba->begin();
//         for(list<int>::iterator it2=f_amax->begin(); it2 != f_amax->end(); ++it2) {
//             teta[*it] = teta[*it2] + 0.0001;
//             ++it;
//         }

        delete amax;
        delete f_amax;
        delete f_ba;

// 	computeQa(state);
// 	LOG_DEBUG(*Qa.argmax() << " " << ba);
// 	assert(*Qa.argmax() == ba);
    }




};

}

#endif // SARSAGRADIENT_HPP








