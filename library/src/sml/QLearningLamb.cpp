#include "sml/QLearningLamb.hpp"
#include "sml/Utils.hpp"

namespace sml
{

QLearningLamb::QLearningLamb ( const StateTemplate* stmp, const ActionTemplate* atmp, const DState& ds, const DAction& da, const LearnConfig& conf ) :
    LearnStat ( conf ), Q ( stmp, atmp ), N ( stmp, atmp ), atmp ( atmp )
{
    this->s = new DState ( ds );
    this->a = new DState ( da );
//     history.insert ( std::pair< DState* , DAction* > ( s, a ) );
}

QLearningLamb::~QLearningLamb() {
    delete s;
    delete a;
}

DAction* QLearningLamb::learn (const DState& sp, double r, float lrate, float epsilon, float discount, float lambda, bool accumulative )
{
    //Take action a, observe r, s'

    //Choose a' from s' using policy derived from Q
    DAction* as = Q.argmax ( sp );
    //if a' ties for the max, then a* <- a'
    DAction* ap = as;
    if ( sml::Utils::rand01() < epsilon ) {
        ap = new DAction ( atmp, rand() % ( int ) atmp->sizeNeeded() );
    }

    double delta = r + discount*Q ( sp, *as ) - Q ( s,a );
    if ( accumulative )
        N ( s,a ) = N ( s, a ) + 1.;
    else
        N ( s,a ) = 1.;

    bool inserted = history.insert ( std::pair< DState* , DAction* > ( s, a ) ).second;
    if(!inserted) {
        delete s;
        delete a;
    }

    for ( std::set< std::pair<DState* , DAction* >, HistoryComparator >::iterator it = history.begin(); it != history.end(); ++it ) {
        DState* sa = it->first;
        DAction* aa = it->second;

        Q ( sa,aa ) = Q ( sa,aa ) + lrate * delta * N ( sa,aa );
        if ( ap == as )
            N ( sa, aa ) = discount*lambda*N ( sa,aa );
        else {
            N ( sa, aa ) = 0.;
            delete sa;
            delete aa;
        }
    }

    if ( ap != as ) {
        delete as;
        history.clear();
    }

    a = ap;
    s = new DState ( sp );

//     LOG_DEBUG(history.size());

    return ap;
}

void QLearningLamb::clear_history(const DState& ds, const DAction& da)
{
    delete s;
    delete a;

    this->s = new DState ( ds );
    this->a = new DState ( da );

    for ( std::set< std::pair<DState* , DAction* >, HistoryComparator >::iterator it = history.begin(); it != history.end(); ++it ) {
        DState* sa = it->first;
        DAction* aa = it->second;
        N ( sa, aa ) = 0.;
        delete sa;
        delete aa;
    }

    history.clear();
//     N.print();
//     Q.print();
}

DAction* QLearningLamb::decision (const DState& s, float epsilon)
{
    if(sml::Utils::rand01() < epsilon ) {
        return new DAction(atmp, {rand() % (int)atmp->sizeNeeded()});
    }
    return Q.argmax ( s );
}

void QLearningLamb::save ( boost::archive::xml_oarchive* xml )
{
    Q.save ( xml );
}

void QLearningLamb::load ( boost::archive::xml_iarchive* xml )
{
    Q.load ( xml );
}

}


