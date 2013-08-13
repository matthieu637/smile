#include "sml/QLearning.hpp"
#include "sml/Utils.hpp"

namespace sml {

QLearning::QLearning(const StateTemplate* stmp, const ActionTemplate* atmp, const DState& s, const DAction& a, RLParam param, const LearnConfig& conf) :
    LearnStat(conf), DPolicy(param), Q(stmp, atmp), P(stmp, atmp), atmp(atmp)
{
    ds = new DState(s);
    da = new DAction(a);
}

QLearning::QLearning(const QLearning& q):LearnStat(q.conf), DPolicy(q.param), Q(q.Q), P(q.P), atmp(q.atmp) {
    ds = new DState(*q.ds);
    da = new DAction(*q.da);
}

QLearning::~QLearning() {
    delete ds;
    delete da;
}

LearnReturn QLearning::_learn(const DState& s, double r)
{
    DAction *ap = Q.argmax(s);
    Q(ds,da) = Q(ds,da) + param.alpha*(r+param.gamma*Q(s, *ap) - Q(ds, da) );

    //Choose a from s using policy derived from Q
    DAction* a = decision(s, param.epsilon);

//     LOG_DEBUG(s << " " << *ds << *da << *a);
    
    delete ap;
    delete ds;
    delete da;

    ds = new DState(s);
    da = a;

    return {a, false};
}

void QLearning::should_done(const DState& s, const DAction& a)
{
//     LOG_DEBUG("advice "<< s << " " << a);
    switch(adviceStrat) {
    case FixedNExploration:
    case FixedNMax:
        DAction* b = P.argmax(s);
        if(P(s, *b) == 1 && !(*b == a) )
            P(s, *b) = 0;
        delete b;

        P(s,a)=1;
        break;
    }

    switch(adviceStrat) {
    case FixedNMax:
    case Max:
        DAction* ba = Q.argmax(s);
        Q(s,a) = Q(s,*ba) + 0.000001;
        delete ba;
    }
}

DAction* QLearning::decision(const DState& s, bool greedy) {
    DAction* a = Q.argmax(s);

    bool may_greedy = greedy;
    if(adviceStrat == FixedNExploration || adviceStrat == FixedNMax) {
        DAction* b = P.argmax(s);
        if( P(s, *b) == 1 ) {
            a = b;
            may_greedy = false;
        }
        else
            delete b;
    }

    //exploration
    if(may_greedy && sml::Utils::rand01() < param.epsilon ) {
        delete a;
        a = new DAction(atmp, {rand() % (int)atmp->sizeNeeded()});
    }

//     LOG_DEBUG("dec " <<s << " " << *a);


//     if(adviceStrat != None)
//       LOG_DEBUG("dec "<< *a << " from " << s );
//     LOG_DEBUG(adviceStrat);
    return a;
}

void QLearning::clear_history(const DState& s, const DAction& a)
{
    delete ds;
    delete da;
    ds = new DState(s);
    da = new DAction(a);
}

void QLearning::should_do(const DState& s, const DAction& a, double reward) {
    (void) reward;
    should_done(s, a);

    clear_history(s, a);
}

void QLearning::had_choosed(const DState&, const DAction&, double, bool){
  
}

float QLearning::getStateImportance(const DState& s){
    return 0;
};

Policy<DState>* QLearning::copyPolicy() {
    return new QLearning(*this);
}

const QTable& QLearning::getPolicy() {
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






