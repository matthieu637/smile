/***************************************************************************

    file                 : smile.cpp
    created              : Wed Jan 8 18:31:16 CET 2003
    copyright            : (C) 2002 Bernhard Wymann
    email                : berniw@bluewin.ch
    version              : $Id: smile.cpp,v 1.3.2.2 2008/11/09 17:50:21 berniw Exp $

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <tgf.h>
#include <track.h>
#include <car.h>
#include <raceman.h>
#include <robottools.h>
#include <robot.h>

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

#ifndef M_PI
   #define M_PI 3.14159265358979323846
#endif

static void initTrack(int index, tTrack* track, void *carHandle, void **carParmHandle, tSituation *s);
static void newRace(int index, tCarElt* car, tSituation *s);
static void drive(int index, tCarElt* car, tSituation *s);
//static int pitcmd(int index, tCarElt* car, tSituation *s);
static void shutdown(int index);
static int InitFuncPt(int index, void *pt);
static void endRace(int index, tCarElt *car, tSituation *s);
void initializeQ();

#include <iostream>
#include <ctime>


const int DECISION_EACH = 13;

const int STATES_ALPHA = 16;
const int STATES_DISTANCE = 12;
const int ACTIONS_ACC = 7;
const int ACTIONS_DIRECTION = 10; //pair for stabilization

int statec = STATES_ALPHA * STATES_DISTANCE ;
int actionc = ACTIONS_ACC * ACTIONS_DIRECTION;

//int Q[320][250];
float Q[STATES_ALPHA][STATES_DISTANCE][ACTIONS_ACC][ACTIONS_DIRECTION];

struct State {
	float alpha; //direction
	float distance;
};

struct DState {
	int alpha;
	int distance;
};

struct DAction {
	int acc;
	int direc;
};


using namespace boost::interprocess;
void syncQ()
{
    
    named_mutex mutex( open_or_create, "qsync_mutex");
    mutex.lock();
	
    FILE *fp = fopen("matrix.txt", "w+");
    
    //si le fichier n'existe pas, pas de sync
    fseek(fp, 0, SEEK_END);
    if( ftell(fp) > 0 ) 
    {
      for(int i = 0; i < STATES_ALPHA; i++)
	  for(int j = 0; j < STATES_DISTANCE; j++)
		  for(int k = 0; k < ACTIONS_ACC; k++)
			  for(int m = 0; m < ACTIONS_DIRECTION; m++)
			  {
			      float q;
			      fscanf(fp, "%f", &q);
			      if(q != 0.0 && Q[i][j][k][m] == 0.0)
				Q[i][j][k][m] = q;
			      else if( q != 0.0 && Q[i][j][k][m] != 0.0 )
				Q[i][j][k][m] = ( Q[i][j][k][m] + q*3. )/4.; //les valeurs déjà apprisent ont plus de poids
			  }
    }
    rewind(fp);
			
    for(int i = 0; i < STATES_ALPHA; i++)
        for(int j = 0; j < STATES_DISTANCE; j++)
        	for(int k = 0; k < ACTIONS_ACC; k++)
			for(int m = 0; m < ACTIONS_DIRECTION; m++)		
			    fprintf(fp, "%f\n", Q[i][j][k][m]);
    fclose(fp);
    
    mutex.unlock();
}


void writeQ()
{
    named_mutex mutex( open_or_create, "qsync_mutex");
    mutex.lock();

    FILE *fp = fopen("matrix.txt", "w");
    for(int i = 0; i < STATES_ALPHA; i++)
        for(int j = 0; j < STATES_DISTANCE; j++)
        	for(int k = 0; k < ACTIONS_ACC; k++)
			for(int m = 0; m < ACTIONS_DIRECTION; m++)
			    fprintf(fp, "%f\n", Q[i][j][k][m]);
    fclose(fp);
    mutex.unlock();
}


void readQ()
{
      named_mutex mutex( open_or_create, "qsync_mutex");
    mutex.lock();
	
 FILE *fp = fopen("matrix.txt", "r");
    for(int i = 0; i < STATES_ALPHA; i++)
        for(int j = 0; j < STATES_DISTANCE; j++)
        	for(int k = 0; k < ACTIONS_ACC; k++)
			for(int m = 0; m < ACTIONS_DIRECTION; m++)
fscanf(fp, "%f", &Q[i][j][k][m]);
fclose(fp); 
    mutex.unlock();
}

void initializeQ(){

    srand((unsigned)time(0));
    
    FILE *fp = fopen("matrix.txt", "r");
    if(fp != NULL)
      readQ();
    else
    for(int i = 0; i < STATES_ALPHA; i++)
        for(int j = 0; j < STATES_DISTANCE; j++)
        	for(int k = 0; k < ACTIONS_ACC; k++)
			for(int m = 0; m < ACTIONS_DIRECTION; m++)
			      Q[i][j][k][m] = 0;
		
	
}


/* Module entry point */
extern "C" int smile(tModInfo *modInfo)
{
	/* clear all structures */
	memset(modInfo, 0, 10*sizeof(tModInfo));

	modInfo->name    = strdup("Smile");	/* name of the module (short) */
	modInfo->desc    = strdup("");	/* description of the module (can be long) */
	modInfo->fctInit = InitFuncPt;			/* init function */
	modInfo->gfId    = ROB_IDENT;				/* supported framework version */
	modInfo->index   = 0;						/* indices from 0 to 9 */

	return 0;
}


/* Module interface initialization. */
static int InitFuncPt(int index, void *pt)
{
	tRobotItf *itf = (tRobotItf *)pt;

	/* create robot instance for index */
	//driver[index] = new Driver(index);
	itf->rbNewTrack = initTrack;	/* Give the robot the track view called */
	itf->rbNewRace  = newRace;		/* Start a new race */
	itf->rbDrive    = drive;		/* Drive during race */
	itf->rbPitCmd   = NULL;		/* Pit commands */
	itf->rbEndRace  = endRace;		/* End of the current race */
	itf->rbShutdown = shutdown;		/* Called before the module is unloaded */
	itf->index      = index;		/* Index used if multiple interfaces */
	return 0;
}


/* Called for every track change or new race. */
static void initTrack(int index, tTrack* track, void *carHandle, void **carParmHandle, tSituation *s)
{
      *carParmHandle = NULL; 
	//driver[index]->initTrack(track, carHandle, carParmHandle, s);
}

const float MAX_UNSTUCK_ANGLE = 15.0/180.0*PI;		/* [radians] */
const float UNSTUCK_TIME_LIMIT = 2.0/DECISION_EACH;				/* [s] */
const float MAX_UNSTUCK_SPEED = 5.0;   /* [m/s] */
const float MIN_UNSTUCK_DIST = 3.0;    /* [m] */

int stuck = 0;
int MAX_UNSTUCK_COUNT;
/* Start a new race. */
static void newRace(int index, tCarElt* car, tSituation *s)
{
	//driver[index]->newRace(car, s);
	initializeQ();
	MAX_UNSTUCK_COUNT = int(UNSTUCK_TIME_LIMIT/RCM_MAX_DT_ROBOTS);
}

DState discretize(const State& st){
	//pointer me
	DState dst = {0,0} ; //init me pls or bug

	for(int i=0;i<STATES_ALPHA; i++)
		if(st.alpha < -M_PI +(M_PI/8*i) ){
			dst.alpha = i;
			break;
		}

//	std::cout << "distance " << st.distance << " "<< dst.distance << std::endl;
//	std::cout << std::flush;

	float dismin = -6.;
	float dismax = 6.;
	
	for(int i=0;i<STATES_DISTANCE; i++)
		if(st.distance < dismin + (dismax-dismin/(float)STATES_DISTANCE)*(float)i){
			dst.distance = i;
			break;
		}
	
	//std::cout << "etat " << dst.alpha << " "<< dst.distance << " " << std::endl;
	//std::cout << std::flush;
	
	return dst;
}

DAction bestQVal(const DState& dst){ 

	
	DAction imax={rand() % ACTIONS_ACC, rand() % ACTIONS_DIRECTION};//pointer me
	for(int i=0;i<ACTIONS_ACC;i++)
		for(int k=0;k<ACTIONS_DIRECTION;k++){
		    //std::cout << dst.alpha << " "<< dst.distance <<" "<< i <<" " << j << " " << k << std::endl;
		    //std::cout << std::flush;
			if(Q[dst.alpha][dst.distance][i][k] > Q[dst.alpha][dst.distance][imax.acc][imax.direc])
			{
			  imax.acc = i;
			  imax.direc = k;
			}
		}
	
//	std::cout << "action " << imax.acc << " "<< imax.direc << " " << imax.stop << std::endl;
//	std::cout << std::flush;

	return imax;
}

DState lastState={-1,-1};
DAction lastAction;
float lrate = 0.05;
float discount = 0.35;
float espilon = 0.05;

float rand01(){
 return (float)rand()/(float)RAND_MAX;
}

/* Check if I'm stuck */
bool isStuck(tCarElt* car, float angle)
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

void applyActionOn(const DAction& ac, tCarElt* car){
    float smin = -0.4;
    float smax = 0.4;
    
    car->ctrl.steer =  smin+((float)ac.direc/(float)ACTIONS_DIRECTION)*(smax-smin);
    
    //std::cout << "steer " << car->ctrl.steer << " " << ac.direc << " " << (float)(ac.direc/ACTIONS_DIRECTION) << " " << (smax-smin) <<" "<< (float)smin+((float)(ac.direc/ACTIONS_DIRECTION))*(smax-smin) << std::endl;
    //std::cout << std::flush;
    
    int accel = ac.acc;
    
    if(accel < 2){
      accel = 2 - accel;
      car->ctrl.gear = -1;
      car->ctrl.brakeCmd = 0;
      car->ctrl.accelCmd = accel / 3.;
    } else if(accel < 4){
      accel -= 2;
      accel = 2 - accel;
      //car->ctrl.gear = car->ctrl.gear;
      car->ctrl.brakeCmd = accel / 3.;
      car->ctrl.accelCmd = 0;
    } else if(accel < 7)
    {
      accel -= 4;
      accel = 3 - accel;
      car->ctrl.gear = 1;
      car->ctrl.brakeCmd = 0;
      car->ctrl.accelCmd = accel / 4.;
    }
    else 
    {
      accel -= 7;
      accel = 3 - accel;
      car->ctrl.gear = 2;
      car->ctrl.brakeCmd = 0;
      car->ctrl.accelCmd = 1. - (accel / 4.);
    }
}

float lastDist = -1;
float lastDammage = 0;
int waittime = 0;

/* Drive during race. */
static void drive(int index, tCarElt* car, tSituation *s)
{
  //reset
  // memset((void *)&car->ctrl, 0, sizeof(tCarCtrl)); 
   waittime ++;

    //std::cout << " ----- " << car->ctrl.gear<< " " << car->ctrl.brakeCmd << " " << car->ctrl.accelCmd << std::endl;
   // std::cout << std::flush;
   
   if(waittime != DECISION_EACH){
  //    applyActionOn(lastAction, car);
      return;
   } else{
     waittime =0;
   }
   
   //create state
    float angle;
    angle = RtTrackSideTgAngleL(&(car->_trkPos)) - car->_yaw;
    NORM_PI_PI(angle); // put the angle back in the range from -PI to PI

     //   std::cout << " ----- " << isStuck(car, angle) << " " << car->ctrl.gear << std::endl;
	//std::cout << std::flush;
   
   float distanceMilieu = 10. - abs( car->_trkPos.toMiddle );
   float distParcourue;
   float dammageGet = car->_dammage - lastDammage;
   if(lastDist==-1)
     distParcourue = 0;
   else
     distParcourue = car->_distFromStartLine - lastDist;
   if(distParcourue > 1000|| distParcourue < -1000) //passe ligne
     distParcourue = 20;
   float r = distanceMilieu*distParcourue; 
   if (r < 1 ) 
     r = -10;
   if(isStuck(car, angle) && car->ctrl.gear == -1)
     r = 15;
   else if(distParcourue < 0.5){
     r = -40;
     if(isStuck(car,angle))
       r -= 45;
   }
   else if(distanceMilieu < 0. || distParcourue < 0.)
     r = -r;
   else r = r*r/4.;
   
   r-=dammageGet;
   
   if(car->_trkPos.toRight < 0. )
     r += 15*car->_trkPos.toRight ;
   else if (car->_trkPos.toLeft < 0.)
     r += 15*car->_trkPos.toLeft ;
   /*
   if(r > 0)
      if((angle < M_PI/16 && angle >= 0) || (angle > -M_PI/16 && angle <= 0 ))
	r += 40;*/
   
   //std::cout << "recompense " << distanceMilieu << " "<< distParcourue << " " << r << " -- "  << car->_distFromStartLine << " " << lastDist << std::endl;
   //std::cout << std::flush;

   lastDist = car->_distFromStartLine;
   lastDammage = car->_dammage;

   State st = { angle , car->_trkPos.toMiddle };
   DState dst = discretize(st);
   DAction dac = bestQVal(dst);

   //update Q value with lastState
   if(lastState.alpha != -1 && lastState.distance != -1){
	Q[lastState.alpha][lastState.distance][lastAction.acc][lastAction.direc] =
		(float) Q[lastState.alpha][lastState.distance][lastAction.acc][lastAction.direc] + //alpha*N[lastState.alpha][lastState.distance][lastAction.acc][lastAction.direc]*
		lrate*(r+discount*Q[dst.alpha][dst.distance][dac.acc][dac.direc] - Q[lastState.alpha][lastState.distance][lastAction.acc][lastAction.direc] );
		
		

   }
   
   if(rand01() < espilon ){
     dac = {rand() % ACTIONS_ACC, rand() % ACTIONS_DIRECTION};
   }

   lastAction = dac;
   lastState = dst;
   
    std::cout << "etat " << dst.alpha << " "<< dst.distance << " action " << dac.acc << " "<< dac.direc << "   recomp : " << r << " : "  << angle << std::endl;
    std::cout << std::flush;
   
    applyActionOn(dac, car);

}


/* Pitstop callback */
/*static int pitcmd(int index, tCarElt* car, tSituation *s)
{
	return driver[index]->pitCommand(s);
}*/


/* End of the current race */
static void endRace(int index, tCarElt *car, tSituation *s)
{
  	std::cout << "End race" << std::endl;
	std::cout << std::flush;
	//driver[index]->endRace(s);
}


/* Called before the module is unloaded */
static void shutdown(int index)
{
	//syncQ();
	writeQ();

	std::cout << "Shutdown" << std::endl;
	std::cout << std::flush;
	//delete driver[index];
}

