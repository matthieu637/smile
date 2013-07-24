#include "simu/GridWorld.hpp"
#include "bib/Logger.hpp"

namespace simu {

//-1 means clean way
//0 means wall
//-10 malus
//10/50 goals
const int GridWorld::REWARD[XMAX][YMAX]= {
    { -1, -1, -1, 10, -1,  0,  -1, -1, -1, -10},
    { -1, -1, -1,  0, -1,  0,  -1, -1, -1, 0  },
    {  0,  0, -1,  0,  0, 10,  -1, -1,  0, 10 },
    { 10,  0, -1,  0, -1,  0,   0, -1, -1, -1 },
    { -1,  0, -1, -1, -1,  0, -20,  0,  0, -1 },
    { -1,  0, -1, -1, -1,  0,  -1,  0, -1, 50 },
    { -1,  0, 10, -1, -1,  0,  -1,  0, -1, 0  },
    { -1, -1, -1, -1, -1,  0,  -1,  0, -1, 0  },
    { -1,  0, -1,  0,  0,  0,  -1,  0, -1, 0  },
    {-20,  0, -1, -1, -1, -1,  -1, -1, -1, 0  }
};

GridWorld::GridWorld() : Environnement< GridWorldState >(new StateTemplate( {XPOS, YPOS, PAS}, {XMAX, YMAX, PMAX}), new ActionTemplate( {MOV}, {4}))
{
    init();
}

void GridWorld::initState() {
    keepGoals = GOALS;
    for(int i=0; i<XMAX; i++)
        for(int j=0; j<YMAX; j++) {
            world[i][j]=REWARD[i][j];
            passed[i][j]= 0;
        }

    state->x = 0;
    state->y = 0;
    state->p = 0;
    passed[0][0] = 1;
    forcedWall = false;
}

void GridWorld::applyOn(const DAction& ac)
{
    // if we are on a goal, we eat it
    if(reward() > 0) {
        world[state->x][state->y] = -1;
        keepGoals--;
    }

    passed[state->x][state->y] ++;

    int direction = ac[MOV];
    bool changed = false;
    switch(direction) {
    case 0:
        if(accessible(state->x, state->y - 1)) {
            state->y --;
            changed = true;
        }
        break;
    case 1:
        if(accessible(state->x, state->y + 1)) {
            state->y ++;
            changed = true;
        }
        break;
    case 2:
        if(accessible(state->x - 1, state->y)) {
            state->x --;
            changed = true;
        }
        break;
    case 3:
        if(accessible(state->x + 1, state->y)) {
            state->x ++;
            changed = true;
        }
        break;
    }

    forcedWall= !changed;

    state->p = passed[state->x][state->y];
}

bool GridWorld::accessible(int x, int y) const {
    if(x < 0 || x >= XMAX || y < 0 || y >= YMAX)
        return false;

    return REWARD[x][y] != 0;
}

double GridWorld::reward() const {
    if(forcedWall) {
// 	LOG_DEBUG("bipped");
        return -5;
    }
    return world[state->x][state->y];
}

DAction* GridWorld::getInitialAction() const {
    return new DAction(getActions(), 0);
}

bool GridWorld::goal() const {
    return keepGoals <= 0;
}

unsigned int GridWorld::maxStep() const {
    return 5000;
}

void GridWorld::computeDState(const GridWorldState& s, DState* dst, const ActionTemplate* repr) {
    (void) repr;
    dst->set(XPOS, s.x);
    dst->set(YPOS, s.y);
    dst->set(PAS, s.p >= PMAX ? PMAX - 1 : s.p);
//     LOG_DEBUG(*dst);
}

}
