#ifndef LEARNSTAT_H
#define LEARNSTAT_H

#include <bib/XMLEngine.hpp>
#include <string>
using std::string;

namespace sml {
  
struct LearnConfig{
    bool restrictive_learning;
    float ratio_limit;
    float const_limit;
};

class LearnStat
{
public:
    LearnStat(const LearnConfig& conf);
    void write(const string& chemin);
    void read(const string& chemin);
    void setPerf(double globalReward);
    void increaseEpisode();
    bool keepLearning(int perf);

protected:
    virtual void save(boost::archive::xml_oarchive* xml) = 0;
    virtual void load(boost::archive::xml_iarchive* xml) = 0;

    int performance;
    int last_performance;
    int episode;
    
    bool performance_set;
    bool read_before;
    
    LearnConfig conf;
};

}

#endif // LEARNSTAT_H
