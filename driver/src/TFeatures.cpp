#include "TFeatures.hpp"
#include <sml/Utils.hpp>
#include <boost/bind.hpp>

using sml::Utils;

Functor1D::Functor1D(TFeatures::f1D type, double road_width) : t(type) ,max(road_width) {}

Functor1D::Functor1D(const string& action) : t(TFeatures::f1D::action), action(action) {}

double Functor1D::call(const State& st, const DAction& ac)
{
    switch(t) {
    case TFeatures::f1D::distanceFromMiddle:
        return sml::Utils::transform(st.distanceFromMiddle, -max/2., max/2., 0., max);
    case TFeatures::f1D::angle:
        return sml::Utils::transform(st.angle, -max/2., max/2., 0., max);
    case TFeatures::f1D::speed:
        return sml::Utils::transform(st.speed, -max/4., 3.*max/4., 0., max);
    case TFeatures::f1D::action:
        return ac[action];
    }

    return -1;
}

Feature<State>::featuring1D TFeatures::_1DMiddle(double road_width) {
    Feature<State>::featuring1D fonctor;
//     Functor1D* inst = new Functor1D( distanceFromMiddle , road_width);
    Functor1D inst( distanceFromMiddle , road_width);
    fonctor = boost::bind(&Functor1D::call, inst , _1, _2);

    return fonctor;
}

Feature<State>::featuring1D TFeatures::_1DAngle(double angle_total) {
    Feature<State>::featuring1D fonctor;
    Functor1D inst( angle , angle_total);
    fonctor = boost::bind(&Functor1D::call, inst , _1, _2);

    return fonctor;
}

Feature<State>::featuring1D TFeatures::_1DSpeed(double total_speed) {
    Feature<State>::featuring1D fonctor;
    Functor1D inst( speed , total_speed);
    fonctor = boost::bind(&Functor1D::call, inst , _1, _2);

    return fonctor;
}

Feature<State>::featuring1D TFeatures::_1DAction(const string& acc) {
    Feature<State>::featuring1D fonctor;
    Functor1D inst( acc );
    fonctor = boost::bind(&Functor1D::call, inst , _1, _2);

    return fonctor;
}
