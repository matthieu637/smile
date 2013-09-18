#ifndef UTILS_HPP
#define UTILS_HPP

///
///\file Utils.hpp
///\brief les méthodes utiles
///
///

#include "bib/Logger.hpp"

namespace sml {

class Utils
{
public :
 
///
///\brief Retourner 0 ou 1
    static float rand01();
    
    static bool rand01(float);
    
    static float randin(float a, float b);
///
///\brief Retourner la valeur absolue de "x"
///\param x : une valeur
    static double abs(double x);
    
///
///\brief transformer la valeur x qui appartient [a,b] à [c,d]
///\param x :une valeur
///	  a,b : intervalle [a,b]
///	  c,d: intervalle [c,d]
    static double transform(double x, double a, double b, double c, double d);

    static time_t srand_mili(bool zero=false);
    
    static float* genNrand(int N, float max);

};

}

#endif // UTILS_HPP
