
#include <iostream>
#include "Driver.hpp"

const float Driver::MAX_UNSTUCK_ANGLE = 15.0/180.0*PI;		/* [radians] */

const float Driver::MAX_UNSTUCK_SPEED = 5.0;				/* [m/s] */
const float Driver::MIN_UNSTUCK_DIST = 3.0;					/* [m] */

const float Driver::SHIFT = 0.9;							/* [-] (% of rpmredline) */
const float Driver::SHIFT_MARGIN = 4.0;						/* [m/s] */


Driver::Driver(int index, int intervalAction)
{
    INDEX = index;
    INTERVAL_ACTION = intervalAction;
    UNSTUCK_TIME_LIMIT = 2.0/INTERVAL_ACTION;				/* [s] */
}

Driver::~Driver()
{

}

void Driver::endRace(tSituation* s)
{
	(void) s;
	std::cout << "End Race" << std::endl;
	std::cout << std::flush;
}

/* Called for every track change or new race. */
void Driver::initTrack(tTrack* t, void *carHandle, void **carParmHandle, tSituation *s)
{
    track = t;
   // *carParmHandle = NULL; 
    (void) carHandle;
    (void) s;
}


/* Start a new race. */
void Driver::newRace(tCarElt* car, tSituation *s)
{
    (void) s;
    float deltaTime = RCM_MAX_DT_ROBOTS;
    MAX_UNSTUCK_COUNT = int(UNSTUCK_TIME_LIMIT/deltaTime);

    stuck = 0;
    this->car = car;
}


/* Drive during race. */
void Driver::drive(tSituation *s)
{
    //reset control
    //memset(&car->ctrl, 0, sizeof(tCarCtrl));

    update(s);
}


/***************************************************************************
 *
 * utility functions
 *
***************************************************************************/

/* Compute the length to the end of the segment */
float Driver::getDistToSegEnd()
{
    if (car->_trkPos.seg->type == TR_STR) {
        return car->_trkPos.seg->length - car->_trkPos.toStart;
    } else {
        return (car->_trkPos.seg->arc - car->_trkPos.toStart)*car->_trkPos.seg->radius;
    }
}

/* Compute gear */
int Driver::getGear()
{
    if (car->_gear <= 0) return 1;
    float gr_up = car->_gearRatio[car->_gear + car->_gearOffset];
    float omega = car->_enginerpmRedLine/gr_up;
    float wr = car->_wheelRadius(2);

    if (omega*wr*SHIFT < car->_speed_x) {
        return car->_gear + 1;
    } else {
        float gr_down = car->_gearRatio[car->_gear + car->_gearOffset - 1];
        omega = car->_enginerpmRedLine/gr_down;
        if (car->_gear > 1 && omega*wr*SHIFT > car->_speed_x + SHIFT_MARGIN) {
            return car->_gear - 1;
        }
    }
    return car->_gear;
}


/* Update my private data every timestep */
void Driver::update(tSituation *s)
{
    (void) s;
    float trackangle = RtTrackSideTgAngleL(&(car->_trkPos));
    angle = trackangle - car->_yaw;
    NORM_PI_PI(angle);

    if(decision_each == INTERVAL_ACTION)
    {
        decision();
        decision_each=0;
    }
    else decision_each++;
}


/* Check if I'm stuck */
bool Driver::isStuck()
{
    if (fabs(angle) > MAX_UNSTUCK_ANGLE &&
            car->_speed_x < MAX_UNSTUCK_SPEED &&
            fabs(car->_trkPos.toMiddle) > MIN_UNSTUCK_DIST) {
        if (stuck > MAX_UNSTUCK_COUNT && car->_trkPos.toMiddle*angle < 0.0) {
            return true;
        } else {
            stuck++;
            return false;
        }
    } else {
        stuck = 0;
        return false;
    }
}

