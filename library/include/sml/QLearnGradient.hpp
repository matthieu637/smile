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
    QLearnGradient(featuredList<State>* features, unsigned int nbFeature, const ActionTemplate* atmp,
                   const DAction& initial, RLParam param, const LearnConfig& conf= {false,0,0}) :
        LearnStat(conf), Policy<State>(param),
        nbFeature(nbFeature),
        teta(nbFeature), e(zero_vector<double>(nbFeature)),
        Qa(atmp), actions(atmp->sizeNeeded()),
        features(features), atmpl(atmp), history()
    {
        for(unsigned int i=0; i<nbFeature; i++)
            teta[i]=-1./5.+ 2.*sml::Utils::rand01()/5.;//TODO:kinda important

        for(unsigned int i=0; i < atmp->sizeNeeded() ; i++)
            actions[i] = DAction(atmpl, i);

        lastAction = new DAction(initial);
    }

    QLearnGradient(const QLearnGradient& q) : LearnStat(q.conf), nbFeature(q.nbFeature),
        teta(q.teta), e(q.e), Qa(q.Qa), actions(q.actions), features(q.features), atmpl(q.atmpl),
        history(q.history), Policy<State>(q.param)
    {
        lastAction = new DAction(*q.lastAction);
    }

///
///\brief Retourner l'action à faire selon l'algorithme sans apprentissage
///\param state : l'état présent
    DAction* decision(const State& state, float epsilon=0.L) {
        if(sml::Utils::rand01() < epsilon )
            return new DAction(atmpl, {rand() % (int)atmpl->sizeNeeded()});

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
    DAction* learn(const State& state, double r)
    {
        DAction* a;
        a = lastAction;

        float delta = r - Qa(*a);

        // For all a in A(s')
        computeQa(state);


        DAction* ap = Qa.argmax();
        delta = delta + this->param.gamma * Qa(*ap);

// 	LOG_DEBUG("history size:" << history.size());
// 	teta = teta + lrate * delta * e;
        for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it) {
            int index = *it;
            teta[index] = teta[index] + this->param.alpha * delta * e[index];
        }

        //begin
        if( sml::Utils::rand01() < this->param.epsilon) {
            a = new DAction(atmpl, rand() % atmpl->sizeNeeded());
            for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it)
                e[*it] = 0L;
            history.clear();
        } else {
//             a = Qa.argmax();
            a = ap;
            for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it) {
                unsigned int index = *it;
                e[index] = this->param.lambda * this->param.gamma * e[index];
// 		LOG_DEBUG(e(*it));
            }
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

        //take action a, observe reward, and next state
        delete lastAction;
        lastAction = a;
        return a;
    }

    void clear_history(const State& s, const DAction& a) {
        delete lastAction;
        lastAction = new DAction(a);

        for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it)
            e[*it] = 0L;
        history.clear();
    }

    void should_done(const State& s, const DAction& a) {

    }

    void should_do(const State& s, const DAction& a, double reward) {

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

///
///\brief Calculer la somme de feature pour chaque action disponible
///\param state : l'état présent
    void computeQa(const State& state) {

        for(vector<DAction>::iterator ai = actions.begin(); ai != actions.end() ; ++ai) { // each actions
            double _Qa = 0.;

            list<int>* actived = extractFeatures(state, *ai);

//   	    bib::Logger::PRINT_ELEMENTS<list<int>>(*actived);

            for(list<int>::iterator it=actived->begin(); it != actived->end(); ++it)
                _Qa += teta[*it];

// 	    LOG_DEBUG("old :" << Qa(*ai) << " new :" << _Qa);
            Qa(*ai) = _Qa;
            delete actived;
        }
    }


///
///\brief Retourner les indices des rectangles activés par cet état et action
///\param st : l'état présent
///	  ac : l'action
    list<int>* extractFeatures(const State& state, const DAction& ac) {
        list<int>* actived = new list<int>;

        int layer = 0;
        for(fLiterator<State> flist = features->begin() ; flist != features->end(); ++flist) { // each tiling
            Feature<State> f = *flist;
            int size = f.getSize();
            int active = f.calc(state, ac);
// 	    LOG_DEBUG(active);
            if(active != -1)
                actived->push_back(layer + active);
            layer += size;
        }

        return actived;
    }



private:
    int nbFeature;

    dbvector teta;
    dbvector e;
    QTable Qa;

    DAction* lastAction;

    std::vector<DAction> actions;

    featuredList<State>* features;
    const ActionTemplate* atmpl;
    std::set<unsigned int> history; //set : no duplicates
};

}

#endif // QLEARNGRADIENT_HPP



