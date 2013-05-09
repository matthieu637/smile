#include "sml/Sarsa.hpp"
#include "sml/Utils.hpp"

namespace sml {

Sarsa::Sarsa(const StateTemplate* stmp, const ActionTemplate* atmp, const DState& s, const DAction& a, const LearnConfig& conf) :
    LearnStat(conf), Q(stmp, atmp), atmp(atmp),ds(&s),da(&a)
{


}

DAction* Sarsa::learn(const DState& s, double r, float lrate, float epsilon, float discount)
{
    DAction *ap = Q.argmax(s);
    if(sml::Utils::rand01() < epsilon )
        ap = new DAction(atmp, {rand() % (int)atmp->sizeNeeded()});//TODO:memory
    
    Q(ds,da) = Q(ds,da) + lrate*(r+discount*Q(s, *ap) - Q(ds, da) );

    ds = &s;
    da = ap;

    return ap;
}

DAction* Sarsa::decision(const DState& s) {
    return Q.argmax(s);
}

void Sarsa::save(boost::archive::xml_oarchive* xml)
{
    Q.save(xml);
}

void Sarsa::load(boost::archive::xml_iarchive* xml)
{
    Q.load(xml);
}

}


