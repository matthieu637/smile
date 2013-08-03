#include "sml/Utils.hpp"
#include <sys/time.h>
// #include <stdlib.h>
#include <random>
#include <algorithm>

namespace sml {

float Utils::rand01() {
    return (float)rand()/(float)RAND_MAX;
}

float Utils::randin(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
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


float* Utils::genNrand(int N, float max){
    float* tab = new float[N];
    tab[0] = 0.;
    for(int i=1;i<N;i++)
      tab[i] = rand01() * max;
    
    std::sort(tab, tab + N, std::less<float>());
    return tab;
}


}
