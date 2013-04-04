#ifndef TWORLD_HPP
#define TWORLD_HPP

#define STK "stuck"
#define AGL "angle"
#define DST "distanceFromMiddle"
#define LDST "leftDistance"
#define RDST "rightDistance"
#define SPD "speed"

#define ACC "acceleration"
#define DIRE "direction"

#include <sml/Action.hpp>
#include "car.h"

class Driver;
using sml::DAction;
using sml::ActionTemplate;

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
    static DAction* initialAction(const sml::ActionTemplate* atmp); 
    static State* initialState();
    static State* observe(const Driver& d);
    static double reward(const Driver& d);
    static unsigned int discretizeAngle(float angle, unsigned int cardinal);
    static unsigned int discretizeDistance(float distance, unsigned int cardinal, double dismin, double dismax);

    static float computeSteering(unsigned int discetized, unsigned int cardinal, double smin, double smax);
    static void applyAcceleration(tCarElt* car, int acc);
    
    static const int ACTIONS_ACC = 4;
};

#endif // TWORLD_HPP
