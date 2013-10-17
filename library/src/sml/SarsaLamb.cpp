#include "sml/SarsaLamb.hpp"
#include "sml/Utils.hpp"

namespace sml {

SarsaLamb::SarsaLamb(const StateTemplate* stmp, const ActionTemplate* atmp, DState& s, DAction& a) :
    Q(stmp, atmp), N(stmp, atmp), atmp(atmp),s(&s),a(&a)
{

}

void SarsaLamb::observeTutor(DState& sp, DAction& ac, double r, float lrate, float discount, float lambda, bool accumulative)
{

    double delta = r + discount*Q(sp, ac) - Q(s,a);
    if(accumulative)
        N(s,a) = N(s, a) + 1.;
    else
        N(s,a) = 1.;
    history.insert(std::pair<DState* , DAction* >(s, a));

    for(std::set< std::pair<DState* , DAction* >, HistoryComparator >::iterator it = history.begin(); it != history.end(); ++it ) {
        DState sa = *(*it).first;
        DState aa = *(*it).second;
        Q(sa,aa) = Q(sa,aa) + lrate * delta * N(sa,aa);

        N(sa, aa) = discount*lambda*N(sa,aa);
    }

    a = &ac;
    s = &sp;
}

DAction* SarsaLamb::learn(DState& sp, double r, float lrate, float epsilon, float discount, float lambda, bool accumulative)
{
    //Take action a, observe r, s'

    //Choose a' from s' using policy derived from Q
    DAction* ap = Q.argmax(sp); //TODO:memory leak
    //if a' ties for the max, then a* <- a'
    if(sml::Utils::rand01() < epsilon ) {
        ap = new DAction(atmp, rand() % (int)atmp->sizeNeeded());//TODO:memory
    }


    double delta = r + discount*Q(sp, *ap) - Q(s,a);
    if(accumulative)
        N(s,a) = N(s, a) + 1.;
    else
        N(s,a) = 1.;
    history.insert(std::pair<DState* , DAction* >(s, a));

    for(std::set< std::pair<DState* , DAction* >, HistoryComparator >::iterator it = history.begin(); it != history.end(); ++it ) {
        DState sa = *(*it).first;
        DState aa = *(*it).second;
        Q(sa,aa) = Q(sa,aa) + lrate * delta * N(sa,aa);

        N(sa, aa) = discount*lambda*N(sa,aa);
    }

    a = ap;
    s = &sp;

    return ap;
}

DAction* SarsaLamb::decision(DState& s) {
    return Q.argmax(s);
}

void SarsaLamb::save(boost::archive::xml_oarchive* xml)
{
    Q.save(xml);
}

void SarsaLamb::load(boost::archive::xml_iarchive* xml)
{
    Q.load(xml);
}

}


