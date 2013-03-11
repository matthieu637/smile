#include "sml/Utils.hpp"

// #include <stdlib.h>
#include <random>

namespace sml {

float Utils::rand01() {
    return (float)rand()/(float)RAND_MAX;
}

double Utils::abs(double x){
    if(x > 0)
      return x;
    else return - x;
}

}