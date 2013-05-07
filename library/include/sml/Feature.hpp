#ifndef FEATURE_HPP
#define FEATURE_HPP

#include <boost/function.hpp>
#include "sml/Action.hpp"
#include "bib/Logger.hpp"

using std::pair;

namespace sml {

template<typename S>
class Feature
{
    enum type { _1D, _2D, _3D, _4D, custom };

public:
    typedef int (*featuring) (const S&, const DAction&, std::vector<double> params);
    typedef boost::function<double (const S&, const DAction&)> featuring1D;

    Feature(featuring f, const std::vector<double> &params, type t=custom):
        f(f), params(params), t(t) {}

    Feature(featuring1D f, const std::vector<double> &params) :
        f1d(f), params(params), t(_1D) {
        assert(params.size()==2);
    }

    Feature(featuring1D f1, featuring1D f2, const std::vector<double> &params) :
        f1d(f1), f1d2(f2), params(params), t(_2D) {
        assert(params.size()==4);
    }

    Feature(featuring1D f1, featuring1D f2, featuring1D f3, const std::vector<double> &params) :
        f1d(f1), f1d2(f2), f1d3(f3), params(params), t(_3D) {
        assert(params.size()==6);
    }

    Feature(featuring1D f1, featuring1D f2, featuring1D f3, featuring1D f4, const std::vector<double> &params) :
        f1d(f1), f1d2(f2), f1d3(f3), f1d4(f4), params(params), t(_3D) {
        assert(params.size()==8);
    }


    int calc(const S& st, const DAction& ac) {
        switch(t)
        {
        case _1D:
            return case1D(st, ac);
        case _2D:
            return case2D(st, ac);
        case _3D:
            return case3D(st, ac);
        case _4D:
            return case4D(st, ac);
        case custom:
            return f(st, ac, params);
        }
        return -1;
    }

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
    }

    int case3D(const S& st, const DAction& ac) {
        int xmax = params[0];
        double xtotal = params[1];

        int ymax = params[2];
        double ytotal = params[3];

        int zmax = params[4];
        double ztotal = params[5];

        double x = f1d(st, ac);
        double y = f1d2(st, ac);
        double z = f1d3(st, ac);

        double xwidth = xtotal/xmax;
        double ywidth = ytotal/ymax;
        double zwidth = ztotal/zmax;

        int X = floor(x/xwidth);
        int Y = floor(y/ywidth);
        int Z = floor(z/zwidth);

        if(X < 0 || X >= xmax || Y < 0 || Y >= ymax || Z < 0 || Z >= zmax)
            return -1;

        return Z*ymax*xmax + Y*xmax + X;
    }

    int case2D(const S& st, const DAction& ac) {
        int xmax = params[0];
        double xtotal = params[1];

        int ymax = params[2];
        double ytotal = params[3];

        double x = f1d(st, ac);
        double y = f1d2(st, ac);

        double xwidth = xtotal/xmax;
        double ywidth = ytotal/ymax;

        int X = floor(x/xwidth);
        int Y = floor(y/ywidth);

        if(X < 0 || X >= xmax || Y < 0 || Y >= ymax)
            return -1;

        return X*ymax + Y;
    }

    int case1D(const S& st, const DAction& ac) {
        int xmax = params[0];
        double xtotal = params[1];
        double x = f1d(st, ac);

        double xwidth = xtotal/xmax;

        int X = floor(x/xwidth);

        if(X < 0 || X >= xmax)
            return -1;

        return X;
    }

private:
    featuring f;
    featuring1D f1d;
    featuring1D f1d2;
    featuring1D f1d3;
    featuring1D f1d4;
    std::vector<double> params;
    type t;
};

}

#endif // FEATURE_HPP
