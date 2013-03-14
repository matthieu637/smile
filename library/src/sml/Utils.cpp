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

// a < x < b => c < X < d
double Utils::transform(double x, double a, double b, double c, double d){
  if(x < a)
    x=a;
  else if (x > b)
    x=b;
  
  return c + ((x - a)/(b - a))*(d - c);
}

}