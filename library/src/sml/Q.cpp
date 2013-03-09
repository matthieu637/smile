
#include "sml/Q.hpp"

#include <bib/XMLEngine.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

using namespace boost::interprocess;

namespace sml {
  
QTable::QTable(){
  map = new std::unordered_map< DState,  std::unordered_map<DAction, double, DAction::hashfunc> , DState::hashfunc>(10);
  
  sml::StateTemplate* omg = new sml::StateTemplate({"t"},{3});
  DAction a(omg, {3});
  
  std::unordered_map<DAction, double,  DAction::hashfunc> op;
  op[a]=5.;
  
  DState q(omg, {1});
  (*map)[q]=op;
}

void QTable::write(const string& chemin)
{
    named_mutex mutex( open_or_create, chemin.c_str());
    mutex.lock();

   // bib::XMLEngine::save<QTable>(*this, "QTable", chemin);

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

std::unordered_map<DAction, double, DAction::hashfunc> QTable::operator[](const DState& s) const
{
  return map->at(s);
}

double QTable::operator()(DState s, DAction a) const
{
  return map->at(s).at(a);
}

}