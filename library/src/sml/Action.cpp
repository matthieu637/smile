#include "sml/Action.hpp"
#include <assert.h>
#include <bib/Logger.hpp>

namespace sml {



ActionTemplate::ActionTemplate(const std::initializer_list<string>& names, const std::initializer_list<int>& sizes): actionNames(names.size()), sizes(sizes) {
    assert(names.size() == sizes.size());
  
    unsigned int i=0;
    for(std::initializer_list<string>::const_iterator it = names.begin(); it != names.end(); ++it)
    {
        actionNames[*it] = i;
        i++;
    }
}

int ActionTemplate::indexFor(const string& name) const {
//     LOG_DEBUG(name << " " << actionNames.size());
    assert(name.size() > 0 && actionNames.find(name) != actionNames.end());
    return actionNames.at(name);
}

int ActionTemplate::actionNumber() const {
    return actionNames.size();
}

bool ActionTemplate::operator==(const ActionTemplate& ac) const{
    return actionNames == ac.actionNames && sizes == ac.sizes;
}

const std::list<int>* ActionTemplate::sizesActions() const{
    return &this->sizes;
}


DAction::DAction(const ActionTemplate* temp, const std::initializer_list< int>& vals)
{
    assert((int)vals.size() == temp->actionNumber());

    this->templ = temp;

    values = new int[templ->actionNumber()];
    int i = 0;
    for(std::initializer_list< int>::const_iterator it = vals.begin(); it != vals.end(); ++it)
    {
        values[i]= *it;
        i++;
    }
}

DAction::~DAction() { //FIXME
//   if(values != nullptr)
//     delete[] values;
}

int DAction::get(const string& name) const {
    return values[templ->indexFor(name)];
}

int DAction::operator[](const string& name) const {
    return get(name);
}

int& DAction::operator[](const string& name) { //cannot be inline
    return values[templ->indexFor(name)];
}

bool DAction::operator==(const DAction& ac) const {
    if(*(ac.templ) == *(templ)) {

        for(int i=0; i<templ->actionNumber(); i++)
            if(values[i] != ac.values[i])
                return false;

        return true;
    }
    return false;
}

}



