#include "sml/Action.hpp"
#include <assert.h>
#include <bib/Logger.hpp>

namespace sml {

DAction::DAction(){
}
  
DAction::DAction(const ActionTemplate* temp, const std::list< int>& vals)
{
    assert((int)vals.size() == temp->actionNumber());

    this->templ = temp;

    values = new int[templ->actionNumber()];
    int i = 0;
    for(std::list< int>::const_iterator it = vals.begin(); it != vals.end(); ++it)
    {
        values[i]= *it;
        i++;
    }
}

DAction::DAction(const ActionTemplate* temp, int value) {
    this->templ = temp;

    values = new int[templ->actionNumber()];

    list<int>::const_iterator it = templ->sizesActions()->begin();
    it++; //always ignore first multiplier
    for(int i = 0 ; i< templ->actionNumber(); i++) {
        int multiplier = 1;

        for(; it != templ->sizesActions()->end(); ++it) //compute multiplier
            multiplier *= *it;

        values[i] = (int) (value / multiplier);
	value -= values[i]*multiplier;

        for(int j=0; j < (templ->actionNumber() -1) - (i + 1); j++) //refill
            it--;
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

unsigned int DAction::hash() const
{
    unsigned int hash = 0;
    list<int>::const_iterator it = templ->sizesActions()->begin();
    it++; //always ignore first multiplier

    for(int i = 0 ; i< templ->actionNumber(); i++) {
        int multiplier = 1;

        for(; it != templ->sizesActions()->end(); ++it) //compute multiplier
            multiplier *= *it;

        hash += values[i] * multiplier;

        for(int j=0; j < (templ->actionNumber() -1) - (i + 1); j++) //refill
            it--;
    }

    return hash;
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

bool DAction::operator<(const DAction& ac) const {
    return hash() < ac.hash();
}

}



