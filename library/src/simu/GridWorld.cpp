#include "simu/GridWorld.hpp"
#include "bib/Logger.hpp"

namespace simu {

//-1 means clean way
//0 means wall
//-10 malus
//10/50 goals
const int GridWorld::REWARD[XMAX][YMAX]= {
    { -1, -1, -1, -1, -1,  0,  -1, -1, -1, 50},
    { -1, -1, -1,  0, 10,  0,  -1, -1, -1, 0  },
    {  0,  0, -1,  0,  0, 10,  -1, -1,  0, -10 },
    { 10,  0, -1,  0, -1,  0,   0, -1, -1, -1 },
    { -1,  0, -1, -1, -1,  0, -20,  0,  0, -1 },
    { -1,  0, -1, -1, -1,  0,  -1,  0, -1, -1 },
    { -1,  0, 10, -1, -1,  0,  -1,  0, -1, 0  },
    { -1, -1, -1, -1, -1,  0,  -1,  0, -1, 0  },
    { -1,  0, -1,  0,  0,  0,  -1,  0, -1, 0  },
    {-20,  0, -1, -1, -1, -1,  -1, -1, -1, 0  }
};

const int GridWorld::XGOALS[NBGOALS] = {3, 1, 2, 6, 0};
const int GridWorld::YGOALS[NBGOALS] = {0, 4, 4, 2, 9};

GridWorld::GridWorld() : Environnement< GridWorldState >(new StateTemplate( {XPOS, YPOS, XG, YG}, {XMAX, YMAX, XMAX, YMAX}), new ActionTemplate( {MOV}, {4}))
{
    init();
}

void GridWorld::initState(bool) {
    currentGoal = 0;
    for(int i=0; i<XMAX; i++)
        for(int j=0; j<YMAX; j++)
            world[i][j]=REWARD[i][j];

    state->x = 0;
    state->y = 0;
    state->xgoal = XGOALS[currentGoal];
    state->ygoal = YGOALS[currentGoal];
    last_reward = -1;
}

void GridWorld::applyOn(const DAction& ac)
{
    // if we are on a goal, we eat it
    if(last_reward > 0) {
        world[state->x][state->y] = -1;
    }

    int direction = ac[MOV];
    switch(direction) {
    case 0:
        if(accessible(state->x, state->y - 1))
            state->y --;
        break;
    case 1:
        if(accessible(state->x, state->y + 1))
            state->y ++;
        break;
    case 2:
        if(accessible(state->x - 1, state->y))
            state->x --;
        break;
    case 3:
        if(accessible(state->x + 1, state->y))
            state->x ++;
        break;
    }
    
    if(reward() > 0) {
        currentGoal++;
        if(currentGoal < NBGOALS) {
            state->xgoal = XGOALS[currentGoal];
            state->ygoal = YGOALS[currentGoal];
        }
    }
    last_reward = reward();
}

bool GridWorld::accessible(int x, int y) const {
    if(x < 0 || x >= XMAX || y < 0 || y >= YMAX)
        return false;

    return REWARD[x][y] != 0;
}

double GridWorld::reward() const {
    return world[state->x][state->y];
}

DAction* GridWorld::getInitialAction() const {
    return new DAction(getActions(), 0);
}

bool GridWorld::goal() const {
    return currentGoal >= NBGOALS /*|| currentGoal == NBGOALS -1 && state->x == 0 && state->y == 9*/;
}

unsigned int GridWorld::maxStep() const {
    return 5000;
}

void GridWorld::computeDState(const GridWorldState& s, DState* dst, const ActionTemplate* repr) {
    (void) repr;
    dst->set(XPOS, s.x);
    dst->set(YPOS, s.y);
    dst->set(XG, s.xgoal);
    dst->set(YG, s.ygoal);
}

}
