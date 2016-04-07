/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Movement.cpp
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

#include "Movement.h"


Movement::Movement(AutomaticOrchestra* pParent, int pNextMovement)
: mChannel(pParent->getChannel()) {
    mParent = pParent;
    mNextMovement = pNextMovement;
    randomSeed(analogRead(0));
#if M_DURATION_DEBUG
    Serial.print("### (M) -> debug with default duration of ");
    Serial.print(DEFAULT_MOVEMENT_DURATION);
    Serial.println(" bars");
#endif
}



void Movement::loop() {
    Pod* mPod = getPod();
    if (mPod) {
        mPod->loop();
    }
}

void Movement::onNoteOn(byte channel, byte note, byte velocity) {
#ifdef M_DEBUG
    Serial.println("### (M) -> onNoteOn");
#endif
    Pod* mPod = getPod();
    if (mPod) {
        mPod->receiveNoteOn(channel, note, velocity);
    }
}

void Movement::onNoteOff(byte channel, byte note, byte velocity) {
#ifdef M_DEBUG
    Serial.println("### (M) -> onNoteOff");
#endif
    Pod* mPod = getPod();
    if (mPod) {
        mPod->receiveNoteOff(channel, note, velocity);
    }
}

void Movement::onControlChange(byte channel, byte control, byte value) {
    Pod* mPod = getPod();
    if (mPod) {
        mPod->receiveControlChange(channel, control, value);
    }
}

void Movement::onClockBarChange(unsigned long bar) {
#ifdef M_DEBUG
    Serial.println("### (M) -> onClockBarChange");
#endif
    Pod* mPod = getPod();
    if (mPod) {
        mPod->onClockBarChange(bar);
    }
}

void Movement::onClockBeatChange(unsigned long beat) {
#ifdef M_DEBUG
    Serial.println("### (M) -> onClockBeatChange");
#endif
    Pod* mPod = getPod();
    if (mPod) {
        mPod->onClockBeatChange(beat);
    }
}
