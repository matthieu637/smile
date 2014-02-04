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
                  const DAction& a, RLParam param, StrategyEffectsAdvice sea=None) :
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

    LearnReturn _learn(const State& state, double r, bool goal)
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

        if(updateNext)
            this->updateWeights(delta);

        this->decayTraces();

        this->addTraces(state, *ap);

        //take action a, observe reward, and next state
        delete this->lastAction;
        this->lastAction = ap;

        updateNext = true;
        return {ap, gotGreedy};
    }

    void _startEpisode(const State& s, const DAction& a) {
        this->decayTraces();

        this->addTraces(s, a);

        updateNext = true;
    }


    void had_choosed(const State& state, const DAction& ba, double r, bool, bool goal) {
        DAction* a = this->lastAction;

        float delta = r - this->Qa(*a);

        // For all a in A(s')
        this->computeQa(state);

        DAction* ap = new DAction(ba);
        if(!goal)
            delta = delta + this->param.gamma * this->Qa(*ap);

        if(updateNext)
            this->updateWeights(delta);

        this->decayTraces();

        this->addTraces(state, *ap);

        //take action a, observe reward, and next state
        delete this->lastAction;
        this->lastAction = ap;

        updateNext = true;
    }

    void should_do(const State& state, const DAction& ba, double r, bool goal) {
        DAction* a = this->lastAction;
        float delta = r - this->Qa(*a);

        // For all a in A(s')
        this->computeQa(state);


        DAction* ap = new DAction(ba);
        if(!goal)
            delta = delta + this->param.gamma * this->Qa(*ap);

        if(updateNext)
            this->updateWeights(delta);

        this->decayTraces();

        this->addTraces(state, *ap);

        //take action a, observe reward, and next state
        delete this->lastAction;
        this->lastAction = ap;


        // that was the default update of Q(s,a)
        //now take advise in consideration
        if(this->adviceStrat == Max) {
            adviceMax(state, ba);
            updateNext = false;
        }
        else updateNext = true;
    }


    Policy<State>* copyPolicy() {
        return new SarsaGradient(*this);
    }


private:

//     void adviceMax(const State& state, const DAction& ba) {
//         this->computeQa(state);
//         DAction* amax = this->Qa.argmax();
//         list<int>* f_amax = this->extractFeatures(state, *amax);
//         list<int>* f_ba = this->extractFeatures(state, ba);
// 
// // 	LOG_DEBUG(f_amax->size() << " " << f_ba->size());
// 
//         list<int>::iterator it=f_ba->begin();
//         for(list<int>::iterator it2=f_amax->begin(); it2 != f_amax->end(); ++it2) {
//             this->teta[*it] = this->teta[*it] + this->param.alpha/this->param.tiling * 2. * (this->teta[*it] - this->teta[*it2]) ;
//             ++it;
//         }
// 
//         delete amax;
//         delete f_amax;
//         delete f_ba;
// 
//     }


    void adviceMax(const State& state, const DAction& ba) {
        this->computeQa(state);
//         DAction* amax = this->Qa.argmaxGreaterThan();
	DAction* amax = this->Qa.argmax();
        list<int>* f_amax = this->extractFeatures(state, *amax);
        list<int>* f_ba = this->extractFeatures(state, ba);

// 	LOG_DEBUG(f_amax->size() << " " << f_ba->size());

        list<int>::iterator it=f_ba->begin();
        for(list<int>::iterator it2=f_amax->begin(); it2 != f_amax->end(); ++it2) {
            this->teta[*it] = this->teta[*it2];
            ++it;
        }

        delete amax;
        delete f_amax;
        delete f_ba;

    }


private:
    bool updateNext;

};

}

#endif // SARSAGRADIENT_HPP








