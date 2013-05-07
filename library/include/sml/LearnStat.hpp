#ifndef LEARNSTAT_H
#define LEARNSTAT_H

#include <bib/XMLEngine.hpp>
#include <string>
using std::string;

namespace sml {

class LearnStat
{
public:
    void write(const string& chemin);
    void read(const string& chemin);

protected:
    virtual void save(boost::archive::xml_oarchive* xml) = 0;
    virtual void load(boost::archive::xml_iarchive* xml) = 0;

    int performance;
    int episode;
};

}

#endif // LEARNSTAT_H
