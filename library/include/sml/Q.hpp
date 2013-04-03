
#ifndef Q_HPP
#define Q_HPP

#include <string>
#include <vector>
#include "sml/Action.hpp"

using std::string;
using std::vector;

namespace sml {

typedef vector<double> hashmap;
  
class QTable
{
public: 
    QTable(const StateTemplate* stmp, const ActionTemplate* atmp);
    QTable(const ActionTemplate* atmp);
    
    const hashmap* operator[](const DState& name) const; 
    DAction* argmax(const DState& name) const;
    DAction* argmax() const ;
    
    double operator()(const DState& s, const DAction& a) const;
    double& operator()(const DState& s, const DAction& a);
    
    double operator()(const DAction& a) const;
    double& operator()(const DAction& a);
    
    double operator()(unsigned int s, unsigned int a) const;
    double& operator()(unsigned int s, unsigned int a);

    void write(const string& chemin);
    void read(const string& chemin);
    
    hashmap* getWholeCouple();

private:
    hashmap *map = nullptr;
    //TODO: could be slightly improve argmax performance by using multiset and hashmap together
    
    const StateTemplate* stmpl;
    const ActionTemplate* atmpl;
};



}

#endif
