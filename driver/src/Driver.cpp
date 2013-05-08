
#include <iostream>
#include <bib/Logger.hpp>
#include "Driver.hpp"
#include <TWorld.hpp>

const float Driver::MAX_UNSTUCK_ANGLE = 15.0/180.0*PI;		/* [radians] */

const float Driver::MAX_UNSTUCK_SPEED = 5.0;				/* [m/s] */
const float Driver::MIN_UNSTUCK_DIST = 3.0;					/* [m] */

const float Driver::SHIFT = 0.9;							/* [-] (% of rpmredline) */
const float Driver::SHIFT_MARGIN = 4.0;						/* [m/s] */
const float Driver::UNSTUCK_TIME_LIMIT = 2.0;				/* [s] */

const float Driver::LOOKAHEAD_CONST = 17.0;					/* [m] */
const float Driver::LOOKAHEAD_FACTOR = 0.33;				/* [-] */


Driver::Driver(int index, int intervalAction, float simu_time):simu_time(simu_time)
{
    srand(time(NULL)); //need random in many algorithms
    INDEX = index;
    INTERVAL_ACTION = intervalAction;


    decision_each = 0;
    stuck = 0;
    globalReward = 0.;
}

Driver::~Driver()
{
  
}

/* Called for every track change or new race. */
void Driver::initTrack(tTrack* t, void *carHandle, void **carParmHandle, tSituation *s)
{
    track = t;
    *carParmHandle = NULL; //until it will bug with GfParmCheckHandle
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

    if(decision_each > INTERVAL_ACTION)
    {
        if (car->_trkPos.toRight != car->_trkPos.toRight || s->currentTime > simu_time*60. /*|| //fix bug during simulation ?
	  (s->currentTime > (nbLaps/4)*5.*60. && !getAlgo()->keepLearning(globalReward/1000))*/) { //break simu with too bad perf
            endRace();
            exit(1);
        }
        
        reward = TWorld::reward(*this);
	globalReward += reward;
        decision();
        decision_each=0;
        lastDammage = car->_dammage;
        lastDistance = car->_distRaced;
    }
}

void Driver::endRace()
{
    LOG_DEBUG("Global Perf : " << globalReward/1000);
    getAlgo()->setPerf(globalReward/1000);
    getAlgo()->increaseEpisode();
}

/***************************************************************************
 *
 * utility functions
 *
***************************************************************************/

/* Compute the length to the end of the segment */
float Driver::getDistToSegEnd() const
{
    int type = car->_trkPos.seg->type;
    if (type == TR_STR) {
        return car->pub.trkPos.seg->length - car->_trkPos.toStart;
    } else {
        return (car->_trkPos.seg->arc - car->_trkPos.toStart)*car->_trkPos.seg->radius;
    }
}

v2d Driver::getTargetPoint() const
{
	tTrackSeg *seg = car->_trkPos.seg;
	float lookahead = LOOKAHEAD_CONST + car->_speed_x*LOOKAHEAD_FACTOR;
	float length = getDistToSegEnd();
	float offset = 0.0;

	while (length < lookahead) {
		seg = seg->next;
		length += seg->length;
	}

	length = lookahead - length + seg->length;
	float fromstart = seg->lgfromstart;
	fromstart += length;

	v2d s;
	s.x = (seg->vertex[TR_SL].x + seg->vertex[TR_SR].x)/2.0;
	s.y = (seg->vertex[TR_SL].y + seg->vertex[TR_SR].y)/2.0;

	if ( seg->type == TR_STR) {
		v2d d, n;
		n.x = (seg->vertex[TR_EL].x - seg->vertex[TR_ER].x)/seg->length;
		n.y = (seg->vertex[TR_EL].y - seg->vertex[TR_ER].y)/seg->length;
		n.normalize();
		d.x = (seg->vertex[TR_EL].x - seg->vertex[TR_SL].x)/seg->length;
		d.y = (seg->vertex[TR_EL].y - seg->vertex[TR_SL].y)/seg->length;
		return s + d*length + offset*n;
	} else {
		v2d c, n;
		c.x = seg->center.x;
		c.y = seg->center.y;
		float arc = length/seg->radius;
		float arcsign = (seg->type == TR_RGT) ? -1.0 : 1.0;
		arc = arc*arcsign;
		s = s.rotate(c, arc);

		n = c - s;
		n.normalize();
		return s + arcsign*offset*n;
	}
}

/* Compute gear */
int Driver::getGear() const
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

    updateStuck();

    decision_each++;
}


void Driver::updateStuck()
{
    if (fabs(angle) > MAX_UNSTUCK_ANGLE &&
            car->_speed_x < MAX_UNSTUCK_SPEED &&
            fabs(car->_trkPos.toMiddle) > MIN_UNSTUCK_DIST) {
        if (stuck > MAX_UNSTUCK_COUNT && car->_trkPos.toMiddle*angle < 0.0) {
            ;
        } else {
            stuck++;
        }
    } else {
        stuck = 0;
    }
}

/* Check if I'm stuck */
bool Driver::isStuck() const
{
    if (fabs(angle) > MAX_UNSTUCK_ANGLE &&
            car->_speed_x < MAX_UNSTUCK_SPEED &&
            fabs(car->_trkPos.toMiddle) > MIN_UNSTUCK_DIST) {
        if (stuck > MAX_UNSTUCK_COUNT && car->_trkPos.toMiddle*angle < 0.0) {
            return true;
        } else {
//             stuck++;
            return false;
        }
    } else {
//         stuck = 0;
        return false;
    }
}

const tCarElt* Driver::getCar() const {
    return car;
}

double Driver::getDamageGet() const {
    return car->_dammage - lastDammage;
}

double Driver::getCoveredDistance() const {
    double distParcourue = 0;

    if(lastDistance != -1)
        distParcourue = car->_distRaced - lastDistance;
    return distParcourue;
}

float Driver::getAngle() const {
    return angle;
}

