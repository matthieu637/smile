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
    typedef std::pair< Feature<S>, unsigned int > sfeaturedList;
    typedef std::list< sfeaturedList > featuredList;
    typedef typename featuredList::iterator fLiterator;

public:
    QLearnGradient(featuredList* features, unsigned int nbFeature, const ActionTemplate* atmp, const DAction& initial) :
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
        for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it){
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
            for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it){
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

    list<int>* extractFeatures(const S& state, const DAction& ac) {
        list<int>* actived = new list<int>;

        int layer = 0;
        for(fLiterator flist = features->begin() ; flist != features->end(); ++flist) { // each tiling
            sfeaturedList sl = *flist;
            Feature<S> f = sl.first;
            int size = sl.second;
            int active = f.calc(state, ac);
// 	    LOG_DEBUG(active);
            if(active != -1)
                actived->push_back(layer + active);
            layer += size;
        }

        return actived;
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
        named_mutex mutex( open_or_create, chemin.c_str());
        mutex.unlock();
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



