
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

class QTable
{
public:
    QTable(const StateTemplate* stmp, const ActionTemplate* atmp);
  
//     template<class Archive>
//     void serialize(Archive& ar, const unsigned int version)
//     {
//         (void) version;
//       //  ar & make_nvp("map", *map);
//     }
   
    
    const boost::unordered_map<DAction, double, DAction::hashfunctor>* operator[](const DState& name) const; 
    
    double operator()(const DState& s, const DAction& a) const;
    double& operator()(const DState& s, const DAction& a);

    void write(const string& chemin);
    void read(const string& chemin);

private:
    boost::unordered_map< DState,  boost::unordered_map<DAction, double, DAction::hashfunctor> , DState::hashfunctor> *map;
    //TODO: could be slightly improve argmax performance with using multiset and hashmap
    //boost::unordered_map< DState,  std::multiset<DAction, double> , DState::hashfunctor> *map;
};

}

#endif
