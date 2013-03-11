#include "TWorld.hpp"
#include <Driver.hpp>

#include <car.h>
#include <bib/Logger.hpp>
#include <sml/Utils.hpp>

double TWorld::reward(const Driver& d) {
    double distanceMilieu = 10. - sml::Utils::abs( d.getCar()->_trkPos.toMiddle );
    double distParcourue = d.getCoveredDistance();
    double dammageGet = d.getDamageGet();

    double r = distanceMilieu*(distParcourue);
    if (r < 1 )
        r = -10;
    if(d.isStuck() && d.getCar()->ctrl.gear == -1 && d.getCar()->ctrl.accelCmd > 0.2 && d.getCar()->ctrl.brakeCmd < 0.1)
        r = 15;
    else if(distParcourue < 0.1) {
        r = -40;
        if(d.isStuck())
            r -= 45;
	else if(distParcourue < 0)
	  r -= 100;
    }
    else if(distanceMilieu < 0. || distParcourue < 0.)
        r = -r;
    else r = r*r/4.;

    r-=dammageGet;

//     LOG_DEBUG(d.getCar()->_trkPos.toRight);
    
    if(d.getCar()->_trkPos.toRight < 1. )
        r += 10*d.getCar()->_trkPos.toRight ;
    else if (d.getCar()->_trkPos.toLeft < 1.)
        r += 10*d.getCar()->_trkPos.toLeft ;
 
    
    if(r > 0 && !d.isStuck())
       if((d.getAngle() < M_PI/8 && d.getAngle() >= 0) || (d.getAngle() > -M_PI/8 && d.getAngle() <= 0 ))
	 r += 40;

//     LOG_DEBUG("recompense " << distanceMilieu << " "<< distParcourue << " " << r << " -- "  << d.getCar()->_distFromStartLine << " ");

    return r;
}

unsigned int TWorld::discretizeAngle(float angle, unsigned int cardinal){
     unsigned int d = 0;
     
     for(unsigned int i=0; i<cardinal; i++)
        if(angle < -M_PI +(M_PI/((double)cardinal/2.))*i ) {
            d = i;
            break;
        }
        
     return d;
}

unsigned int TWorld::discretizeDistanceFromMiddle(float distance, unsigned int cardinal, double dismin, double dismax){
    int d = -1;
      
    for(unsigned int i=0; i<cardinal; i++)
        if(distance < dismin + ((dismax-dismin)/(double)cardinal)*(double)i) {
            d = i;
            break;
        }
    if(d == -1 )
      return cardinal - 1;
      
    return (unsigned int)d;
}

float TWorld::computeSteering(unsigned int discetized, unsigned int cardinal, double smin, double smax){
    return smin+((float)discetized/(float)cardinal)*(smax-smin);
}
