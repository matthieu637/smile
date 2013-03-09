
#include "sml/Q.hpp"

#include <bib/XMLEngine.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/unordered/unordered_map.hpp>
#include <boost/unordered_map.hpp>
#include "unordered_map_serialization.hpp"


using namespace boost::interprocess;

namespace sml {

QTable::QTable(const StateTemplate* stmp, const ActionTemplate* atmp) {
    map = new boost::unordered_map< DState,  boost::unordered_map<DAction, double, DAction::hashfunctor> , DState::hashfunctor>(stmp->sizeNeeded());

    boost::unordered_map<DAction, double, DAction::hashfunctor> actions(atmp->sizeNeeded());

    for(unsigned int i=0; i< atmp->sizeNeeded(); i++) {
        DAction ac(atmp, i);
        actions[ac] = 0.L;
    }

    for(unsigned int i=0; i< stmp->sizeNeeded(); i++) {
        DState st(stmp, i);
        (*map)[st] = actions;
    }
    /*
    DAction a(atmp, {2,2});
    DState st(stmp, {2, 2});
    LOG_DEBUG((*map)[st][a]);*/
}

void QTable::write(const string& chemin)
{
    named_mutex mutex( open_or_create, chemin.c_str());
    mutex.lock();

    bib::XMLEngine::save< boost::unordered_map< DState,  boost::unordered_map<DAction, double, DAction::hashfunctor> , DState::hashfunctor> >(*map, "map", chemin);

    mutex.unlock();
}

void QTable::read(const string& chemin)
{
    named_mutex mutex( open_or_create, chemin.c_str());
    mutex.lock();

    // QTable* q2 = bib::XMLEngine::load<QTable>("QTable", chemin);
    // this->map = q2->map;

    mutex.unlock();
}

const boost::unordered_map<DAction, double, DAction::hashfunctor>* QTable::operator[](const DState& s) const
{
    return &(map->at(s));
}

double QTable::operator()(const DState& s, const DAction& a) const
{
    return map->at(s).at(a);
}

double& QTable::operator()(const DState& s, const DAction& a)
{
    return map->at(s)[a]; //TODO:be sure
}

}
