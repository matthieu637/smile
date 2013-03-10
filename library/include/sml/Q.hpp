
#ifndef Q_HPP
#define Q_HPP

#include "sml/Action.hpp"

#include <string>
#include <list>
#include <unordered_map>
#include <set>
#include <boost/serialization/nvp.hpp>
#include <boost/unordered/unordered_map.hpp>

using boost::serialization::make_nvp;
using std::string;

namespace sml {

typedef boost::unordered_map<DAction, double, DAction::hashfunctor> ActionsTable;
typedef boost::unordered_map< DState,  ActionsTable , DState::hashfunctor> StateTable;
  
class QTable
{
public: 
    QTable(const StateTemplate* stmp, const ActionTemplate* atmp);
    
    const ActionsTable* operator[](const DState& name) const; 
    const DAction* argmax(const DState& name) const;
    
    double operator()(const DState& s, const DAction& a) const;
    double& operator()(const DState& s, const DAction& a);

    void write(const string& chemin);
    void read(const string& chemin);
    
    StateTable* getWholeCouple();

private:
    StateTable *map;
    //TODO: could be slightly improve argmax performance by using multiset and hashmap together
    
    const StateTemplate* stmpl;
    const ActionTemplate* atmpl;
};

}

#endif
