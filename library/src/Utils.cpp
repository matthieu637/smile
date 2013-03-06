#include "sml/Utils.hpp"

#include <stdlib.h>
//#include <random>

namespace sml {

float Utils::rand01() {
    return (float)rand()/(float)RAND_MAX;
}

}