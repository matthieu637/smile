#ifndef FEATURE_HPP
#define FEATURE_HPP

///
///\file feature.hpp
///\brief Représente une couche pour les fonctions d'approximations (par exemple un tiling)
//

#include <boost/function.hpp>
#include "sml/Action.hpp"
#include "bib/Logger.hpp"

using std::pair;
using std::vector;

namespace sml {

template<typename S>
class Feature
{
    enum type { _ND, custom };

public:
    typedef int (*featuring) (const S&, const DAction&, std::vector<double> params);
    typedef boost::function<double (const S&, const DAction&)> featuring1D;

///
///\brief Constructeur pour créer une couche complexe (RBF, log, ...)
///\param f : une fonction
/// 	  params : paramètre pour la fonction
    Feature(featuring f, const std::vector<double> &params, type t=custom):
        f(f), params(params), t(t) {}

///
///\brief Constructeur pour créer une couche regulière : un tiling en dimension quelconque
///\param featuresl : liste des fonctions pour chaque dimension
/// 	  params : paramètre pour la fonction
    Feature(const vector<featuring1D>& featuresl, const std::vector<double> &params):
        featuresl(featuresl), params(params), t(_ND), var(featuresl.size()), max(featuresl.size()), total(featuresl.size()), width(featuresl.size()), size(1)
    {
        assert(params.size()==featuresl.size()*2);

        int index = 0;
        for(int i=0; i< params.size(); i+=2) {
            max[index] = params[i];
            total[index] = params[i+1];
            width[index] = total[index]/max[index];
            size *= max[index];
            index++;
        }
    }

///
///\brief Retourner le taille total de dimensions
    int getSize() {
        return size;
    }

///
///\brief Déterminer l'indice activé pour l'état et l'action pour un tiling
///\param st : l'état donné
/// 	  at : l'action donnée
    int calc(const S& st, const DAction& ac) {
        switch(t)
        {
        case _ND:
            return caseND(st, ac);
        case custom:
            return f(st, ac, params);
        }
        return -1;
    }

///
///\brief Déterminer l'indice activé pour l'état et l'action
///\param st : l'état donné
/// 	  at : l'action donnée
    int caseND(const S& st, const DAction& ac) {

        for(int index=0; index < featuresl.size(); index++) {
// 	    LOG_DEBUG(featuresl[index](st, ac) << " " << floor( featuresl[index](st, ac) / width[index]) << " " <<  max[index] << " " << width[index] << " " << index);
            var[index] = floor( featuresl[index](st, ac) / width[index]);
            if(var[index] < 0 || var[index] >= max[index])
                return -1;
        }

        int calc=0;
        for(int i=featuresl.size() -1 ; i >= 0; i--) {

            int factor = var[i];
            for(int j= i - 1; j >= 0; j--) {
                factor *= max[j];
            }
            calc += factor ;
        }
        return calc;
    }

    /*
        int case4D(const S& st, const DAction& ac) {
            int xmax = params[0];
            double xtotal = params[1];
            int ymax = params[2];
            double ytotal = params[3];
            int zmax = params[4];
            double ztotal = params[5];
            int umax = params[6];
            double utotal = params[7];

            double x = f1d(st, ac);
            double y = f1d2(st, ac);
            double z = f1d3(st, ac);
            double u = f1d4(st, ac);

            double xwidth = xtotal/xmax;
            double ywidth = ytotal/ymax;
            double zwidth = ztotal/zmax;
            double uwidth = utotal/umax;

            int X = floor(x/xwidth);
            int Y = floor(y/ywidth);
            int Z = floor(z/zwidth);
            int U = floor(u/uwidth);

            if(X < 0 || X >= xmax || Y < 0 || Y >= ymax || Z < 0 || Z >= zmax || U < 0 || U >= umax)
                return -1;

            return U*zmax*ymax*xmax + Z*ymax*xmax + Y*xmax + X;
        }*/

private:
    featuring f;
    featuring1D f1d;
    vector<featuring1D> featuresl;
    vector<double> params;
    type t;

    vector<int> var;
    vector<int> max;
    vector<double> total;
    vector<double> width;

    int size;
};

template<typename State>
using featuredList = std::list< Feature<State> > ;

template<typename State>
using fLiterator = typename std::list< Feature<State> >::iterator ;

}

#endif // FEATURE_HPP
