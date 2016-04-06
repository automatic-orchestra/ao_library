/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 AutomaticOrchestra.h
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

#ifndef AUTOMATICORCHESTRA_H
#define AUTOMATICORCHESTRA_H

#include <Arduino.h>

class Movement;
class Clock;
class Playlist;

//TODO add global volume control!!!!
class AutomaticOrchestra {
public:
    AutomaticOrchestra(Clock* pClock, Playlist* pPlaylist);
    void start();
    void loop();
    String getMacAddress();
    bool isKlockMeister();

    int getChannel() {
        return mMidiChannel;
    }

    Clock* getClock() {
        return mClock;
    }
    void changeMovement(int pMovementID);
    /* receive events from network */
    void onNoteOn(byte channel, byte note, byte velocity);
    void onNoteOff(byte channel, byte note, byte velocity);
    void onControlChange(byte channel, byte control, byte value);
    /* receive events from clock */
    void onClockBarChange(unsigned long bar);
    void onClockBeatChange(unsigned long beat);

    void sendChangeMovement(int mMovementID);
    void sendChangeBPM(unsigned int pBPM);
    void sendEnumeratePods(unsigned int pStartCount);
    void sendPodCount(unsigned int pPodCount);
    void sendRootNote(int pRootNote);
    void sendShift(int pValue);

    void killNotes();

    void turnOnLED() {
        digitalWrite(13, HIGH);
        mLEDState = LED_DURATION_LOOP_CYCLES;
    }

    void turnOffLED() {
        digitalWrite(13, LOW);
        mLEDState = 0;
    }

    const static int AO_CONTROL_MESSAGES = 40;
    const static int AO_CONTROL_MESSAGE_CHANGE_MOVEMENT = AO_CONTROL_MESSAGES + 0;
    const static int AO_CONTROL_MESSAGE_CHANGE_BPM = AO_CONTROL_MESSAGES + 1;
    const static int AO_CONTROL_MESSAGE_ENUMERATE_PODS = AO_CONTROL_MESSAGES + 2;
    const static int AO_CONTROL_MESSAGE_POD_COUNT = AO_CONTROL_MESSAGES + 3;
    const static int AO_CONTROL_MESSAGE_SCALE = AO_CONTROL_MESSAGES + 4;
    const static int AO_CONTROL_MESSAGE_ROOT_NOTE = AO_CONTROL_MESSAGES + 5;
    const static int AO_CONTROL_MESSAGE_SHIFT = AO_CONTROL_MESSAGES + 6;
    const static int AO_CONTROL_MESSAGES_MAX = 69;

private:

    void getIDFromMacAddress(String* pMacAddress, int * pMidiID, bool * pKlockMeister);
    Movement* getMovement(int pMovementID);

    void setupLED() {
        pinMode(13, OUTPUT);
        turnOnLED();
    }

    String mMacAddress;
    int mMidiChannel;
    bool mKlockMeister;
    Clock* mClock;
    Playlist* mPlaylist;
    Movement* mCurrentMovement;

    const static int LED_DURATION_LOOP_CYCLES = 100;
    int mLEDState = 0;

    int mChangeMovementFromControlMessage;

    bool mWaitingFirstBest = true;
};

#endif //AUTOMATICORCHESTRA_H
