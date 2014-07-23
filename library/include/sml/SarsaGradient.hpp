#ifndef SARSAGRADIENT_HPP
#define SARSAGRADIENT_HPP

///
///\file SarsaGradient.hpp
///\brief Algorithme par descente de gradient de QLearning avec des fonctions d'approximation
///

#include <boost/numeric/ublas/vector.hpp>
#include <sml/RLGradientDescent.hpp>
#include <simu/MCar.hpp>
#include <algorithm>

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
        RLGradientDescent<State>(features, nbFeature/*12288*/, atmp, s, a, param, sea), e2(zero_vector<double>(nbFeature))
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

	this->updateWeights(delta);

        this->decayTraces();

        this->addTraces(state, *ap);

        //take action a, observe reward, and next state
        delete this->lastAction;
        this->lastAction = ap;

        adviceMaxUpdate();
        return {ap, gotGreedy};
    }

    void _startEpisode(const State& s, const DAction& a) {
        this->decayTraces();

        this->addTraces(s, a);

        historique_max.clear();
        //LOG_DEBUG("reset");
    }


    void had_choosed(const State& state, const DAction& ba, double r, bool, bool goal) {
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

        adviceMaxUpdate();
    }

    void should_do(const State& state, const DAction& ba, double r, bool goal) {
        DAction* a = this->lastAction;
        float delta = r - this->Qa(*a);

        // For all a in A(s')
        this->computeQa(state);


        DAction* ap = new DAction(ba);
        if(!goal)
            delta = delta + this->param.gamma * this->Qa(*ap);

	this->updateWeights(delta);

        this->decayTraces();

        this->addTraces(state, *ap);// TODO: MAX STRAT RM THIS?

        //take action a, observe reward, and next state
        delete this->lastAction;
        this->lastAction = ap;


        // that was the default update of Q(s,a)
        //now take advise in consideration
        if(this->adviceStrat == Max) {
            historique_max.push_back(pair<State, DAction *>(state, new DAction(ba)));

//             list<int>* activeIndex = this->extractFeatures(state, ba);
//             for(list<int>::iterator it = activeIndex->begin(); it != activeIndex->end() ; ++it) {
//                 int index = *it;
//                 if(this->param.accumu)
//                     e2[index] += 1.;
//                 else
//                     e2[index] = 1.;
//             }
//             delete activeIndex;


        }

        adviceMaxUpdate();
    }


    Policy<State>* copyPolicy() {
        return new SarsaGradient(*this);
    }


private:

    void adviceMaxUpdate() {

        //SHUFFLE IF NOT WORK LOOK AT INFORMATION Q(a) - Q(a*)
        //rm from history
        //for(int i=0;i< historique_max.size();i++){
        //  historique_max[i] = historique_max[(int)Utils::randin(0, historique_max.size())];
        //}
        random_shuffle(historique_max.begin(), historique_max.end());
	
        for(typename deque<pair<State, DAction* >>::iterator it = historique_max.begin(); it != historique_max.end() ;) {
	   // if(sml::Utils::rand01(0.1f))
	   if(adviceMax(it->first, *it->second))
	     it = historique_max.erase(it);
	   else ++it;
        }
        //LOG_DEBUG(historique_max.size());
        //e2 *= this->param.lambda;
    }

    bool adviceMax(const State& state, const DAction& ba) {
        //LOG_DEBUG("test "<< ba);

        this->computeQa(state);
        DAction* amax = this->Qa.argmax();
        if(!(*amax == ba)) {
            list<int>* f_amax = this->extractFeatures(state, *amax);
            list<int>* f_ba = this->extractFeatures(state, ba);

// 	LOG_DEBUG(f_amax->size() << " " << f_ba->size());

            list<int>::iterator it=f_ba->begin();
            for(list<int>::iterator it2=f_amax->begin(); it2 != f_amax->end(); ++it2) {
                this->teta[*it] = this->teta[*it] - 2. *(this->param.alpha/this->param.tiling) /** this->e[*it]*/ * (this->teta[*it] - this->teta[*it2]) ;
                //this->e[*it] = 0L;
                ++it;
            }

            delete f_amax;
            delete f_ba;
	    delete amax;
	    return false;
        }

        delete amax;

	return true;
    }


//     void adviceMax(const State& state, const DAction& ba) {
//         this->computeQa(state);
// //         DAction* amax = this->Qa.argmaxGreaterThan();
// 	DAction* amax = this->Qa.argmax();
//         list<int>* f_amax = this->extractFeatures(state, *amax);
//         list<int>* f_ba = this->extractFeatures(state, ba);
//
// // 	LOG_DEBUG(f_amax->size() << " " << f_ba->size());
//
//         list<int>::iterator it=f_ba->begin();
//         for(list<int>::iterator it2=f_amax->begin(); it2 != f_amax->end(); ++it2) {
//             this->teta[*it] = this->teta[*it2];
//             ++it;
//         }
//
//         delete amax;
//         delete f_amax;
//         delete f_ba;
//
//     }


private:
    deque<pair<State, DAction* >> historique_max;
    dbvector e2;

};

}

#endif // SARSAGRADIENT_HPP








