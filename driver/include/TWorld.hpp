#ifndef TWORLD_HPP
#define TWORLD_HPP

class Driver;

struct State {
    float alpha; //direction
    float distance;
};


class TWorld
{

public:
    static double reward(const Driver& d);
    static unsigned int discretizeAngle(float angle, unsigned int cardinal);
    static unsigned int discretizeDistanceFromMiddle(float distance, unsigned int cardinal, double dismin, double dismax);

    static float computeSteering(unsigned int discetized, unsigned int cardinal, double smin, double smax);
};

#endif // TWORLD_HPP
