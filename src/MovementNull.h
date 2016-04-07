/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 MovementNull.h
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

#ifndef MOVEMENTNULL_H
#define	MOVEMENTNULL_H

#include "Pod.h"
#include "Movement.h"

class MovementNull : public Movement {
public:

    MovementNull(AutomaticOrchestra* pParent, int pNextMovement) : Movement(pParent, pNextMovement) {
        mPod = new Pod(pParent);
    }

    virtual ~MovementNull() {
        delete mPod;
    }

    void loop() {
    }

    Pod* getPod() {
        return mPod;
    }

    String getName() {
        return "MovementNull (MN)";
    }

    int isFinished() {
        if(parent()->isKlockMeister()) {
            return mBeatCounter >= 8 ? getNextMovement() : MOVEMENT_DO_NOT_CHANGE;
        } else {
            return MOVEMENT_DO_NOT_CHANGE;
        }
    }

    void onClockBeatChange(unsigned long pBeat) {
        mBeatCounter = pBeat;
        Serial.printf("### (MN) beat: %i\n", pBeat);
    }

private:

    Pod* mPod;
    int mBeatCounter = 0;
};

#endif //MOVEMENTNULL_H

