#ifndef DRIVERFEEDBACK_H
#define DRIVERFEEDBACK_H

///
///\file DriverFeedback.hpp
///\brief fourni les méthodes nécessaires à la récupération des événements claviers 
///
///

#include "Driver.hpp"
#include <vector>
#include <tgfclient.h>

using std::vector;

class DriverFeedback : public Driver
{
public:
  
///
///\struct tControlCmd
///\brief Définir une touche
    typedef struct {
        int		type;		/* Type of control (analog, joy button, keyboard) */
        int		val;		/* control index */
    } tControlCmd;

///
///\brief Constructeur
///\param index : numéro de l'agent
/// 	  intervalAction : la fréquence d'action de l'action de l'agent
///	  simu_time : le temps maximal pour un simulation
///	  listenKeys : les touches de la clavier à écouter
    DriverFeedback(int index, int intervalAction, float simu_time,const vector<tControlCmd>& listenKeys);

    
///
///\brief Mettre à jour les informations des touches activés
    void update(tSituation *s);
    
///
///\brief Déclelancheur de touches
    static int onKeyAction(unsigned char key, int modifier, int state);
    static int onSKeyAction(int key, int modifier, int state);


private:

    void updateKeys();

    static bool first_time;
    static int currentKey[256];
    static int currentSKey[256];

protected:
    typedef struct
    {
        int state;
        int edgeDn;
        int edgeUp;
    } tKeyInfo;

    static tKeyInfo keyInfo[256];
    static tKeyInfo skeyInfo[256];
    static tdble lastKeyUpdate;
    
    vector<tControlCmd> listenKeys;
};

#endif // DRIVERFEEDBACK_H
