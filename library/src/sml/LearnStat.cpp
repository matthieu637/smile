#include "sml/LearnStat.hpp"
#include <sml/Utils.hpp>
#include "bib/Logger.hpp"
#include <boost/filesystem.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>


using namespace boost::interprocess;

namespace sml {

LearnStat::LearnStat(const LearnConfig& conf):performance(0), last_performance(0), episode(0), performance_set(false), read_before(false), conf(conf) {}

void LearnStat::write(const string& chemin) {

    named_mutex mutex( open_or_create, chemin.c_str());

    if(performance_set && conf.restrictive_learning && read_before) {
        float limit = last_performance - conf.ratio_limit*sml::Utils::abs(last_performance) - conf.const_limit;
        if( performance < limit )
        {
//             mutex.unlock();
            return;
        }
    }

    LOG_INFO("Enregistrement du fichier XML " << chemin);
    mutex.lock();

    ofstream outputFile(chemin);
    assert(outputFile.good());
    xml_oarchive xml(outputFile);
    xml << make_nvp("perf", performance);
    xml << make_nvp("episode", episode);

    save(&xml);

    outputFile.close();
    mutex.unlock();
}

void LearnStat::read(const string& chemin) {
    if(  !boost::filesystem::exists( chemin ) ) {
        LOG_DEBUG(chemin << " n'existe pas.");
    }
    else {
        named_mutex mutex( open_or_create, chemin.c_str());
        mutex.lock();

        ifstream inputFile(chemin);
        assert(inputFile.good());
        xml_iarchive xml(inputFile);
        xml >> make_nvp("performance", performance);
        xml >> make_nvp("episode", episode);
        load(&xml);
        inputFile.close();
        read_before=true;
	 mutex.unlock();
    }
//     LOG_DEBUG("rb " << read_before);
}

void LearnStat::setPerf(double globalReward) {
    last_performance = performance;
    performance = globalReward;
    performance_set=true;
}

void LearnStat::increaseEpisode() {
    episode++;
}

bool LearnStat::keepLearning(int perf) {
    if(conf.restrictive_learning && read_before) {
        float ratio = (float)perf/(float)performance;
        float limit = 1.2;
        if((perf < 0 && performance < 0 && ratio > limit) ||
                (perf > 0 && performance > 0 && ratio < limit)
          ) {
            LOG_DEBUG("breaked! too bad perf (" << perf <<") << (" << performance << ")" );
            return false;
        }
    }

    return true;
}

}
