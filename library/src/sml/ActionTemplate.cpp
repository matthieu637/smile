#include "sml/ActionTemplate.hpp"

#include <assert.h>
#include <bib/Logger.hpp>

namespace sml {

ActionTemplate::ActionTemplate() {

}

ActionTemplate::ActionTemplate(const std::list<string>& names, const std::list<int>& sizes): actionNames(names.size()), sizes(sizes) {
    assert(names.size() == sizes.size());

    unsigned int i=0;
    for(std::list<string>::const_iterator it = names.begin(); it != names.end(); ++it)
    {
        actionNames[*it] = i;
        i++;
    }
}

ActionTemplate::~ActionTemplate(){
    actionNames.clear();
    sizes.clear();
}

int ActionTemplate::actionNumber(const string& name) const {
    unsigned int reach = actionNames.at(name);
    unsigned int i = 0;

    std::list<int>::const_iterator it;
    for(it = sizes.begin(); it != sizes.end(); ++it) {
        if(i==reach)
            return *(it);
        i++;
    }

    return (*it);
}

int ActionTemplate::indexFor(const string& name) const {
//     LOG_DEBUG(name << " " << actionNames.size());
    assert(name.size() > 0 && actionNames.find(name) != actionNames.end());
    return actionNames.at(name);
}

int ActionTemplate::actionNumber() const {
    return actionNames.size();
}


bool ActionTemplate::operator==(const ActionTemplate& ac) const {
    return actionNames == ac.actionNames && sizes == ac.sizes;
}

const std::list<int>* ActionTemplate::sizesActions() const {
    return &this->sizes;
}

unsigned int ActionTemplate::sizeNeeded() const {
    unsigned int r = 1;

    for(list<int>::const_iterator it = sizes.begin(); it != sizes.end(); ++it)
        r*=(*it);
    return r;
}

}
