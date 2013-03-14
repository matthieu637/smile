#ifndef TWORLD_HPP
#define TWORLD_HPP

#define STK "stuck"
#define AGL "angle"
#define DST "distanceFromMiddle"
#define LDST "leftDistance"
#define RDST "rightDistance"
#define SPD "speed"

#define ACC "acceleration"
#define DIR "direction"

class Driver;

struct State {
    bool stuck;  // 0 1
    float angle; // -pi / pi
    float distanceFromMiddle; // -20 / 20
    float leftDistance;
    float rightDistance;
    float speed; //-40 / 40 ?
};


class TWorld
{

public:
    static State* observe(const Driver& d);
    static double reward(const Driver& d);
    static unsigned int discretizeAngle(float angle, unsigned int cardinal);
    static unsigned int discretizeDistance(float distance, unsigned int cardinal, double dismin, double dismax);

    static float computeSteering(unsigned int discetized, unsigned int cardinal, double smin, double smax);
};

#endif // TWORLD_HPP
