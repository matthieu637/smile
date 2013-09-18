#ifndef GRIDWORLDFULL_H
#define GRIDWORLDFULL_H

#include "simu/Environnement.hpp"

namespace simu {

#define XPOS "XPOS"
#define YPOS "YPOS"
#define XG "XGOAL"
#define YG "YGOAL"
#define GL "GOAL"
  
#define MOV "move"

#define XMAX 10
#define YMAX 10
#define GMAX 5

struct GridWorldState {
    int x;
    int y;
    int xgoal;
    int ygoal;
    int currentGoal;
};

class GridWorld : public Environnement<GridWorldState>
{
public:
    GridWorld();

public:
    DAction* getInitialAction() const;
    double reward() const;
    bool goal() const;
    unsigned int maxStep() const;
    void computeDState(const GridWorldState& s, DState* dst, const StateTemplate* repr);
protected:
    void applyOn(const DAction& ac);
    void initState(bool random=false);

    bool accessible(int x, int y) const;

public:
// private:
    static const int REWARD[XMAX][YMAX];
    static const int NBGOALS = GMAX;
    static const int XGOALS [NBGOALS];
    static const int YGOALS [NBGOALS];

    int world[XMAX][YMAX];
    int last_reward;
};

}

#endif // WUMPUS_H

