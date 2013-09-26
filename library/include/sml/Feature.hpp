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
        f(f), params(params), t(t) {
        LOG_ERROR("not implemented");
    }

///
///\brief Constructeur pour créer une couche regulière : un tiling en dimension quelconque
///\param featuresl : liste des fonctions pour chaque dimension
/// 	  params : paramètre pour la fonction
    Feature(const vector<featuring1D>& featuresl, const std::vector<double> &params):
        featuresl(featuresl), params(params), t(_ND), maxi(featuresl.size()), total(featuresl.size()), width(featuresl.size()), size(1)
    {
        assert(params.size()==featuresl.size()*2);

        int index = 0;
        for(int i=0; i< params.size(); i+=2) {
            maxi[index] = params[i] + 2; // add external 
            total[index] = params[i+1];
            width[index] = total[index]/(maxi[index] - 2);
            size *= maxi[index];
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
        vector<int> var(featuresl.size());

        for(int index=0; index < featuresl.size(); index++) {
// 	    LOG_DEBUG(featuresl[index](st, ac) << " " << (int) floor( featuresl[index](st, ac) / width[index]) << " " <<  maxi[index] << " " << width[index] << " " << index);
            var[index] = (int) floor( featuresl[index](st, ac) / width[index]);
	    var[index] ++; //for left depassement
            if(var[index] < 0 || var[index] >= maxi[index])
                return -1;
        }

        int calc=0;
        for(int i=featuresl.size() -1 ; i >= 0; i--) {

            int factor = var[i];
            for(int j= i - 1; j >= 0; j--) {
                factor *= maxi[j];
            }
            calc += factor ;
        }
        return calc;
    }

    /*
        int case4D(const S& st, const DAction& ac) {
            int xmaxi = params[0];
            double xtotal = params[1];
            int ymaxi = params[2];
            double ytotal = params[3];
            int zmaxi = params[4];
            double ztotal = params[5];
            int umaxi = params[6];
            double utotal = params[7];

            double x = f1d(st, ac);
            double y = f1d2(st, ac);
            double z = f1d3(st, ac);
            double u = f1d4(st, ac);

            double xwidth = xtotal/xmaxi;
            double ywidth = ytotal/ymaxi;
            double zwidth = ztotal/zmaxi;
            double uwidth = utotal/umaxi;

            int X = floor(x/xwidth);
            int Y = floor(y/ywidth);
            int Z = floor(z/zwidth);
            int U = floor(u/uwidth);

            if(X < 0 || X >= xmaxi || Y < 0 || Y >= ymaxi || Z < 0 || Z >= zmaxi || U < 0 || U >= umaxi)
                return -1;

            return U*zmaxi*ymaxi*xmaxi + Z*ymaxi*xmaxi + Y*xmaxi + X;
        }*/

private:
    featuring f;
    featuring1D f1d;
    vector<featuring1D> featuresl;
    vector<double> params;
    type t;

//     vector<int> var;
    vector<int> maxi;
    vector<double> total;
    vector<double> width;

    int size;
};

template<typename State>
using featuredList = std::list< Feature<State>* > ;

template<typename State>
using fLiterator = typename std::list< Feature<State>* >::iterator ;

}

#endif // FEATURE_HPP

