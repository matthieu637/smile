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

struct GridWorldLSState {
    int x;
    int y;
    int p;
};

class GridWorldLS : public Environnement<GridWorldLSState>
{
public:
    GridWorldLS();

public:
    DAction* getInitialAction() const;
    double reward() const;
    bool goal() const;
    unsigned int maxStep() const;
    void computeDState(const GridWorldLSState& s, DState* dst, const StateTemplate* repr);
protected:
    void applyOn(const DAction& ac);
    void initState(bool random=false);

    bool accessible(int x, int y) const;

private:
    static const int REWARD[XMAX][YMAX];
    static const int GOALS = 6;

    int world[XMAX][YMAX];
    int passed[XMAX][YMAX];
    int keepGoals;
    int last_reward;
};

}

#endif // WUMPUS_H

