#ifndef QLEARNGRADIENT_HPP
#define QLEARNGRADIENT_HPP

///
///\file QLearnGradient.hpp
///\brief Algorithme par descente de gradient de QLearning avec des fonctions d'approximation
///

#include <boost/numeric/ublas/vector.hpp>
#include <sml/RLGradientDescent.hpp>
#include <simu/MCar.hpp>

namespace sml {


template <class State>
class QLearnGradient : public RLGradientDescent<State>
{
public:

///
///\brief Constructeur
///\param features : les quadrillages
///       nbFeature : le nombre total de rectagle de tous les quadrillages
/// 	  atmp : le modèle d'action
///       initial : l'action initiale
///       conf : la configuration d'apprentissage
    QLearnGradient(featuredList<State>* features, unsigned int nbFeature, const ActionTemplate* atmp, const State& s,
                   const DAction& a, RLParam param, StrategyEffectsAdvice sea) :
	RLGradientDescent<State>(features, nbFeature, atmp, s, a, param, sea)
    {
        this->startEpisode(s, a);
    }

//     QLearnGradient(const QLearnGradient& q) : LearnStat(q.conf), Policy<State>(q.param, q.adviceStrat), nbFeature(q.nbFeature),
//         teta(q.teta), e(q.e), Qa(q.Qa), actions(q.atmpl->sizeNeeded()), features(q.features), atmpl(q.atmpl),
//         history(q.history)
//     {
//         for(unsigned int i=0; i < atmpl->sizeNeeded() ; i++)
//             actions[i] = new DAction(atmpl, i);
//         lastAction = new DAction(*q.lastAction);
//     }

    ~QLearnGradient() {

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
    LearnReturn _learn(const State& state, double r, bool, bool)
    {
        DAction* a;
        a = this->lastAction;

        float delta = r - this->Qa(*a);

        // For all a in A(s')
        this->computeQa(state);


        DAction* ap = this->Qa.argmax();
        delta = delta + this->param.gamma * this->Qa(*ap);
	this->updateWeights(delta);


        LearnReturn lr = this->decision_learn(state, true, *ap);
        delete ap;
        a = lr.ac;

        //take action a, observe reward, and next state
        delete this->lastAction;
        this->lastAction = a;
        return lr;
    }


    void had_choosed(const State& state, const DAction& ba, double r, bool did_greedy, bool, bool) {
        DAction* a;
        a = this->lastAction;

        float delta = r - this->Qa(*a);

        // For all a in A(s')
        this->computeQa(state);


        DAction* ap = this->Qa.argmax();
        delta = delta + this->param.gamma * this->Qa(*ap);
        delete ap;

	this->updateWeights(delta);

        a = this->decision_learn(state, false, ba, did_greedy ? forced_clear : forced_no_clear).ac;

        //take action a, observe reward, and next state
        delete this->lastAction;
        this->lastAction = a;
    }

    void should_do(const State& s, const DAction& ba, double r, bool, bool) {
        DAction* a;
        a = this->lastAction;

        float delta = r - this->Qa(*a);

        // For all a in A(s')
        this->computeQa(s);

        DAction* ap = this->Qa.argmax();
        delta = delta + this->param.gamma * this->Qa(*ap);

	this->updateWeights(delta);
	
        if(this->adviceStrat == InformedExploration) {
            a = this->decision_learn(s, false, ba, *ap==ba ? forced_clear : forced_no_clear).ac;
        } else if(this->adviceStrat == Max) {
            adviceMax(s, ba);
            a = this->decision_learn(s, false, ba).ac;
//             for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it) {
//                 unsigned int index = *it;
//                 e[index] = this->param.lambda * this->param.gamma * e[index];
//             }
//             a = new DAction(atmpl, {rand() % (int)atmpl->sizeNeeded()});
// 	    a = new DAction(ba);
        } else if(this->adviceStrat == None) {
            LOG_ERROR("a none sea call should_do");
        }

        delete ap;

        //take action a, observe reward, and next state
        delete this->lastAction;
        this->lastAction = a;
    }
    
    LearnReturn decision_learn(const State& state, bool want_greedy, const DAction& argmax, custom_dec_learn comp=normal) {
        DAction* a = nullptr;
        bool gotGreedy = false;

        //begin
        if( want_greedy && sml::Utils::rand01(this->param.epsilon)) {
            a = new DAction(this->atmpl, rand() % this->atmpl->sizeNeeded());
            gotGreedy = true;
        } else {
            a = new DAction(argmax);
        }

        if(comp == normal);
//             gotGreedy = !( *a == argmax) ; //comment me to speed up convergence?
        else gotGreedy = comp == forced_clear;

        if(gotGreedy)
            this->resetTraces();
        else
            this->decayTraces();

	this->addTraces(state, *a);

        return {a, gotGreedy};
    }

    void _startEpisode(const State& s, const DAction& a){
      delete decision_learn(s, false, a).ac; 
    }

    Policy<State>* copyPolicy() {
        return new QLearnGradient(*this);
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

#endif // QLEARNGRADIENT_HPP





