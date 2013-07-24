#ifndef WUMPUS_H
#define WUMPUS_H

#include "simu/Environnement.hpp"

namespace simu {

#define XPOS "XPOS"
#define YPOS "YPOS"
#define PAS "passed"
  
#define MOV "move"

#define XMAX 10
#define YMAX 10
#define PMAX 2

struct GridWorldState {
    int x;
    int y;
    int p;
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
    void initState();
    
    bool accessible(int x, int y) const;

private:
    static const int REWARD[XMAX][YMAX];
    static const int GOALS = 6;
    
    int world[XMAX][YMAX];
    int passed[XMAX][YMAX];
    int keepGoals;
    bool forcedWall;
};

}

#endif // WUMPUS_H

