
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
using std::vector;

namespace sml {

typedef vector<double> hashmap;
  
class QTable
{
public: 
    friend class boost::serialization::access;
    QTable(const StateTemplate* stmp, const ActionTemplate* atmp);
    
    const hashmap* operator[](const DState& name) const; 
    DAction* argmax(const DState& name) const;
    
    double operator()(const DState& s, const DAction& a) const;
    double& operator()(const DState& s, const DAction& a);
    
    double operator()(unsigned int s, unsigned int a) const;
    double& operator()(unsigned int s, unsigned int a);

    void write(const string& chemin);
    void read(const string& chemin);
    
    hashmap* getWholeCouple();
    void init();

private:
    hashmap *map = nullptr;
    //TODO: could be slightly improve argmax performance by using multiset and hashmap together
    
    const StateTemplate* stmpl;
    const ActionTemplate* atmpl;
};



}

#endif
