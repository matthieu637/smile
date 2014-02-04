#include "TFeatures.hpp"
#include <sml/Utils.hpp>
#include <boost/bind.hpp>

using sml::Utils;

// Functor1D::Functor1D(TFeatures::f1D type, double road_width) : t(type) ,max(road_width) {}
// 
// Functor1D::Functor1D(const string& action) : t(TFeatures::f1D::action), action(action) {}
// 
// double Functor1D::call(const State& st, const DAction& ac)
// {
//     switch(t) {
//     case TFeatures::f1D::distanceFromMiddle:
//         return sml::Utils::transform(st.distanceFromMiddle, -max/2., max/2., 0., max);
//     case TFeatures::f1D::angle:
//         return sml::Utils::transform(st.angle, -max/2., max/2., 0., max);
//     case TFeatures::f1D::speed:
//         return sml::Utils::transform(st.speed, -max/5., 4.*max/5., 0., max);
//     case TFeatures::f1D::left:
//         return sml::Utils::transform(st.leftDistance, -2*max/5., 3.*max/5., 0., max);
//     case TFeatures::f1D::right:
//         return sml::Utils::transform(st.rightDistance, -2*max/5., 3.*max/5., 0., max);
//     case TFeatures::f1D::straight:
//         return sml::Utils::transform(st.straightLength, 0., max, 0., max);
//     case TFeatures::f1D::nextArc:
//         return sml::Utils::transform(st.nextArc, -max/2., max/2., 0., max);
//     case TFeatures::f1D::action:
//         return ac[action];
//     }
// 
//     return -1;
// }
// 
// Feature<State>::featuring1D TFeatures::_1D(TFeatures::f1D type, double max) {
//     Feature<State>::featuring1D fonctor;
//     Functor1D inst( type , max);
//     fonctor = boost::bind(&Functor1D::call, inst , _1, _2);
// 
//     return fonctor;
// }
// 
// Feature<State>::featuring1D TFeatures::_1DMiddle(double road_width) {
//     return _1D(distanceFromMiddle , road_width);
// }
// 
// Feature<State>::featuring1D TFeatures::_1DAngle(double angle_total) {
//     return _1D(angle , angle_total);
// }
// 
// Feature<State>::featuring1D TFeatures::_1DSpeed(double total_speed) {
//     return _1D(speed , total_speed);
// }
// Feature<State>::featuring1D TFeatures::_1DLeft(double road_width) {
//     return _1D(left , road_width);
// }
// 
// Feature<State>::featuring1D TFeatures::_1DRight(double road_width) {
//     return _1D(right , road_width);
// }
// 
// Feature<State>::featuring1D TFeatures::_1DStraight(double length) {
//     return _1D(straight , length);
// }
// 
// Feature<State>::featuring1D TFeatures::_1DNextArc(double length) {
//     return _1D(nextArc , length);
// }
// 
// Feature<State>::featuring1D TFeatures::_1DAction(const string& acc) {
//     Feature<State>::featuring1D fonctor;
//     Functor1D inst( acc );
//     fonctor = boost::bind(&Functor1D::call, inst , _1, _2);
// 
//     return fonctor;
// }


