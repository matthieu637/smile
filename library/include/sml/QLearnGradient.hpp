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

namespace sml {

typedef boost::numeric::ublas::vector<double> dbvector;
typedef boost::numeric::ublas::vector<bool> bbvector;

using boost::numeric::ublas::zero_vector;

template <class S>
class QLearnGradient : public LearnStat
{
public:
    typedef std::list< Feature<S> > featuredList;
    typedef typename featuredList::iterator fLiterator;

public:

///
///\brief Constructeur
///\param features : les quadrillages
///       nbFeature : le nombre total de rectagle de tous les quadrillages
/// 	  atmp : le modèle d'action
///       initial : l'action initiale
///       conf : la configuration d'apprentissage
    QLearnGradient(featuredList* features, unsigned int nbFeature, const ActionTemplate* atmp, const DAction& initial, const LearnConfig& conf) :
        LearnStat(conf),
        nbFeature(nbFeature),
        teta(nbFeature), e(zero_vector<double>(nbFeature)),
        Qa(atmp), actions(atmp->sizeNeeded()),
        features(features), atmpl(atmp), history()
    {
        for(unsigned int i=0; i<nbFeature; i++)
            teta[i]=-1./5.+ 2.*sml::Utils::rand01()/5.;//TODO:kinda important

        for(unsigned int i=0; i < atmp->sizeNeeded() ; i++)
            actions[i] = DAction(atmpl, i);

        lastAction = &initial;
    }
    
  
///
///\brief Retourner l'action à faire selon l'algorithme sans apprentissage 
///\param state : l'état présent
    const DAction* decision(const S& state) {
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
    const DAction* learn(const S& state, double r, float lrate, float epsilon, float lamda, float discout, bool accumulative)
    {
        const DAction* a;
        a = lastAction;

        float delta = r - Qa(*a);

        // For all a in A(s')
        computeQa(state);


        const DAction* ap = Qa.argmax();
        delta = delta + discout * Qa(*ap);

// 	LOG_DEBUG("history size:" << history.size());
// 	teta = teta + lrate * delta * e;
        for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it) {
            int index = *it;
            teta[index] = teta[index] + lrate * delta * e[index];
        }

        //begin
        if( sml::Utils::rand01() < epsilon) {
            a = new DAction(atmpl, rand() % atmpl->sizeNeeded());
            for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it)
                e[*it] = 0L;
            history.clear();
        } else {
//             a = Qa.argmax();
            a = ap;
            for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it) {
                unsigned int index = *it;
                e[index] = lamda * discout * e[index];
// 		LOG_DEBUG(e(*it));
            }
        }

        list<int> activeIndex = *extractFeatures(state, *a);
        for(list<int>::iterator it = activeIndex.begin(); it != activeIndex.end() ; ++it) {
            int index = *it;
            if(accumulative)
                e[index] += 1.;
            else
                e[index] = 1.;
            history.insert(index);
        }

        //take action a, observe reward, and next state
        lastAction = a;
        return a;
    }

    
///
///\brief apprendre de ce que fait le tuteur
///\param st : l'état présent
///	  ac : l'action ce que le tuteur fait dans cet état
/// 	  r : la récompense
///       lrate : le taux d'apprentissage
///	  lambda : importance de l'historique
///	  discount : importance du prochain état de la récompense 
///	  accumulative : si les traces est accumulative ou non
    void observeTutor(const S& st, const DAction& ac, double r, double lrate, double lambda, double discount, bool accumulative) {
        float delta = r - Qa(*lastAction);

        // For all a in A(s')
        computeQa(st);

        const DAction* ap = Qa.argmax();
        delta = delta + discount * Qa(*ap);

// 	LOG_DEBUG("history size:" << history.size());
// 	teta = teta + lrate * delta * e;
        for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it) {
            int index = *it;
            teta[index] = teta[index] + lrate * delta * e[index];
        }

        //begin
        for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it) {
            unsigned int index = *it;
            e[index] = lambda * discount * e[index];
        }


        list<int> activeIndex = *extractFeatures(st, ac);
        for(list<int>::iterator it = activeIndex.begin(); it != activeIndex.end() ; ++it) {
            int index = *it;
            if(accumulative)
                e[index] += 1.;
            else
                e[index] = 1.;
            history.insert(index);
        }

        //take action a, observe reward, and next state
        lastAction = &ac;
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
    void computeQa(const S& state) {

        for(vector<DAction>::iterator ai = actions.begin(); ai != actions.end() ; ++ai) { // each actions
            double _Qa = 0.;

            list<int>* actived = extractFeatures(state, *ai);

//   	    bib::Logger::PRINT_ELEMENTS<list<int>>(*actived);

            for(list<int>::iterator it=actived->begin(); it != actived->end(); ++it)
                _Qa += teta[*it];

// 	    LOG_DEBUG("old :" << Qa(*ai) << " new :" << _Qa);
            Qa(*ai) = _Qa;
        }
    }

    /*
        list<int>* extractFeatures(const S& state, const DAction& ac) {
            list<int>* actived = new list<int>;

            int i = 0;
            int layer = 0;
            for(fLiterator flist = features->begin() ; flist != features->end(); ++flist) { // each feature
                for( sfLiterator f = (*flist).begin(); f != (*flist).end() ; ++f) {
                    double active = (*f).calc(state, ac);
                    if(active==1.) { //save computations ie 1 feature only active per layer(tiling)
    		    actived->push_back(i);
                        i = layer+(*flist).size();//jump to the next layer
                        break;
                    }
                    i++;
                }
                layer+= (*flist).size();
            }

            actived->push_back(nbFeature);//bias always active

            return actived;
        }*/
    
    
///
///\brief Retourner les indices des rectangles activés par cet état et action
///\param st : l'état présent
///	  ac : l'action 
    list<int>* extractFeatures(const S& state, const DAction& ac) {
        list<int>* actived = new list<int>;

        int layer = 0;
        for(fLiterator flist = features->begin() ; flist != features->end(); ++flist) { // each tiling
            Feature<S> f = *flist;
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
    list<int>* F;
    QTable Qa;

    const DAction* lastAction;

    std::vector<DAction> actions;

    featuredList* features;
    const ActionTemplate* atmpl;
    std::set<unsigned int> history; //set : no duplicates
};

}

#endif // QLEARNGRADIENT_HPP



