/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 AutomaticOrchestra.cpp
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

#include <memory>
#include "AutomaticOrchestra.h"
#include "Clock.h"
#include "Playlist.h"
#include "CFOMidi.h"
#include "CFOSynthesizer.h"
#include "MacAddress.h"
#include "MovementNull.h"
#include "Scale.h"

#define AO_DEBUG


// ////////////////////////////////////////
// Constructors
// ////////////////////////////////////////

AutomaticOrchestra::AutomaticOrchestra(Clock* pClock, Playlist* pPlaylist) :
mClock(pClock), mPlaylist(pPlaylist), mCurrentMovement(0),
mChangeMovementFromControlMessage(Movement::MOVEMENT_DO_NOT_CHANGE) {
    mPlaylist->setParent(this);
    setupDeviceParameters(MacAddress::get());
}

// ////////////////////////////////////////
// Public Methods
// ////////////////////////////////////////

void AutomaticOrchestra::start() {
    // DO NOT CHANGE THE INITAL MOVEMENT HERE!!!
    // Set it in the getMovement() method when passing in the secodn parameter to movement null.
    sendChangeMovement(Movement::MOVEMENT_NULL);
    setupLED();
}

void AutomaticOrchestra::loop() {
    if (mCurrentMovement) {
        mCurrentMovement->loop();
        int mState = mCurrentMovement->isFinished();
        if (mState != Movement::MOVEMENT_DO_NOT_CHANGE) {
            sendChangeMovement(mState);
            delete mCurrentMovement;
            mCurrentMovement = nullptr;
        }
    }
    /* change movement from control message */
    if (mChangeMovementFromControlMessage != Movement::MOVEMENT_DO_NOT_CHANGE) {
        changeMovement(mChangeMovementFromControlMessage);
        mChangeMovementFromControlMessage = Movement::MOVEMENT_DO_NOT_CHANGE;
    }
    /* turn off LED if necessary */
    if (mWaitingFirstBest) {
        turnOnLED();
    } else {
        if (mLEDState > 0) {
            mLEDState--;
            if (mLEDState <= 0) {
                turnOffLED();
            }
        }
    }

}

void AutomaticOrchestra::sendChangeMovement(int mMovementID) {
    Midi.sendController(getChannel(), AutomaticOrchestra::AO_CONTROL_MESSAGE_CHANGE_MOVEMENT, mMovementID);
}

void AutomaticOrchestra::sendChangeBPM(unsigned int pBPM) {
    // as per convention only send half of the bpm you want.
    // this allows for bpm rate transmitted from 0 to 254.
    // due to the limitation that the value can only be 7 bit long => max. 127!
    if (pBPM > 254) {
        pBPM = 254;
        Serial.println("### (AO) Due to limitations of the current implementation the BPM rate can not get higher than 254.");
    }
    Midi.sendController(getChannel(), AutomaticOrchestra::AO_CONTROL_MESSAGE_CHANGE_BPM, (int) (pBPM / 2));
}

void AutomaticOrchestra::sendEnumeratePods(unsigned int pStartCount) {
    Midi.sendController(getChannel(), AO_CONTROL_MESSAGE_ENUMERATE_PODS, pStartCount);
}

void AutomaticOrchestra::sendPodCount(unsigned int pPodCount) {
    Midi.sendController(getChannel(), AO_CONTROL_MESSAGE_POD_COUNT, pPodCount);
}

void AutomaticOrchestra::sendRootNote(int pRootNote) {
    Midi.sendController(getChannel(), AO_CONTROL_MESSAGE_ROOT_NOTE, pRootNote);
}

void AutomaticOrchestra::sendShift(int pValue) {
    Midi.sendController(getChannel(), AO_CONTROL_MESSAGE_SHIFT, pValue);
}

String AutomaticOrchestra::getMacAddress() {
    return mMacAddress;
}

bool AutomaticOrchestra::isKlockMeister() {
    return mKlockMeister;
}

void AutomaticOrchestra::onNoteOn(byte channel, byte note, byte velocity) {
    if (mCurrentMovement) {
        mCurrentMovement->onNoteOn(channel, note, velocity);
    }
}

void AutomaticOrchestra::onNoteOff(byte channel, byte note, byte velocity) {
    turnOffLED();
    if (mCurrentMovement) {
        mCurrentMovement->onNoteOff(channel, note, velocity);
    }
}

void AutomaticOrchestra::onControlChange(byte channel, byte control, byte value) {
    // evaluate AO custom control messages
    switch (control) {
            // change movement from control message
        case AO_CONTROL_MESSAGE_CHANGE_MOVEMENT:
            mChangeMovementFromControlMessage = value;
#ifdef AO_DEBUG
            Serial.printf("### (AO) schedule movement change ( from network ) to '%i'.\n", mChangeMovementFromControlMessage);
#endif
            break;
            // change clocks bpm from control message
        case AO_CONTROL_MESSAGE_CHANGE_BPM:
            mClock->setBeatsPerMinute(value * 2);
#ifdef AO_DEBUG
            Serial.printf("### (AO) changed clocks bpm rate to '%d'.\n", mClock->getBeatsPerMinute());
#endif
            break;
    }

    if (mCurrentMovement) {
        mCurrentMovement->onControlChange(channel, control, value);
    }
}

void AutomaticOrchestra::onClockBarChange(unsigned long bar) {
    if (mCurrentMovement) {
        mCurrentMovement->onClockBarChange(bar);
    }
}

void AutomaticOrchestra::onClockBeatChange(unsigned long beat) {
    mWaitingFirstBest = false;
    if (mCurrentMovement) {
        mCurrentMovement->onClockBeatChange(beat);
    }
}


// ////////////////////////////////////////
// Private Methods
// ////////////////////////////////////////

Movement* AutomaticOrchestra::getMovement(int pMovementID) {
    return mPlaylist->getMovement(pMovementID);
}

void AutomaticOrchestra::changeMovement(int pMovementID) {
    Serial.printf("### (AO) change to movement : %i\n", pMovementID);

    // https://en.wikipedia.org/wiki/Delete_(C%2B%2B)
    if (mCurrentMovement) {
        Serial.print("### (AO) deleting movement: ");
        Serial.println(mCurrentMovement->getName());
        delete mCurrentMovement;
        mCurrentMovement = nullptr;
    }

    killNotes();

    // restart clock with each new movement
    if (mClock) {
        mClock->reset();
    }

    mCurrentMovement = getMovement(pMovementID);
    if (mCurrentMovement) {
        Serial.print("### (AO) created movement : ");
        Serial.print(mCurrentMovement->getName());
        Serial.print("  ---  at address: 0x");
        Serial.println((int) (mCurrentMovement), HEX);
    } else {
        Serial.println("ERROR: no movement created! Check getMovement() method for matching case statement.");
    }
}

void AutomaticOrchestra::killNotes() {
    for (int i = 0; i < 127; i++) {
        Music.noteOff(i);
    }
}
