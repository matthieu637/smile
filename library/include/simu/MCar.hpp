#ifndef MCAR_HPP
#define MCAR_HPP

#include "simu/Environnement.hpp"

///
///\file MCar.hpp
///\brief Simulation du MontainCar
///
///


using namespace std;
using namespace sml;

namespace simu {

struct MCarState {
    float position;
    float velocity;
};

// bool operator<(const MCarState& s1, const MCarState& s2){
//     if( s1.position < s2.position)
//       return true;
//     
//     return s1.velocity < s2.velocity;
// }

class MCar : public Environnement<MCarState> {

#define POS "position"
#define VEL "velocity"
#define MOT "motor"

#define mcar_min_position -1.2
#define mcar_max_position 0.6
#define mcar_max_velocity 0.07            // the negative of this is also the minimum velocity
#define mcar_goal_position 0.5

public:
    MCar(int nbPosStep, int nbVelStep);


    double reward() const;
    DAction* getInitialAction() const;
    bool goal() const;
    unsigned int maxStep() const;
    
protected:
    void applyOn(const DAction& ac);
    void computeDState(const MCarState& s, DState* dst, const StateTemplate* repr);
    void initState(bool random=false);
};

}


#endif
