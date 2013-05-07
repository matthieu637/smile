/***************************************************************************

    file                 : sparkle.cpp
    created              : Wed Jan 8 18:31:16 CET 2003
    copyright            : (C) 2002 Bernhard Wymann
    email                : berniw@bluewin.ch
    version              : $Id: sparkle.cpp,v 1.3.2.2 2008/11/09 17:50:21 berniw Exp $

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
#include <iostream>
#include <math.h>

#include "QLearnDiscr.hpp"
#include "QLearnDiscr2.hpp"
#include "QLearnGen.hpp"
#include "QLearnGenCmplx.hpp"
#include <QLearnGenFdb.hpp>

#define NBBOTS 5

static const char* botname[NBBOTS] = {
    "smile_0", "smile_1", "smile_2", "smile_3", "smile_4"
};

static const char* botdesc[NBBOTS] = {
    "Discretize Q learning", "Discretize Q learning (lamba)", "Q learning f. approximation",
    "Q learning f. approximation Env Cmplx", "Q learning f. approximation Feedback"
};

static Driver *driver[NBBOTS];

static void initTrack(int index, tTrack* track, void *carHandle, void **carParmHandle, tSituation *s);
static void newRace(int index, tCarElt* car, tSituation *s);
static void drive(int index, tCarElt* car, tSituation *s);
/*static int pitcmd(int index, tCarElt* car, tSituation *s);*/
static void shutdown(int index);
static int InitFuncPt(int index, void *pt);
// static void endRace(int index, tCarElt *car, tSituation *s);


/* Module entry point */
extern "C" int smile(tModInfo *modInfo)
{
    int i;
    
    /* clear all structures */

    memset(modInfo, 0, 10*sizeof(tModInfo));

    for (i = 0; i < NBBOTS; i++) {
        modInfo[i].name    = strdup(botname[i]);	/* name of the module (short) */
        modInfo[i].desc    = strdup(botdesc[i]);	/* description of the module (can be long) */
        modInfo[i].fctInit = InitFuncPt;			/* init function */
        modInfo[i].gfId    = ROB_IDENT;				/* supported framework version */
        modInfo[i].index   = i;						/* indices from 0 to 9 */
    }

    return 0;
}


/* Module interface initialization. */
static int InitFuncPt(int index, void *pt)
{

    tRobotItf *itf = (tRobotItf *)pt;

    /* create robot instance for index */

    switch(index) {
    case 0:
        driver[index] = new QLearnDiscr(index);
        break;
    case 1:
        driver[index] = new QLearnDiscr2(index);
        break;
    case 2:
	driver[index] = new QLearnGen(index);
	break;
    case 3:
	driver[index] = new QLearnGenCmplx(index);
	break;
    case 4:
 	driver[index] = new QLearnGenFdb(index);
	break;
    }
    
    itf->rbNewTrack = initTrack;	/* Give the robot the track view called */
    itf->rbNewRace  = newRace;		/* Start a new race */
    itf->rbDrive    = drive;		/* Drive during race */
    itf->rbPitCmd   = NULL;		/* Pit commands */
    itf->rbEndRace  = NULL;		/* End of the current race */
    itf->rbShutdown = shutdown;		/* Called before the module is unloaded */
    itf->index      = index;		/* Index used if multiple interfaces */

    return 0;
}


/* Called for every track change or new race. */
static void initTrack(int index, tTrack* track, void *carHandle, void **carParmHandle, tSituation *s)
{
    driver[index]->initTrack(track, carHandle, carParmHandle, s);
}


/* Start a new race. */
static void newRace(int index, tCarElt* car, tSituation *s)
{
    driver[index]->newRace(car, s);
}


/* Drive during race. */
static void drive(int index, tCarElt* car, tSituation *s)
{
    (void) car;
    driver[index]->drive(s);
}


/* Pitstop callback */
/*static int pitcmd(int index, tCarElt* car, tSituation *s)
{
	//return driver[index]->pitCommand(s);
}
*/

/* End of the current race */
// static void endRace(int index, tCarElt *car, tSituation *s)
// {
//   
// }


/* Called before the module is unloaded */
static void shutdown(int index)
{
    driver[index]->endRace();
    delete driver[index];
}

