#include "sml/Utils.hpp"
#include <sys/time.h>
// #include <stdlib.h>
#include <random>

namespace sml {

float Utils::rand01() {
    return (float)rand()/(float)RAND_MAX;
}

double Utils::abs(double x) {
    if(x > 0)
        return x;
    else return - x;
}

// a < x < b => c < X < d
double Utils::transform(double x, double a, double b, double c, double d) {
    if(x < a)
        x=a;
    else if (x > b)
        x=b;

    return c + ((x - a)/(b - a))*(d - c);
}

time_t Utils::srand_mili(bool zero) {
    if(zero){
        srand(0);
	return 0;
    }
    else {
        timeval t1;
        gettimeofday(&t1, NULL);
        srand(t1.tv_usec * t1.tv_sec);
	return t1.tv_usec * t1.tv_sec; 
    }
}

}
