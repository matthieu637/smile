#ifndef QLEARNGRADIENT_HPP
#define QLEARNGRADIENT_HPP

#include <boost/numeric/ublas/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <bib/XMLEngine.hpp>
#include "sml/Q.hpp"
#include "sml/Feature.hpp"
#include "sml/Utils.hpp"

using namespace boost::interprocess;

namespace sml {

typedef boost::numeric::ublas::vector<double> dbvector;
typedef boost::numeric::ublas::vector<bool> bbvector;

using boost::numeric::ublas::zero_vector;

template <class S>
class QLearnGradient
{
public:
    typedef std::list< Feature<S> > sfeaturedList;
    typedef std::list< sfeaturedList > featuredList;
private:
    typedef typename featuredList::iterator fLiterator;
    typedef typename sfeaturedList::iterator sfLiterator;

public:
    QLearnGradient(featuredList* features, unsigned int nbFeature, const ActionTemplate* atmp, const DAction& initial) :
        nbFeature(nbFeature),
        nbFeatureActions(nbFeature*atmp->sizeNeeded()),
        teta(nbFeatureActions), e(zero_vector<double>(nbFeatureActions)),
        Qa(atmp), actions(atmp->sizeNeeded()),
        features(features), atmpl(atmp), history()
    {
        for(unsigned int i=0; i<nbFeatureActions; i++)
            teta[i]=-1./5.+ 2.*sml::Utils::rand01()/5.;//TODO:kinda important

        for(unsigned int i=0; i < atmp->sizeNeeded() ; i++)
            actions[i] = DAction(atmpl, i);

	lastAction = &initial;
    }

    const DAction* decision(const S& state, double r, float lrate, float epsilon, float lamda, float discout)
    {
        const DAction* a;
        a = lastAction;

        float delta = r - Qa(*a);

        // For all a in A(s')
	list<int> activeIndex = *computeQa(state);


        DAction ap = *Qa.argmax();
        delta = delta + discout * Qa(ap);
// 	teta = teta + lrate * delta * e;
	for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it)
	  teta(*it) = teta(*it) + lrate * delta * e(*it);


        //begin
        if( sml::Utils::rand01() < epsilon) {
            a = new DAction(atmpl, rand() % atmpl->sizeNeeded());
	    for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it)
	      e(*it) = 0L;
	    history.clear();
        } else {
            a = Qa.argmax();
	    for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it)
	      e(*it) = lamda * discout * e(*it);
        }

        for(list<int>::iterator it = activeIndex.begin(); it != activeIndex.end() ; ++it){
            e[(*it) + a->hash() * nbFeature] += 1.;
	    history.insert((*it) + a->hash() * nbFeature);
	}

        //take action a, observe reward, and next state

        lastAction = a;
        return a;
    }

    list<int>* computeQa(const S& state) {
        int featurePerLayer = nbFeature / features->size();
        int i = 0;
        int layer = 0;
        list<int>* activeIndex = new list<int>;
        for( fLiterator flist = features->begin(); flist != features->end(); ++flist) { // each feature
            for( sfLiterator f = (*flist).begin(); f != (*flist).end() ; ++f) {
                double active = (*f).calc(state);
                if(active==1.) { //save computations ie 1 feature only active per layer(tiling)
                    activeIndex->push_back(i);
                    i = (layer+1)*featurePerLayer;//jump to the next layer
                    break;
                }
                i++;
            }
            layer++;
        }
        // bib::Logger::PRINT_ELEMENTS<list<int>>(activeIndex);

        i = 0;
        for(vector<DAction>::iterator ai = actions.begin(); ai != actions.end() ; ++ai) { // each actions
            double _Qa = 0.;

            for(list<int>::iterator it = activeIndex->begin(); it != activeIndex->end() ; ++it)
                _Qa += teta[(*it) + i * nbFeature ];

            Qa(*ai) = _Qa;
            i++;
        }
        
        return activeIndex;
    }

    void read(const string& chemin) {
        named_mutex mutex( open_or_create, chemin.c_str());
        mutex.lock();

        if(  !boost::filesystem::exists( chemin ) ) {
            LOG_DEBUG(chemin << " n'existe pas.");
        }
        else {
            teta = *bib::XMLEngine::load< dbvector >("teta", chemin);
        }
    }

    void write(const string& chemin) {
        bib::XMLEngine::save< dbvector >(teta, "teta", chemin);
// 	bib::Logger::PRINT_ELEMENTS<dbvector>(teta);
        named_mutex mutex( open_or_create, chemin.c_str());
        mutex.unlock();
    }


private:
    int nbFeature;
    int nbFeatureActions;

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



