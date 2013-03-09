
#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>
#include "sml/ActionTemplate.hpp"

using std::string;
using boost::serialization::make_nvp;

namespace sml {

class Action {

private:
    double *values;
    ActionTemplate *templ;
};

class DAction {

public:
    //DAction();
    DAction(const ActionTemplate* temp, const std::list<int>& vals);
    DAction(const ActionTemplate* temp, int value);
    ~DAction();
//   int get( int index) const;
    int get(const string& name) const;
    int operator[](const string& name) const;
    int& operator[](const string& name);
    bool operator==(const DAction& ac) const;
    bool operator<(const DAction& ac) const;
    unsigned int hash() const;

    struct hashfunctor {
        size_t operator() (const DAction& ac) const {
            return ac.hash();
        }
    };

    
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      (void) version;
      for(int i = 0 ; i< templ->actionNumber(); i++)
	ar & make_nvp("values", values[i]);
    }

private:
    int *values = nullptr;
    const ActionTemplate *templ;
};

typedef DAction DState;

}

#endif
