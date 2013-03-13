
#include "sml/Q.hpp"
#include "bib/Logger.hpp"

#define ACC "acceleration"
#define DIR "direction"
#define AGL "angle"
#define DST "distanceFromMiddle"
#define LDST "leftDistance"
#define RDST "rightDistance"
#define SPD "speed"



#include <fstream>
#include <string>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include "bib/Logger.hpp"

using std::ofstream;
using std::ifstream;
using std::string;
using boost::archive::xml_oarchive;
using boost::archive::xml_iarchive;
using boost::serialization::make_nvp;


int STATES_ALPHA = 2;
int STATES_DISTANCE = 2;
int ACTIONS_ACC = 2;
int ACTIONS_DIRECTION = 2;

using namespace sml;
using namespace bib;

int main() {
    sml::ActionTemplate ACTION_TEMPLATE( {ACC, DIR}, {ACTIONS_ACC, ACTIONS_DIRECTION});
    sml::StateTemplate STATE_TEMPLATE( {AGL, DST}, {STATES_ALPHA, STATES_DISTANCE});

    QTable Q(&ACTION_TEMPLATE, &STATE_TEMPLATE);
    DAction a(&ACTION_TEMPLATE, {1,1});
    LOG_DEBUG(a.hash());
    DAction b(&ACTION_TEMPLATE, a.hash()-1);
    LOG_DEBUG(b.get(0) << " " << b.get(1));
    LOG_DEBUG(b.hash());
    Q(a, b) = 180.1;
    Q(a, a) = 0.278965421;
    LOG_DEBUG(Q(a, b));
    LOG_DEBUG(Q(a, DAction(&ACTION_TEMPLATE, {0,0})));
    LOG_DEBUG(Q(a, DAction(&ACTION_TEMPLATE, {0,1})));
    LOG_DEBUG("argmax " << Q.argmax(a)->get(0) << " " << Q.argmax(a)->get(1));

    Q.write("test.xml");
    Q.read("test.xml");
    
    LOG_DEBUG(Q(a, b));
    LOG_DEBUG(Q(a, a));
    LOG_DEBUG("argmax " << Q.argmax(a)->get(0) << " " << Q.argmax(a)->get(1));
    
    return 0;
}
