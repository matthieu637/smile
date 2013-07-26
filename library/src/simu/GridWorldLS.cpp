#include "simu/GridWorldLS.hpp"
#include "bib/Logger.hpp"

namespace simu {

//-1 means clean way
//0 means wall
//-10 malus
//10/50 goals
const int GridWorldLS::REWARD[XMAX][YMAX]= {
    { -1, -1, -1, 10, -1,  0,  -1, -1, -1, -10},
    { -1, -1, -1,  0, -1,  0,  -1, -1, -1, -1 },
    {  0,  0, -1,  0, -1, 10,  -1, -1,  0, 10 },
    {-10,  0, -1,  0, -1,  0,  -1,  0, -1, -1 },
    { 10,  0, -1, -1, -1,  0, -20,  0,  0, -1 },
    { -1,  0, -1, -1, -1,  0,  -1,  0, -1, 10 },
    { -1,  0, 50,  0, -1,  0,  -1,  0, -1, 0  },
    { -1, -1, -1,  0, -1,  0,  -1,  0, -1, 0  },
    { -1,  0, -1,  0,  0,  0,  -1,  0, -1, 0  },
    { -1, -1, -1, -1, -1, -1,  -1, -1, -1, 0  }
};

GridWorldLS::GridWorldLS() : Environnement< GridWorldLSState >(new StateTemplate( {XPOS, YPOS}, {XMAX, YMAX}), new ActionTemplate( {MOV}, {4}))
{
    init();
}

void GridWorldLS::initState() {
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
    last_reward = -1;
}

void GridWorldLS::applyOn(const DAction& ac)
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
    
    state->p = passed[state->x][state->y];
    passed[state->x][state->y]++;

    if(reward() > 0)
        keepGoals--;
    
    last_reward = reward();
}

bool GridWorldLS::accessible(int x, int y) const {
    if(x < 0 || x >= XMAX || y < 0 || y >= YMAX)
        return false;

    return REWARD[x][y] != 0;
}

double GridWorldLS::reward() const {
    return world[state->x][state->y] - state->p;
}

DAction* GridWorldLS::getInitialAction() const {
    return new DAction(getActions(), 0);
}

bool GridWorldLS::goal() const {
    return keepGoals <= 0;
}

unsigned int GridWorldLS::maxStep() const {
    return 5000;
}

void GridWorldLS::computeDState(const GridWorldLSState& s, DState* dst, const ActionTemplate* repr) {
    (void) repr;
//     LOG_DEBUG(s.x << " " << s.y << " " << s.p);
    dst->set(XPOS, s.x);
    dst->set(YPOS, s.y);
//     dst->set(PAS, s.p >= PMAX ? PMAX - 1 : s.p);
//     LOG_DEBUG(*dst);
}

}
