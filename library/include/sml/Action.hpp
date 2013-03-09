
#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>
#include <list>
#include <unordered_map>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>


using std::list;
using std::string;
using boost::serialization::make_nvp;

namespace sml {

class ActionTemplate {
public:
    ActionTemplate(const std::initializer_list<string>& names, const std::initializer_list<int>& sizes);
    int indexFor(const string& name) const;
    int actionNumber() const;
    const std::list<int>* sizesActions() const;
    bool operator==(const ActionTemplate& ac) const;
private :
    //std::list<string> actionNames;
    std::unordered_map< string, int> actionNames;
    std::list<int> sizes;
};

typedef ActionTemplate StateTemplate;

class Action {

private:
    double *values;
    ActionTemplate *templ;
};

class DAction {

public:
    //DAction();
    DAction(const ActionTemplate* temp, const std::initializer_list<int>& vals);
    ~DAction();
//   int get( int index) const;
    int get(const string& name) const;
    int operator[](const string& name) const;
    int& operator[](const string& name);
    bool operator==(const DAction& ac) const;

    struct hashfunc {
        size_t operator() (const DAction& ac) const {
            int hash = 0;
            list<int>::const_iterator it = ac.templ->sizesActions()->begin();
            it++; //always ignore first multiplier

            for(int i = 0 ; i< ac.templ->actionNumber(); i++) {
                int multiplier = 1;

                for(; it != ac.templ->sizesActions()->end(); ++it) //compute multiplier
                    multiplier *= *it;

                hash += ac.values[i] * multiplier;

                for(int j=0; j < (ac.templ->actionNumber() -1) - (i + 1); j++) //refill
                    it--;
            }

            return hash;
        }
    };

    /*
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      (void) version;
      ar & make_nvp("values", values);
    }*/

private:
    int *values = nullptr;
    const ActionTemplate *templ;
};

typedef DAction DState;

}

#endif
