#include "simu/GridWorld.hpp"
#include "bib/Logger.hpp"
#include <sml/Utils.hpp>

namespace simu {

//-1 means clean way
//0 means wall
//-10 malus
//10/50 goals
const int GridWorld::REWARD[XMAX][YMAX]= {
    { -1, -1, -1, -1, -1,  0,  -1, -1, -1, 40},
    { -1, -1, -1,  0, 20,  0,  -1, -1, -1, 0  },
    {  0,  0, -1,  0,  0, 20,  -1, -1,  0, -20 },
    { 20,  0, -1,  0, -1,  0,   0, -1, -1, -1 },
    { -1,  0, -1, -1, -1,  0, -20,  0,  0, -1 },
    { -1,  0, -1, -1, -1,  0,  -1,  0, -1, -1 },
    { -1,  0, 20, -1, -1,  0,  -1,  0, -1, 0  },
    { -1, -1, -1, -1, -1,  0,  -1,  0, -1, 0  },
    { -1,  0, -1,  0,  0,  0,  -1,  0, -1, 0  },
    {-20,  0, -1, -1, -1, -1,  -1, -1, -1, 0  }
};

const int GridWorld::XGOALS[NBGOALS] = {3, 1, 2, 6, 0};
const int GridWorld::YGOALS[NBGOALS] = {0, 4, 5, 2, 9};

GridWorld::GridWorld() : Environnement< GridWorldState >(new StateTemplate( {XPOS, YPOS, GL}, {XMAX, YMAX, GMAX}), new ActionTemplate( {MOV}, {4}))
{
    init();
}

void GridWorld::initState(bool random) {

    for(int i=0; i<XMAX; i++)
        for(int j=0; j<YMAX; j++)
            world[i][j]=REWARD[i][j];

    state->x = 0;
    state->y = 0;
    state->currentGoal = 0;
    state->xgoal = XGOALS[state->currentGoal];
    state->ygoal = YGOALS[state->currentGoal];
    last_reward = -1;
    
    if(random){
      state->x = rand() % 10;
      state->y = rand() % 10;
    }
}

void GridWorld::applyOn(const DAction& ac)
{
//     LOG_DEBUG(*dstate << " " << ac);
  
    // if we are on a goal, we eat it
    if(last_reward > 0) {
        world[state->x][state->y] = -1;
	
	//and pass to the next
        state->currentGoal++;
        if(state->currentGoal < NBGOALS) {
            state->xgoal = XGOALS[state->currentGoal];
            state->ygoal = YGOALS[state->currentGoal];
        }
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

    last_reward = reward();
}

bool GridWorld::accessible(int x, int y) const {
    if(x < 0 || x >= XMAX || y < 0 || y >= YMAX)
        return false;

    return REWARD[x][y] != 0;
}

double GridWorld::reward() const {
    if(world[state->x][state->y] > 0 ) {
        if(state->x == state->xgoal && state->y == state->ygoal)
            return world[state->x][state->y];
        return -1;
    }

    return world[state->x][state->y];
}

DAction* GridWorld::getInitialAction() const {
    return new DAction(getActions(), 0);
}

bool GridWorld::goal() const {
    return state->currentGoal >= NBGOALS /*|| currentGoal == NBGOALS -1 && state->x == 0 && state->y == 9*/;
}

unsigned int GridWorld::maxStep() const {
    return 1000;
}

void GridWorld::computeDState(const GridWorldState& s, DState* dst, const ActionTemplate* repr) {
    (void) repr;
    dst->set(XPOS, s.x);
    dst->set(YPOS, s.y);
//     dst->set(XG, s.xgoal);
//     dst->set(YG, s.ygoal);
    dst->set(GL, s.currentGoal);
}

}
