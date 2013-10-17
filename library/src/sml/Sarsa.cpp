#include "sml/Sarsa.hpp"
#include "sml/Utils.hpp"

namespace sml {

Sarsa::Sarsa(const StateTemplate* stmp, const ActionTemplate* atmp, const DState& s, const DAction& a) :
    Q(stmp, atmp), P(stmp, atmp), atmp(atmp)
{
    ds = new DState(s);
    da = new DAction(a);
}

Sarsa::Sarsa(const Sarsa& q):Q(q.Q), P(q.P), atmp(q.atmp) {
    ds = new DState(*q.ds);
    da = new DAction(*q.da);
}

Sarsa::~Sarsa() {
    delete ds;
    delete da;
}

DAction* Sarsa::learn(const DState& s, double r, float lrate, float epsilon, float discount)
{
    //Choose a from s using policy derived from Q
    DAction* a = decision(s, epsilon);

    Q(ds,da) = Q(ds,da) + lrate*(r+discount*Q(s, *a) - Q(ds, da) );

    delete ds;
    delete da;

    ds = new DState(s);
    da = a;

    return a;
}

void Sarsa::should_done(const DState&, const DAction&)
{
// //     LOG_DEBUG(s << " " << a);
//     switch(adviceStrat) {
//     case FixedNExploration:
//     case FixedNMax:
//         DAction* b = P.argmax(s);
//         if(P(s, *b) == 1 && !(*b == a) )
//             P(s, *b) = 0;
//         delete b;
// 
//         P(s,a)=1;
//         break;
//     }
// 
//     switch(adviceStrat) {
//     case FixedNMax:
//     case Max:
//         DAction* ba = Q.argmax(s);
//         Q(s,a) = Q(s,*ba) + 0.000001;
//         delete ba;
//     }
}

DAction* Sarsa::decision(const DState&, float) {
//     DAction* a = Q.argmax(s);
// 
//     bool greedy = true;
//     if(adviceStrat == FixedNExploration || adviceStrat == FixedNMax) {
//         DAction* b = P.argmax(s);
//         if( P(s, *b) == 1 ) {
//             a = b;
//             greedy = false;
//         }
//         else
//             delete b;
//     }
// 
//     //exploration
//     if(greedy && sml::Utils::rand01() < epsilon ) {
//         delete a;
//         a = new DAction(atmp, {rand() % (int)atmp->sizeNeeded()});
//     }
// 
// //     LOG_DEBUG("dec " <<s << " " << *a);
//     
//     
// //     if(adviceStrat != None)
// //       LOG_DEBUG("dec "<< *a << " from " << s );
// //     LOG_DEBUG(adviceStrat);
//     return a;
  return nullptr;
}

void Sarsa::clear_history(const DState& s, const DAction& a)
{
    delete ds;
    delete da;
    ds = new DState(s);
    da = new DAction(a);
}

void Sarsa::should_do(const DState& s, const DAction& a) {
    should_done(s, a);

    clear_history(s, a);
}

// Policy<DState>* Sarsa::copyPolicy() {
//     return new Sarsa(*this);
// }

const QTable& Sarsa::getPolicy() {
    return Q;
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





