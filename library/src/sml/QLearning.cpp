#include "sml/QLearning.hpp"
#include "sml/Utils.hpp"

namespace sml {

QLearning::QLearning(const StateTemplate* stmp, const ActionTemplate* atmp, const DState& s, const DAction& a, const LearnConfig& conf) :
    LearnStat(conf), Q(stmp, atmp), atmp(atmp)
{
    ds = new DState(s);
    da = new DState(a);
}

QLearning::~QLearning(){
    delete ds;
    delete da;
}

DAction* QLearning::learn(const DState& s, double r, float lrate, float epsilon, float discount)
{
//     LOG_DEBUG(s << " " << *ds);
    DAction *ap = Q.argmax(s);
    Q(ds,da) = Q(ds,da) + lrate*(r+discount*Q(s, *ap) - Q(ds, da) );

    //exploitation
    DAction* a = ap ; //Q.argmax(s); // Choose a from s

    //exploration
    if(sml::Utils::rand01() < epsilon ){
	delete a;
        a = new DAction(atmp, {rand() % (int)atmp->sizeNeeded()});
    }

    delete ds;
    delete da;
    
    ds = new DState(s);
    da = a;

    return a;
}

void QLearning::should_done(const DState& s, const DAction& a, double r, float lrate)
{
    Q(s,a) = r;
}

DAction* QLearning::decision(const DState& s, float epsilon) {
    if(sml::Utils::rand01() < epsilon ){
        return new DAction(atmp, {rand() % (int)atmp->sizeNeeded()});
    }
    return Q.argmax(s);
}

void QLearning::should_do(const DState& s, const DAction& a, double r, float lrate, float discount){
//     DAction *ap = Q.argmax(s);
//     Q(ds,da) = Q(ds,da) + lrate*(r+discount*Q(s, *ap) - Q(ds, da) );
    Q(s,a)=r;
    
//     delete ap;
    delete ds;
    delete da;
    
    ds = new DState(s);
    da = new DAction(a);
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


