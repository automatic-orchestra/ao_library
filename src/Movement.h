/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Movement.h
 Copyright (c) 2016 Automatic Orchestra

 See the COPYRIGHT file at the top-level directory of this distribution and at:
 https://github.com/automatic-orchestra/ao_library/blob/master/COPYRIGHT.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 This file is part of Automatic Orchestra. It is subject to the license terms
 in the LICENSE file found in the top-level directory of this distribution and at:
 https://github.com/automatic-orchestra/ao_library/blob/master/LICENSE.

 No part of Automatic Orchestra, including this file, may be copied, modified,
 propagated, or distributed except according to the terms contained in the
 LICENSE file.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#ifndef MOVEMENT_H
#define	MOVEMENT_H

#include "Orchestra.h"
#include "Pod.h"

//#define M_DEBUG
// When debugging the movements with a shorter duration this
// debug flag needs only be uploaded to the klockmeister device
#define M_DURATION_DEBUG false

class Movement {
public:
    /* constants for movements */
    const static int MOVEMENT_DO_NOT_CHANGE = -1;
    const static int MOVEMENT_NULL = 0;
#if M_DURATION_DEBUG
    const static int DEFAULT_MOVEMENT_DURATION = 8;
#else
    const static int DEFAULT_MOVEMENT_DURATION = 128;
#endif

    //TODO remove necessity to pass in id of next movement and store this in playlist.
    Movement(Orchestra* pParent, int pNextMovement);
    virtual ~Movement() {}

    virtual Pod* getPod() = 0;
    virtual String getName() = 0;

    virtual void loop();
    virtual void onNoteOn(byte channel, byte note, byte velocity);
    virtual void onNoteOff(byte channel, byte note, byte velocity);
    virtual void onControlChange(byte channel, byte control, byte value);
    //TODO add bar and beat count to base movement implementation?
    virtual void onClockBarChange(unsigned long bar);
    virtual void onClockBeatChange(unsigned long beat);
    //TODO add better default behavior than not to change at all...
    virtual int isFinished() { return MOVEMENT_DO_NOT_CHANGE; }

protected:

    int channel() { return mChannel; }
    Orchestra* parent() { return getParent(); }
    Orchestra* getParent() { return mParent; }
    int getNextMovement() { return mNextMovement; }
    void setMovementDuration(unsigned int pDurationInBars) { mMovementDuration = pDurationInBars; }
    unsigned int getMovementDuration() { return mMovementDuration; }

private:
    Orchestra* mParent;
    int mChannel;
    int mNextMovement = 0;
    unsigned int mMovementDuration = DEFAULT_MOVEMENT_DURATION;

};

#endif //MOVEMENT_H
