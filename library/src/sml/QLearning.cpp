#include "sml/QLearning.hpp"
#include "sml/Utils.hpp"

namespace sml {

QLearning::QLearning(const StateTemplate* stmp, const ActionTemplate* atmp, const DState& s, const DAction& a) : Q(stmp, atmp), atmp(atmp),ds(&s),da(&a)
{


}

DAction* QLearning::decision(const DState& s, double r, float lrate, float epsilon, float discount)
{
    DAction *ap = Q.argmax(s);
    Q(ds,da) = Q(ds,da) + lrate*(r+discount*Q(s, *ap) - Q(ds, da) );
    LOG_DEBUG("e"<<*ds << " a" << *da << " q" <<Q(ds,da));

    //exploitation
    DAction* a = ap ; //Q.argmax(s); // Choose a from s

    //exploration
    if(sml::Utils::rand01() < epsilon )
        a = new DAction(atmp, {rand() % (int)atmp->sizeNeeded()});//TODO:memory
    
    ds = &s;
    da = a;

    return a;
}

void QLearning::write(const string& arg1)
{
    Q.write(arg1);
}

void QLearning::read(const string& arg1)
{
    Q.read(arg1);
}
}


