
#include "sml/Q.hpp"

#include <bib/XMLEngine.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/unordered_map.hpp>
#include <boost/filesystem.hpp>
#include "unordered_map_serialization.hpp"


using namespace boost::interprocess;

namespace sml {

QTable::QTable(const StateTemplate* stmp, const ActionTemplate* atmp):stmpl(stmp), atmpl(atmp) { 
    map = new StateTable(stmp->sizeNeeded());

    ActionsTable actions(atmp->sizeNeeded());

    for(unsigned int i=0; i< atmp->sizeNeeded(); i++) {
        DAction ac(atmp, i);
        actions[ac] = 0.L;
    }

    for(unsigned int i=0; i< stmp->sizeNeeded(); i++) {
        DState st(stmp, i);
        (*map)[st] = actions;
    }
}

const DAction* QTable::argmax(const DState& name) const{
    const ActionsTable* actions = this->operator[](name);
    
    const DAction* imax = &(*actions->begin()).first;
    for(ActionsTable::const_iterator it = actions->begin();it !=  actions->end(); ++it)
      if( actions->at(*imax) < (*it).second)
	imax = &(*it).first;
      
    return imax;
}

const ActionsTable* QTable::operator[](const DState& s) const
{
    return &(map->at(s));
}

double QTable::operator()(const DState& s, const DAction& a) const
{
    return map->at(s).at(a);
}

double& QTable::operator()(const DState& s, const DAction& a)
{
    return map->at(s)[a];
}

StateTable* QTable::getWholeCouple(){
  return map;
}

void QTable::write(const string& chemin)
{
    named_mutex mutex( open_or_create, chemin.c_str());
//     mutex.unlock();
    mutex.lock();

    bib::XMLEngine::save< StateTable >(*map, "QTable", chemin);

    mutex.unlock();
}

void QTable::read(const string& chemin)
{
    if(  !boost::filesystem::exists( chemin ) ){
      LOG_DEBUG(chemin << " n'existe pas.");
    }
    else{
    named_mutex mutex( open_or_create, chemin.c_str());
//     mutex.unlock();
    mutex.lock();

    map = bib::XMLEngine::load< StateTable >("QTable", chemin);

    mutex.unlock();
    }
}

}
