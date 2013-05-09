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


};

}

#endif // UTILS_HPP
