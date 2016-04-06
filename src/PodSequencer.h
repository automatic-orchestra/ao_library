/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 PodSequencer.h
 Copyright (c) 2016 Automatic Orchestra

 See the COPYRIGHT file at the top-level directory of this distribution and at:
 https://github.com/automatic-orchestra/library-for-arduino/blob/master/COPYRIGHT.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 This file is part of Automatic Orchestra. It is subject to the license terms
 in the LICENSE file found in the top-level directory of this distribution and at:
 https://github.com/automatic-orchestra/library-for-arduino/blob/master/LICENSE.

 No part of Automatic Orchestra, including this file, may be copied, modified,
 propagated, or distributed except according to the terms contained in the
 LICENSE file.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#ifndef PODSEQUENCER_H
#define	PODSEQUENCER_H

#include "Pod.h"

class PodSequencer : public Pod {
public:

    PodSequencer(AutomaticOrchestra* pParent) : Pod(pParent) {
    }

    virtual ~PodSequencer() {
    }

    virtual void onClockBeatChange(unsigned long beat) {
        const int mBeat = beat % PATTERN_LEN; /* just in case, shouldn t be a problem though */
        const int mNotePitch = getPitch(mBeat);
        const int mNoteVelocity = mVelocities[mBeat];
        switch (mPitches[mBeat]) {
            case NON:
                playNoteOff();
                break;
            case CONT:
                break;
            default:
                playNoteOn(mNotePitch, mNoteVelocity);
                sendNoteOn(getParent()->getChannel(), mNotePitch, mNoteVelocity);
        }
    }

    virtual void onClockBarChange(unsigned long bar) {
    }

    const static int NON = -127;

    const static int CONT = -126;

protected:
    const static int PATTERN_LEN = 16;
    int mPitches[PATTERN_LEN] = {
        NON, NON, NON, NON,
        NON, NON, NON, NON,
        NON, NON, NON, NON,
        NON, NON, NON, NON,
    };
    int mVelocities[PATTERN_LEN] = {
        127, 127, 127, 127,
        127, 127, 127, 127,
        127, 127, 127, 127,
        127, 127, 127, 127,
    };

    virtual int getPitch(int i) {
        return mPitches[i];
    }

    void setPitches(int pSequence[]) {
        for (int i = 0; i < PATTERN_LEN; i++) {
            mPitches[i] = pSequence[i];
        }
    }

    void setVelocities(int pVelocity[]) {
        for (int i = 0; i < PATTERN_LEN; i++) {
            mVelocities[i] = pVelocity[i];
        }
    }

    void setAllVelocities(int pVelocity) {
        for (int i = 0; i < PATTERN_LEN; i++) {
            mVelocities[i] = pVelocity;
        }
    }
};

#endif //PODSEQUENCER_H

