#ifndef QLEARNGRADIENT_HPP
#define QLEARNGRADIENT_HPP

#include <boost/numeric/ublas/vector.hpp>
#include <boost/filesystem.hpp>
#include <bib/XMLEngine.hpp>
#include "sml/Q.hpp"
#include "sml/Feature.hpp"
#include "sml/Utils.hpp"

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
    QLearnGradient(featuredList* features, unsigned int nbFeature, const ActionTemplate* atmp) :
        nbFeature(nbFeature), teta(nbFeature), e(zero_vector<double>(nbFeature)),
        Qa(atmp), actions(atmp->sizeNeeded()), fakefeature(nbFeature) ,
        features(features), atmpl(atmp)
    {
        StateTemplate* stmpl = new sml::StateTemplate( {""}, {(int)nbFeature});
        for(unsigned int i=0; i<nbFeature; i++) {
            teta[i]= sml::Utils::rand01()/5.;
            fakefeature[i]= DAction(stmpl , i);
        }

        for(unsigned int i=0; i < atmp->sizeNeeded() ; i++)
            actions[i] = DAction(atmpl, i);

        //Fa = new QTable(atmpl, stmpl);
        lastAction = new DAction(atmp, 0);
    }

    DAction* decision(const S& state, double r, float lrate, float epsilon, float lamda, float discout)
    {
        DAction* a;
        a = lastAction;

        float delta = r - Qa(*a);


        int featurePerLayer = nbFeature / features->size();
        int i = 0;
        int layer = 0;
        list<int> activeIndex;
        for( fLiterator flist = features->begin(); flist != features->end(); ++flist) { // each feature
            for( sfLiterator f = (*flist).begin(); f != (*flist).end() ; ++f) {
                double active = (*f).calc(state);
                // (*Fa)(*ai, fakefeature[i]) = active;
                if(active==1.) { //save computations ie 1 feature only active per layer(tiling)
                    //_Qa += teta[i];
                    activeIndex.push_back(i);
                    i = (layer+1)*featurePerLayer;//jump to the next layer
                    break;
                }
                i++;
            }
            layer++;
        }
        bib::Logger::PRINT_ELEMENTS<list<int>>(activeIndex);

        for(vector<DAction>::iterator ai = actions.begin(); ai != actions.end() ; ++ai) { // each actions
            double _Qa = 0.;

            for(list<int>::iterator it = activeIndex.begin(); it != activeIndex.end() ; ++it)
                _Qa += teta[(*it)*2];

            Qa(*ai) = _Qa;
        }


        DAction ap = *Qa.argmax();
        delta = delta + discout * Qa(ap);
	LOG_DEBUG(teta.size() << " " << e.size());
        teta = teta + lrate * delta * e;


        //begin

        if( sml::Utils::rand01() < epsilon) {
            a = new DAction(atmpl, rand() % atmpl->sizeNeeded());
            e *= 0;
        } else {
            a = Qa.argmax();
            e = lamda * discout * e;
        }

//         for(unsigned int i=0; i<nbFeature; i++) // for all i in Fa : e(i) <- e(i) + 1
//             if( (*Fa)(*a,fakefeature[i]) == 1.)
//                 e[i] = e[i] + 1.;
	
	for(list<int>::iterator it = activeIndex.begin(); it != activeIndex.end() ; ++it)
                e[*it] += 1.;

        //take action a, observe reward, and next state

        lastAction = a;
        return a;
    }

    void read(const string& chemin) {
        if(  !boost::filesystem::exists( chemin ) ) {
            LOG_DEBUG(chemin << " n'existe pas.");
        }
        else {
            teta = *bib::XMLEngine::load< dbvector >("teta", chemin);
        }
    }

    void write(const string& chemin) {
        bib::XMLEngine::save< dbvector >(teta, "teta", chemin);
    }


private:
    int nbFeature;

    dbvector teta;
    dbvector e;
//     QTable* Fa;
    list<int>* F;
    QTable Qa;

    DAction* lastAction;

    std::vector<DAction> actions;
    std::vector<DAction> fakefeature;

    featuredList* features;
    const ActionTemplate* atmpl;
};

}

#endif // QLEARNGRADIENT_HPP
