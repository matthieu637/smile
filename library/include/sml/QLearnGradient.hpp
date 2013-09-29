#ifndef QLEARNGRADIENT_HPP
#define QLEARNGRADIENT_HPP

///
///\file QLearnGradient.hpp
///\brief Algorithme par descente de gradient de QLearning avec des fonctions d'approximation
///


#include <boost/numeric/ublas/vector.hpp>
#include <bib/XMLEngine.hpp>
#include "sml/Q.hpp"
#include "sml/Feature.hpp"
#include "sml/Utils.hpp"
#include "LearnStat.hpp"
#include "Policy.hpp"
#include <simu/MCar.hpp>

namespace sml {

typedef boost::numeric::ublas::vector<double> dbvector;
typedef boost::numeric::ublas::vector<bool> bbvector;

using boost::numeric::ublas::zero_vector;

template <class State>
class QLearnGradient : public LearnStat, public Policy<State>
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
                   const DAction& initial, RLParam param, StrategyEffectsAdvice sea, const LearnConfig& conf= {false,0,0}) :
        LearnStat(conf), Policy<State>(param, sea),
        nbFeature(nbFeature),
        teta(nbFeature), e(zero_vector<double>(nbFeature)),
        Qa(atmp), actions(atmp->sizeNeeded()),
        features(features), atmpl(atmp), history()
    {
        for(unsigned int i=0; i<nbFeature; i++)
            teta[i] =  0L ;

        for(unsigned int i=0; i < atmp->sizeNeeded() ; i++) {
            actions[i] = new DAction(atmpl, i);
// 	    LOG_DEBUG(actions[i]->get("motor") << " " << atmpl->indexFor("motor") << " " << atmpl->actionNumber());
        }

        lastAction = new DAction(initial);
        clear_history(s, initial);
    }

    QLearnGradient(const QLearnGradient& q) : LearnStat(q.conf), Policy<State>(q.param, q.adviceStrat), nbFeature(q.nbFeature),
        teta(q.teta), e(q.e), Qa(q.Qa), actions(q.atmpl->sizeNeeded()), features(q.features), atmpl(q.atmpl),
        history(q.history)
    {
        for(unsigned int i=0; i < atmpl->sizeNeeded() ; i++)
            actions[i] = new DAction(atmpl, i);
        lastAction = new DAction(*q.lastAction);
    }

    ~QLearnGradient() {
        for(vector<DAction*>::iterator ai = actions.begin(); ai != actions.end() ; ++ai)
            delete *ai;
        delete lastAction;
    }

///
///\brief Retourner l'action à faire selon l'algorithme sans apprentissage
///\param state : l'état présent
    DAction* decision(const State& state, bool greedy) {
        if(greedy && sml::Utils::rand01(this->param.epsilon)  ) {
// 	    LOG_DEBUG("got greeding");
            return new DAction(atmpl, {rand() % (int)atmpl->sizeNeeded()});
        }

        computeQa(state);
        return Qa.argmax();
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
    LearnReturn _learn(const State& state, double r)
    {
// 	this->param.alpha = this->param.alpha * 0.999999999;
        DAction* a;
        a = lastAction;

        float delta = r - Qa(*a);

        // For all a in A(s')
        computeQa(state);


        DAction* ap = Qa.argmax();
        delta = delta + this->param.gamma * Qa(*ap);

// 	teta = teta + this->param.alpha * delta * e;
        for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it) {
            int index = *it;
            teta[index] = teta[index] + ( this->param.alpha/this->param.tiling ) * delta * e[index];
        }

        LearnReturn lr = decision_learn(state, true, *ap);
        delete ap;
        a = lr.ac;

        //take action a, observe reward, and next state
        delete lastAction;
        lastAction = a;
        return lr;
    }

    void clear_history(const State& s, const DAction& a) {
        delete lastAction;
        lastAction = new DAction(a);

        // 	e *= 0.;
        for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it)
            e[*it] = 0L;
        history.clear();


        //start an new episod
        computeQa(s);
        delete decision_learn(s, false, a).ac;
    }

    void should_done(const State&, const DAction&) {

    }

    void had_choosed(const State& state, const DAction& ba, double r, bool did_greedy) {
        DAction* a;
        a = lastAction;

        float delta = r - Qa(*a);

        // For all a in A(s')
        computeQa(state);


        DAction* ap = Qa.argmax();
        delta = delta + this->param.gamma * Qa(*ap);
	delete ap;

        for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it) {
            int index = *it;
            teta[index] = teta[index] + ( this->param.alpha/this->param.tiling ) * delta * e[index];
        }
        
        a = decision_learn(state, false, ba, did_greedy ? forced_clear : forced_no_clear).ac;

        //take action a, observe reward, and next state
        delete lastAction;
        lastAction = a;
    }

    void should_do(const State& s, const DAction& ba, double r) {
        DAction* a;
        a = lastAction;

        float delta = r - Qa(*a);

        // For all a in A(s')
        computeQa(s);

        DAction* ap = Qa.argmax();
        delta = delta + this->param.gamma * Qa(*ap);
        

// 	teta = teta + lrate * delta * e;
        for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it) {
            int index = *it;
            teta[index] = teta[index] + ( this->param.alpha/this->param.tiling ) * delta * e[index];
        }

        if(this->adviceStrat == InformedExploration) {
            a = decision_learn(s, false, ba, *ap==ba ? forced_clear : forced_no_clear).ac;
        } else if(this->adviceStrat == Max) {
            adviceMax(s, ba);
            a = decision_learn(s, false, ba).ac;
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
        delete lastAction;
        lastAction = a;
    }

    float getStateImportance(const State& state) {
        computeQa(state);

        DAction* amax = Qa.argmax();
        DAction* amin = Qa.argmin();

        float importance = Qa(*amax) - Qa(*amin);
// 	LOG_DEBUG(Qa(*amax) << " " << Qa(*amin) << " " << importance);
        delete amax;
        delete amin;

        return importance;
    }

    Policy<State>* copyPolicy() {
        return new QLearnGradient(*this);
    }


///
///\brief Mettre à jour ce que l'algorithme a appris
///\param xml : le fichier XML
    void update(const DAction& ac) {
        lastAction = &ac;
    }

///
///\brief Sauvegarder ce que l'algorithme a appris
///\param xml : le fichier XML
    void save(boost::archive::xml_oarchive* xml)
    {
        *xml << make_nvp("teta", teta);
    }

///
///\brief Charger ce que l'algorithme a appris
///\param xml : le fichier XML
    void load(boost::archive::xml_iarchive* xml) {
        *xml >> make_nvp("teta", teta);
    }


private:
    enum custom_dl {normal, forced_clear, forced_no_clear};

    LearnReturn decision_learn(const State& state, bool want_greedy, const DAction& argmax, custom_dl comp=normal) {
        DAction* a = nullptr;
        bool gotGreedy = false;

        //begin
        if( want_greedy && sml::Utils::rand01(this->param.epsilon)) {
            a = new DAction(atmpl, rand() % atmpl->sizeNeeded());
            gotGreedy = true;
        } else {
            a = new DAction(argmax);
        }

        if(comp == normal)
            gotGreedy = !( *a == argmax) ; //comment me to speed up convergence?
        else gotGreedy = comp == forced_clear;

        if(gotGreedy) {
            for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it)
                e[*it] = 0L;
            history.clear();
// 	   e *= 0.;
        } else {
            for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it) {
                unsigned int index = *it;
                e[index] = this->param.lambda * this->param.gamma * e[index];
            }
//            e *= this->param.lambda * this->param.gamma;
        }

        list<int>* activeIndex = extractFeatures(state, *a);
        for(list<int>::iterator it = activeIndex->begin(); it != activeIndex->end() ; ++it) {
            int index = *it;
            if(this->param.accumu)
                e[index] += 1.;
            else
                e[index] = 1.;
            history.insert(index);
        }
        delete activeIndex;

        return {a, gotGreedy};
    }

    void adviceMax(const State& state, const DAction& ba) {
        computeQa(state);
        DAction* amax = Qa.argmax();
        list<int>* f_amax = extractFeatures(state, *amax);
        list<int>* f_ba = extractFeatures(state, ba);

// 	LOG_DEBUG(f_amax->size() << " " << f_ba->size());
// 	bib::Logger::PRINT_ELEMENTS<list<int>>(*f_amax);
// 	bib::Logger::PRINT_ELEMENTS<list<int>>(*f_ba);
        assert(f_amax->size() == f_ba->size());

//         float tot = Qa(*amax) - Qa(ba);
//         tot = tot / f_ba->size();

// 	for(list<int>::iterator it2=f_ba->begin(); it2 != f_ba->end(); ++it2) {
//             teta[*it2] = teta[*it2] + tot + 0.0001;
//         }

        list<int>::iterator it=f_ba->begin();
        for(list<int>::iterator it2=f_amax->begin(); it2 != f_amax->end(); ++it2) {
            teta[*it] = teta[*it2] + 0.0001;
            ++it;
        }

        delete amax;
        delete f_amax;
        delete f_ba;

// 	computeQa(state);
// 	LOG_DEBUG(*Qa.argmax() << " " << ba);
// 	assert(*Qa.argmax() == ba);
    }

///
///\brief Calculer la somme de feature pour chaque action disponible
///\param state : l'état présent
    void computeQa(const State& state) {

//         LOG_DEBUG(mcs.position<< " " << mcs.velocity);
        for(vector<DAction*>::iterator ai = actions.begin(); ai != actions.end() ; ++ai) { // each actions
            double _Qa = this->param.initial;

            list<int>* actived = extractFeatures(state, **ai);
// 	    LOG_DEBUG((*ai)->get("motor"));
//             bib::Logger::PRINT_ELEMENTS<list<int>>(*actived);

            for(list<int>::iterator it=actived->begin(); it != actived->end(); ++it)
                _Qa += teta[*it];

// // 	    LOG_DEBUG("old :" << Qa(**ai) << " new :" << _Qa);
            Qa(**ai) = _Qa;
            delete actived;
        }
//         LOG_DEBUG("#########");
    }


///
///\brief Retourner les indices des rectangles activés par cet état et action
///\param st : l'état présent
///	  ac : l'action
    list<int>* extractFeatures(const State& state, const DAction& ac) {
        list<int>* actived = new list<int>;

        int layer = 0;
        for(fLiterator<State> flist = features->begin() ; flist != features->end(); ++flist) { // each tiling
            Feature<State> *f = *flist;
            int size = f->getSize();
            int active = f->calc(state, ac);
// 	    LOG_DEBUG(active);
            if(active != -1)
                actived->push_back(layer + active);
            layer += size;
        }
//         LOG_DEBUG("##");

        return actived;
    }

// #include <simu/MCar.hpp>
// 
//     list<int>* extractFeatures(const State& st, const DAction& ac) {
// 	int a = ac["motor"];
// 	simu::MCarState& state = (simu::MCarState&)st;
//         list<int>* actived = new list<int>;
// 
//         float state_vars[2];
// 	int F[8];
//         state_vars[0] = state.position / (1.7 / 16);
//         state_vars[1] = state.velocity / (0.14 / 16);
// 	
// 	GetTiles(&F[0], 8, state_vars, 2, 7776, a);
// 	
// 	for(int i=0;i<8;i++)
// 	  actived->push_back( F[i] );
// 
// // 	bib::Logger::PRINT_ELEMENTS<list<int>>(*actived);
//         return actived;
//     }


private:
    int nbFeature;

    dbvector teta;
    dbvector e;
    QTable Qa;

    DAction* lastAction;

    std::vector<DAction*> actions;

    featuredList<State>* features;
    const ActionTemplate* atmpl;
    std::set<unsigned int> history; //set : no duplicates
};

}

#endif // QLEARNGRADIENT_HPP





