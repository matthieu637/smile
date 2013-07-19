#include "sml/QLearning.hpp"
#include "sml/Utils.hpp"

namespace sml {

QLearning::QLearning(const StateTemplate* stmp, const ActionTemplate* atmp, const DState& s, const DAction& a, const LearnConfig& conf) :
    LearnStat(conf), Q(stmp, atmp), atmp(atmp)
{
    ds = new DState(s);
    da = new DAction(a);
}

QLearning::QLearning(const QLearning& q):LearnStat(q.conf), Q(q.Q), atmp(q.atmp){
    ds = new DState(*q.ds);
    da = new DAction(*q.da);
}

QLearning::~QLearning() {
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
    if(sml::Utils::rand01() < epsilon ) {
        delete a;
        a = new DAction(atmp, {rand() % (int)atmp->sizeNeeded()});
    }

    delete ds;
    delete da;

    ds = new DState(s);
    da = a;

    return a;
}

void QLearning::should_done(const DState& s, const DAction& a)
{
//     LOG_DEBUG("ok");
    DAction* ba = Q.argmax(s);
    Q(s,a) = Q(s,*ba);
    delete ba;
}

DAction* QLearning::decision(const DState& s, float epsilon) {
    if(sml::Utils::rand01() < epsilon ) {
        return new DAction(atmp, {rand() % (int)atmp->sizeNeeded()});
    }
    return Q.argmax(s);
}

void QLearning::clear_history(const DState& s, const DAction& a)
{
    delete ds;
    delete da;
    ds = new DState(s);
    da = new DAction(a);
}

void QLearning::should_do(const DState& s, const DAction& a) {
//     DAction *ap = Q.argmax(s);
//     Q(ds,da) = Q(ds,da) + lrate*(r+discount*Q(s, *ap) - Q(ds, da) );
    Q(s,a)= 10000;

    clear_history(s, a);
}

Policy<DState>* QLearning::copyPolicy(){
    return new QLearning(*this);
}

const QTable& QLearning::getPolicy(){
    return Q;
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


