#include "sml/LearnStat.hpp"
#include "bib/Logger.hpp"
#include <boost/filesystem.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>


using namespace boost::interprocess;

namespace sml {

void LearnStat::write(const string& chemin) {
    named_mutex mutex( open_or_create, chemin.c_str());

    LOG_INFO("Enregistrement du fichier XML " << chemin);

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
    }
}

}
