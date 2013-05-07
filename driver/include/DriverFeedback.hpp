#ifndef DRIVERFEEDBACK_H
#define DRIVERFEEDBACK_H

#include "Driver.hpp"
#include <vector>
#include <tgfclient.h>

using std::vector;

class DriverFeedback : public Driver
{
public:
    typedef struct {
        int		type;		/* Type of control (analog, joy button, keyboard) */
        int		val;		/* control index */
    } tControlCmd;

    DriverFeedback(int index, int intervalAction, float nbLaps,const vector<tControlCmd>& listenKeys);

    void update(tSituation *s);
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
