#ifndef FEATURE_HPP
#define FEATURE_HPP

#include "sml/Action.hpp"
#include "bib/Logger.hpp"

namespace sml {

template<typename S>
class Feature
{
public:
    typedef double (*featuring) (const S&, std::vector<double> params);

    Feature(featuring f, const std::vector<double> &params):f(f), params(params) {}

    double calc(const S& st) {
        return f(st, params);
    }

private:
    featuring f;
    std::vector<double> params;
};

}

#endif // FEATURE_HPP
