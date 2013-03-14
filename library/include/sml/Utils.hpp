#ifndef UTILS_HPP
#define UTILS_HPP

namespace sml{

class Utils
{
public : 
  static float rand01();
  static double abs(double x);
  static double transform(double x, double a, double b, double c, double d);
};

}

#endif // UTILS_HPP
