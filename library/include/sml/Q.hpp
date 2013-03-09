
#ifndef Q_HPP
#define Q_HPP

#include "sml/Action.hpp"

#include <string>
#include <list>
#include <unordered_map>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>

using boost::serialization::make_nvp;
using std::string;

namespace sml {

class QTable
{
public:
    QTable();
  
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        (void) version;
      //  ar & make_nvp("map", *map);
    }
   
    
    std::unordered_map<DAction, double, DAction::hashfunc> operator[](const DState& name) const; 
    
    double operator()(DState s, DAction a) const;

    void write(const string& chemin);
    void read(const string& chemin);

private:
  
    std::unordered_map< DState,  std::unordered_map<DAction, double, DAction::hashfunc> , DState::hashfunc> *map;

};

}

#endif
