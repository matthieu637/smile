#ifndef ACTIONTEMPLATE_HPP
#define ACTIONTEMPLATE_HPP

#include <string>
#include <list>
#include <boost/unordered/unordered_map.hpp>
// #include <boost/serialization/nvp.hpp>
// #include <boost/serialization/list.hpp>

using std::list;
using std::string;
//using boost::serialization::make_nvp;

namespace sml{

class ActionTemplate {
public:
    ActionTemplate();//empty constructor for serialization
    ActionTemplate(const std::list<string>& names, const std::list<int>& sizes);
    int indexFor(const string& name) const;
    int actionNumber() const;
    const std::list<int>* sizesActions() const;
    bool operator==(const ActionTemplate& ac) const;
    unsigned int sizeNeeded() const;
    /*
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      (void) version;
      ar & make_nvp("ActionName", actionNames);
      ar & make_nvp("Sizes", sizes);
      
    }*/
private :
    boost::unordered_map< string, int> actionNames;
    std::list<int> sizes;
};

typedef ActionTemplate StateTemplate;

}

#endif // ACTIONTEMPLATE_HPP
