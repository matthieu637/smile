#ifndef RLGRADIENT_HPP
#define RLGRADIENT_HPP

#include "sml/Q.hpp"
#include "sml/Feature.hpp"
#include "sml/Utils.hpp"
#include "Policy.hpp"
#include <boost/serialization/list.hpp>

// void GetTiles(int tiles[],int num_tilings,float variables[], int num_variables,
//               int memory_size,int hash1=-1, int hash2=-1, int hash3=-1);



namespace sml {

typedef boost::numeric::ublas::vector<double> dbvector;
typedef boost::numeric::ublas::vector<bool> bbvector;

using boost::numeric::ublas::zero_vector;

enum custom_dec_learn {normal, forced_clear, forced_no_clear};


template <class State>
class RLGradientDescent : public Policy<State> {

public:

    RLGradientDescent(featuredList<State>* features, unsigned int nbFeature, const ActionTemplate* atmp, const State&,
                      const DAction& initial, RLParam param, StrategyEffectsAdvice sea) :
        Policy<State>(param, sea),
        nbFeature(nbFeature),
        teta(nbFeature), e(zero_vector<double>(nbFeature)),
        Qa(atmp), actions(atmp->sizeNeeded()),
        features(features), atmpl(atmp), history()
    {
        for(unsigned int i=0; i<nbFeature; i++)
            teta[i] =  0L ;

        for(unsigned int i=0; i < atmp->sizeNeeded() ; i++) {
            actions[i] = new DAction(atmpl, i);
        }

        lastAction = new DAction(initial);
    }

    RLGradientDescent(const RLGradientDescent& q) : Policy<State>(q.param, q.adviceStrat), nbFeature(q.nbFeature),
        teta(q.teta), e(q.e), Qa(q.Qa), actions(q.atmpl->sizeNeeded()), features(q.features), atmpl(q.atmpl),
        history(q.history)
    {
        for(unsigned int i=0; i < atmpl->sizeNeeded() ; i++)
            actions[i] = new DAction(atmpl, i);
        lastAction = new DAction(*q.lastAction);
    }

    virtual ~RLGradientDescent() {
        for(vector<DAction*>::iterator ai = actions.begin(); ai != actions.end() ; ++ai)
            delete *ai;
        delete lastAction;
    }


    void startEpisode(const State& s, const DAction& a) {
        delete lastAction;
        lastAction = new DAction(a);

        resetTraces();

        //start an new episod
        computeQa(s);
        _startEpisode(s, a);
    }

    void should_done(const State&, const DAction&) {

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

///
///\brief Retourner l'action à faire selon l'algorithme sans apprentissage
///\param state : l'état présent
    DAction* decision(const State& state, bool greedy) {
        if(greedy && sml::Utils::rand01(this->param.epsilon)  ) {
            return new DAction(atmpl, {rand() % (int)atmpl->sizeNeeded()});
        }

        computeQa(state);
        return Qa.argmax();
    }

///
///\brief Sauvegarder ce que l'algorithme a appris
///\param xml : le fichier XML
    void save(boost::archive::xml_oarchive* xml)
    {
// 	bib::Logger::PRINT_ELEMENTS<dbvector>(teta);
        *xml << make_nvp("teta", teta);
        int i=0;
        for(fLiterator<State> flist = features->begin() ; flist != features->end(); ++flist) {
            (*flist)->save(xml, i);
            i++;
        }
    }

///
///\brief Charger ce que l'algorithme a appris
///\param xml : le fichier XML
    void load(boost::archive::xml_iarchive* xml) {
//       	bib::Logger::PRINT_ELEMENTS<dbvector>(teta);
        *xml >> make_nvp("teta", teta);
        int i=0;
        for(fLiterator<State> flist = features->begin() ; flist != features->end(); ++flist) {
            (*flist)->load(xml, i);
            i++;
        }
// 	bib::Logger::PRINT_ELEMENTS<dbvector>(teta);
    }

protected:

    void resetTraces() {
        for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it)
            e[*it] = 0L;
        history.clear();
        // 	   e *= 0.;
    }

    void decayTraces() {
        for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it) {
            unsigned int index = *it;
            e[index] = this->param.lambda * this->param.gamma * e[index];
        }
//            e *= this->param.lambda * this->param.gamma;
    }

    void addTraces(const State& state, const DAction& a) {
        list<int>* activeIndex = extractFeatures(state, a);
        for(list<int>::iterator it = activeIndex->begin(); it != activeIndex->end() ; ++it) {
            int index = *it;
            if(this->param.accumu)
                e[index] += 1.;
            else
                e[index] = 1.;
            history.insert(index);
        }
        delete activeIndex;
    }

    void updateWeights(float delta) {
        // 	teta = teta + this->param.alpha * delta * e;
        for (std::set<unsigned int>::iterator it=history.begin(); it!=history.end(); ++it) {
            int index = *it;
            teta[index] = teta[index] + ( this->param.alpha/this->param.tiling ) * delta * e[index];
        }
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
            if(active != -1)
                actived->push_back(layer + active);
            layer += size;
        }

        return actived;
    }

//     list<int>* extractFeatures(const State& st, const DAction& ac) {
//         int a = ac["motor"];
//         simu::MCarState& state = (simu::MCarState&)st;
//         list<int>* actived = new list<int>;
//
//         float state_vars[2];
//         int F[8];
//         state_vars[0] = state.position / (1.7 / 16.);
//         state_vars[1] = state.velocity / (0.14 / 16.);
//
//         GetTiles(&F[0], 8, state_vars, 2, 12288, a);
//
//         for(int i=0; i<8; i++)
//             actived->push_back( F[i] );
//
// //   bib::Logger::PRINT_ELEMENTS<list<int>>(*actived);
//         return actived;
//     }

    virtual void _startEpisode(const State&, const DAction&) = 0;

protected:
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

#endif
