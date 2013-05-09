#include "DriverFeedback.hpp"
#include <bib/Logger.hpp>

bool DriverFeedback::first_time = true;
int DriverFeedback::currentKey[256];
int DriverFeedback::currentSKey[256];
DriverFeedback::tKeyInfo DriverFeedback::keyInfo[256];
DriverFeedback::tKeyInfo DriverFeedback::skeyInfo[256];
tdble DriverFeedback::lastKeyUpdate = -10.0;

DriverFeedback::DriverFeedback(int index, int intervalAction, float simu_time, const vector<tControlCmd>& listenKeys) : Driver(index, intervalAction, simu_time), listenKeys(listenKeys)
{
    memset(keyInfo, 0, sizeof(keyInfo));
    memset(skeyInfo, 0, sizeof(skeyInfo));

    memset(currentKey, 0, sizeof(currentKey));
    memset(currentSKey, 0, sizeof(currentSKey));
}

void DriverFeedback::update(tSituation *s) {

    Driver::update(s);
    
    if(first_time ) {
	int scrw, scrh, dummy;
	GfScrGetSize(&scrw, &scrh, &dummy, &dummy);
	
	if(scrw != 0 && scrh != 0){//if actived screen
            GfuiKeyEventRegisterCurrent(onKeyAction);
            GfuiSKeyEventRegisterCurrent(onSKeyAction);
	}

        first_time = false;
    }

    if (lastKeyUpdate != s->currentTime) {
        /* Update the controls only once for all the players */
        updateKeys();
        lastKeyUpdate = s->currentTime;
    }

}

int DriverFeedback::onKeyAction(unsigned char key, int modifier, int state)
{
//     LOG_DEBUG("keyAction" << key << " st " << state << " mo " <<modifier);
    (void) modifier;
    DriverFeedback::currentKey[key] = state;
    return 0;
}

int DriverFeedback::onSKeyAction(int key, int modifier, int state)
{
//     LOG_DEBUG("keyActionS" << key << " st " << state << " mo " <<modifier);
    (void) modifier;
    currentSKey[key] = state;
    return 0;
}


void DriverFeedback::updateKeys()
{
    int key;
//     LOG_DEBUG("update key");

    for (unsigned int i = 0; i < listenKeys.size(); i++) {
        if (listenKeys[i].type == GFCTRL_TYPE_KEYBOARD) {
            key = listenKeys[i].val;
// 	    LOG_DEBUG(key << " " << GFUI_KEY_DOWN << " " << GFUI_KEY_UP);
            if (currentKey[key] == GFUI_KEY_DOWN) {
                if (keyInfo[key].state == GFUI_KEY_UP) {
                    keyInfo[key].edgeDn = 1;
                } else {
                    keyInfo[key].edgeDn = 0;
                }
            } else {
                if (keyInfo[key].state == GFUI_KEY_DOWN) {
                    keyInfo[key].edgeUp = 1;
                } else {
                    keyInfo[key].edgeUp = 0;
                }
            }
            keyInfo[key].state = currentKey[key];
        }

        if (listenKeys[i].type == GFCTRL_TYPE_SKEYBOARD) {
            key = listenKeys[i].val;
            if (currentSKey[key] == GFUI_KEY_DOWN) {
                if (skeyInfo[key].state == GFUI_KEY_UP) {
                    skeyInfo[key].edgeDn = 1;
                } else {
                    skeyInfo[key].edgeDn = 0;
                }
            } else {
                if (skeyInfo[key].state == GFUI_KEY_DOWN) {
                    skeyInfo[key].edgeUp = 1;
                } else {
                    skeyInfo[key].edgeUp = 0;
                }
            }
            skeyInfo[key].state = currentSKey[key];
        }
    }

}

