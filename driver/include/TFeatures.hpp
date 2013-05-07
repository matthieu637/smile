
#ifndef TFEATURES_H
#define TFEATURES_H

#include <sml/Feature.hpp>
#include "TWorld.hpp"

using sml::Feature;

class TFeatures
{
public:
  enum f1D { angle, distanceFromMiddle, speed, action};
  
  static Feature<State>::featuring1D _1DMiddle(double road_width);
  static Feature<State>::featuring1D _1DAngle(double angle_total);
  static Feature<State>::featuring1D _1DSpeed(double total_speed);
  static Feature<State>::featuring1D _1DAction(const string& acc);
};

class Functor1D
{
public:
    Functor1D(TFeatures::f1D type, double road_width);
    Functor1D(const string& action);

    double call(const State& st, const DAction& ac);

private:
    TFeatures::f1D t;
    double max;
    string action;
};

#endif // TFEATURES_H
