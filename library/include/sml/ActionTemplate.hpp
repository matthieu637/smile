#ifndef ACTIONTEMPLATE_HPP
#define ACTIONTEMPLATE_HPP

#include <string>
#include <list>
#include <unordered_map>

using std::list;
using std::string;

namespace sml{

class ActionTemplate {
public:
    ActionTemplate(const std::list<string>& names, const std::list<int>& sizes);
    int indexFor(const string& name) const;
    int actionNumber() const;
    const std::list<int>* sizesActions() const;
    bool operator==(const ActionTemplate& ac) const;
    unsigned int sizeNeeded() const;
private :
    //std::list<string> actionNames;
    std::unordered_map< string, int> actionNames;
    std::list<int> sizes;
};

typedef ActionTemplate StateTemplate;

}

#endif // ACTIONTEMPLATE_HPP
