#include "sml/QLearning.hpp"
#include "sml/Utils.hpp"

namespace sml {

QLearning::QLearning(const StateTemplate* stmp, const ActionTemplate* atmp, const DState& s, const DAction& a, const LearnConfig& conf) :
    LearnStat(conf), Q(stmp, atmp), atmp(atmp),ds(&s),da(&a)
{


}

DAction* QLearning::learn(const DState& s, double r, float lrate, float epsilon, float discount)
{
    DAction *ap = Q.argmax(s);
    Q(ds,da) = Q(ds,da) + lrate*(r+discount*Q(s, *ap) - Q(ds, da) );

    //exploitation
    DAction* a = ap ; //Q.argmax(s); // Choose a from s

    //exploration
    if(sml::Utils::rand01() < epsilon )
        a = new DAction(atmp, {rand() % (int)atmp->sizeNeeded()});//TODO:memory

    ds = &s;
    da = a;

    return a;
}

void QLearning::should_done(const DState& s, const DAction& a, double r, float lrate)
{
    Q(s,a) = Q(s,a) + lrate*(r - Q(s,a));
}

DAction* QLearning::decision(const DState& s) {
    return Q.argmax(s);
}

void QLearning::save(boost::archive::xml_oarchive* xml)
{
    Q.save(xml);
}

void QLearning::load(boost::archive::xml_iarchive* xml)
{
    Q.load(xml);
}

}


