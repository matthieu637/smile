#include "QLearnDiscr2.hpp"
#include <sml/SaveLoad.hpp>
#include <sml/Utils.hpp>
#include <iostream>
#include <bib/Logger.hpp>

#define ACC "acceleration"
#define DIR "direction"
#define AGL "angle"
#define DST "distance"
#define SPD "speed"



const sml::ActionTemplate QLearnDiscr2::ACTION_TEMPLATE = sml::ActionTemplate( {ACC, DIR}, {QLearnDiscr2::ACTIONS_ACC, QLearnDiscr2::ACTIONS_DIRECTION});
const sml::StateTemplate QLearnDiscr2::STATE_TEMPLATE = sml::StateTemplate( {AGL, DST}, {QLearnDiscr2::STATES_ALPHA, QLearnDiscr2::STATES_DISTANCE}); //, SPD});

QLearnDiscr2::QLearnDiscr2(int index):Driver(index, DECISION_EACH), Q(&STATE_TEMPLATE, &ACTION_TEMPLATE), N(&STATE_TEMPLATE, &ACTION_TEMPLATE)
{

}

QLearnDiscr2::~QLearnDiscr2()
{
    Q.write("smile1.data");
}

double QLearnDiscr2::reward() {
    double distanceMilieu = 10. - abs( car->_trkPos.toMiddle );
    double distParcourue;
    double dammageGet = car->_dammage - lastDammage;

    if(lastDist==-1)
        distParcourue = 0;
    else
        distParcourue = car->_distFromStartLine - lastDist;
    if(distParcourue > 1000|| distParcourue < -1000) //passe ligne
        distParcourue = 20;
    double r = distanceMilieu*distParcourue;
    if (r < 1 )
        r = -10;
    if(isStuck() && car->ctrl.gear == -1)
        r = 15;
    else if(distParcourue < 0.5) {
        r = -40;
        if(isStuck())
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

    //std::cout << "recompense " << [DST]Milieu << " "<< distParcourue << " " << r << " -- "  << car->_distFromStartLine << " " << lastDist << std::endl;
    //std::cout << std::flush;

    lastDist = car->_distFromStartLine;
    lastDammage = car->_dammage;

    return r;
}

void QLearnDiscr2::decision()
{
    DState s = *lastState;

    //Take action a, observe r, s'
    DAction a = *lastAction;
    double r = reward();
    State st = { angle , car->_trkPos.toMiddle };
    DState* Psp = discretize(st);//TODO:memory leak
    DState sp = *Psp;

    //Choose a' from s' using policy derived from Q
    DAction* PaP = Q.argmax(sp); //TODO:memory leak
    DAction ap = *PaP;
    DAction as = ap; //if a' ties for the max, then a* <- a'
    if(sml::Utils::rand01() < espilon ){
        PaP = new DAction(&ACTION_TEMPLATE, {rand() % ACTIONS_ACC, rand() % ACTIONS_DIRECTION});//TODO:memory
	ap = *PaP;
    }


    if(init) {
        double delta = r + discount*Q(sp, as) - Q(s,a);
        N(s,a) = N(s, a) + 1.;

	/*
        for(unsigned int sa = 0; sa < STATE_TEMPLATE.sizeNeeded(); sa++)
            for(unsigned int aa = 0; aa < ACTION_TEMPLATE.sizeNeeded(); aa++) {
		  Q(sa,aa) = Q(sa,aa) + lrate * delta * N(sa,aa);
                if(ap == as)
                    N(sa, aa) = discount*lamba*N(sa,aa);
                else
                    N(sa, aa) = 0.;
            }*/
            
        for(std::list< std::pair<DState* , DAction* > >::iterator it = history.begin(); it != history.end(); ++it ){
	    DState sa = *(*it).first;
	    DState aa = *(*it).second;
	    Q(sa,aa) = Q(sa,aa) + lrate * delta * N(sa,aa);
	    if(ap == as)
	      N(sa, aa) = discount*lamba*N(sa,aa);
	    else
	      N(sa, aa) = 0.;
	}
            
        if(ap == as);
	else
	  history.clear();
            
	history.push_back(std::pair<DState* , DAction* >(Psp, PaP));
    }


    lastAction = PaP;
    lastState = Psp;
    init=true;

    std::cout << "etat " << sp[AGL] << " "<< sp[DST] << " action " << ap[ACC] << " "<< ap[DIR] << "   recomp : " << r << " : "  << angle << std::endl;
    std::cout << std::flush;

    applyActionOn(ap, car);

}

void QLearnDiscr2::newRace(tCarElt* car, tSituation *s) {
    Driver::newRace(car,s);
    
    Q.init();
    N.init();
    Q.read("smile1.data");

    lastState = new DState(&STATE_TEMPLATE, 0);
    lastAction = new DAction(&ACTION_TEMPLATE, 0);
}


DState* QLearnDiscr2::discretize(const State& st) {
    DState* dst = new DState(&STATE_TEMPLATE, 0) ;

    for(int i=0; i<STATES_ALPHA; i++)
        if(st.alpha < -M_PI +(M_PI/((double)STATES_ALPHA/2.))*i ) {
            dst->set(AGL,i);
            break;
        }

//	std::cout << "[DST] " << st.[DST] << " "<< dst.[DST] << std::endl;
//	std::cout << std::flush;

    double dismin = -8.;
    double dismax = 8.;

    for(int i=0; i<STATES_DISTANCE; i++)
        if(st.distance < dismin + ((dismax-dismin)/(double)STATES_DISTANCE)*(double)i) {
            dst->set(DST, i);
            break;
        }

//     std::cout << "etat " << (*dst)[AGL] << " "<< (*dst)[DST] << " " << st.distance << std::endl;
//     std::cout << std::flush;

    return dst;
}

void QLearnDiscr2::applyActionOn(const DAction& ac, tCarElt* car) {
    double smin = -0.5;
    double smax = 0.5;


    car->ctrl.steer =  smin+((double)ac[DIR]/(double)ACTIONS_DIRECTION)*(smax-smin);

    //std::cout << "steer " << car->ctrl.steer << " " << ac[DIR] << " " << (double)(ac[DIR]/ACTIONS_DIRECTION) << " " << (smax-smin) <<" "<< (double)smin+((double)(ac[DIR]/ACTIONS_DIRECTION))*(smax-smin) << std::endl;
    //std::cout << std::flush;

    unsigned int accel = ac[ACC];

    if(accel < 2) {
        accel = 2 - accel;
        car->ctrl.gear = -1;
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = accel / 3.;
    } else if(accel < 4) {
        accel -= 2;
        accel = 2 - accel;
        //car->ctrl.gear = car->ctrl.gear;
        car->ctrl.brakeCmd = accel / 3.;
        car->ctrl.accelCmd = 0;
    } else if(accel < 7)
    {
        accel -= 4;
        accel = 3 - accel;
        car->ctrl.gear = getGear();
        car->ctrl.brakeCmd = 0;
        car->ctrl.accelCmd = accel / 4.;
    }
}

#undef ACC
#undef DIR
#undef AGL
#undef DST
#undef SPD


