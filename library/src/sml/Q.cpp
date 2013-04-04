
#include "sml/Q.hpp"

#include <bib/XMLEngine.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include "bib/Logger.hpp"

using namespace boost::interprocess;

namespace sml {

QTable::QTable(const StateTemplate* stmp, const ActionTemplate* atmp):stmpl(stmp), atmpl(atmp) {
    map = new hashmap(stmpl->sizeNeeded() * atmpl->sizeNeeded());

    for(unsigned int i=0; i< stmpl->sizeNeeded(); i++) {
        for(unsigned int j=0; j< atmpl->sizeNeeded(); j++) {
            map->at(i*atmpl->sizeNeeded() + j) = 0.L;
        }
    }
}

QTable::QTable(const ActionTemplate* atmp):atmpl(atmp) {
    stmpl = new StateTemplate( {""}, {1}); // for file saving
    map = new hashmap(atmpl->sizeNeeded());

    for(unsigned int j=0; j< atmpl->sizeNeeded(); j++)
        map->at(j) = 0.L;
}

DAction* QTable::argmax(const DState& name) const {
    unsigned int hashState = name.hash();
    unsigned int beginRange = hashState * atmpl->sizeNeeded();

    unsigned int imax = rand() % atmpl->sizeNeeded();
//     LOG_DEBUG(imax << " " << hashState << " " << atmpl->sizeNeeded() << " " << name["angle"] << " " << name["distance"] );
    for(unsigned int j=0; j< atmpl->sizeNeeded(); j++)
        if(map->at(beginRange + imax) < map->at(beginRange + j) )
            imax = j;

    return new DAction(atmpl, imax);
}

DAction* QTable::argmax() const {

    unsigned int imax = rand() % atmpl->sizeNeeded();
//     LOG_DEBUG(imax << " " << hashState << " " << atmpl->sizeNeeded() << " " << name["angle"] << " " << name["distance"] );
    for(unsigned int j=0; j< atmpl->sizeNeeded(); j++)
        if(map->at(imax) < map->at(j) )
            imax = j;

    return new DAction(atmpl, imax);
}

double QTable::operator()(const DState& s, const DAction& a) const
{
    return this->operator()(s.hash(), a.hash());
}

double& QTable::operator()(const DState& s, const DAction& a)
{
    return this->operator()(s.hash(), a.hash());
}

double QTable::operator()(const DState* s, const DAction* a) const {
    return this->operator()(*s, *a);
}
double& QTable::operator()(const DState* s, const DAction* a) {
    return this->operator()(*s, *a);
}

double QTable::operator()(const DAction& a) const
{
    return this->operator()(0, a.hash());
}

double& QTable::operator()(const DAction& a)
{
    return this->operator()(0, a.hash());
}


double QTable::operator()(unsigned int s, unsigned int a) const {
    unsigned int beginRange = s * atmpl->sizeNeeded();

//     LOG_DEBUG("acces at " << beginRange + a);
    return (*map)[beginRange + a];
}

double& QTable::operator()(unsigned int s, unsigned int a) {
    unsigned int beginRange = s * atmpl->sizeNeeded();

//     LOG_DEBUG("set at " << beginRange + a);
    return (*map)[beginRange + a];
}


hashmap* QTable::getWholeCouple() {
    return map;
}

void QTable::write(const string& chemin)
{
    named_mutex mutex( open_or_create, chemin.c_str());

    bib::XMLEngine::save< hashmap >(*map, "QTable", chemin);

    mutex.unlock();
}

void QTable::read(const string& chemin)
{
    if(  !boost::filesystem::exists( chemin ) ) {
        LOG_DEBUG(chemin << " n'existe pas.");
    }
    else {
        named_mutex mutex( open_or_create, chemin.c_str());
        mutex.lock();

        map = bib::XMLEngine::load< hashmap >("QTable", chemin);
    }
}

}
