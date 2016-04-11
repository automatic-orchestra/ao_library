/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Orchestra.cpp
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

#include "Orchestra.h"
#include "Movement.h"
#include "Playlist.h"
#include "Arrangement.h"
#include "OrchestraMessages.h"

#define LED_PIN 13


Orchestra::Orchestra() {
    setupLED();
    // init variable here, since Movement is imported only in the implementation class.
    mChangeMovementFromControlMessage = Movement::MOVEMENT_DO_NOT_CHANGE;
}


Orchestra::~Orchestra() {
    delete mPlaylist;
    mPlaylist = nullptr;
    delete mCurrentMovement;
    mCurrentMovement = nullptr;
}


void Orchestra::setupPlaylist(Playlist* pPlaylist) {
    mPlaylist = pPlaylist;
    mPlaylist->setParent(this);
}


void Orchestra::start() {
    if(mPlaylist == nullptr){
        Serial.println("(O) -> start() - ERROR: call setupPlaylist() before calling start()!");
        return;
    }
}


void Orchestra::update() {
    // change movement from control message
    if (mChangeMovementFromControlMessage != Movement::MOVEMENT_DO_NOT_CHANGE) {
        changeMovement(mChangeMovementFromControlMessage);
        mChangeMovementFromControlMessage = Movement::MOVEMENT_DO_NOT_CHANGE;
    }
    // set LED state
    if (mLEDState > 0) {
        mLEDState--;
        if (mLEDState <= 0) {
            turnOffLED();
        }
    }
}


String Orchestra::getMacAddress() {
    return mMacAddress;
}


int Orchestra::getChannel() {
    return mMidiChannel;
}


bool Orchestra::isKlockMeister() {
    return mKlockMeister;
}


void Orchestra::printConfiguration() {
    Serial.println("----------------------------------");
    Serial.println("       DEVICE CONFIGURATION       ");
    Serial.println("----------------------------------");
    Serial.print(" Mac Address:   ");
    Serial.println(mMacAddress);
    Serial.printf(" MIDI Channel:  %i\n", mMidiChannel);
    Serial.printf(" Klock Meister: %i\n", mKlockMeister);
    Serial.println("----------------------------------");
}


void Orchestra::turnOnLED() {
    digitalWrite(LED_PIN, HIGH);
    mLEDState = LED_DURATION_LOOP_CYCLES;
}


void Orchestra::turnOffLED() {
    digitalWrite(LED_PIN, LOW);
    mLEDState = 0;
}


void Orchestra::setupLED() {
    pinMode(LED_PIN, OUTPUT);
    turnOnLED();
}


void Orchestra::onNoteOn(byte channel, byte note, byte velocity) {
}


void Orchestra::onNoteOff(byte channel, byte note, byte velocity) {
}


void Orchestra::onControlChange(byte channel, byte control, byte value) {
    // evaluate incoming message
    switch (control) {
        case OrchestraMessages::CHANGE_MOVEMENT:
            mChangeMovementFromControlMessage = value;
            Serial.printf("(O) -> onControlChange(): schedule movement change to %i.\n", mChangeMovementFromControlMessage);
            break;
    }
    // forward messages to movement
    if (mCurrentMovement) {
        mCurrentMovement->onControlChange(channel, control, value);
    }
}


void Orchestra::onClockTick() {
}


void Orchestra::onClockStart() {
}


void Orchestra::sendChangeMovement(int pMovementID) {
    Midi.sendController(getChannel(), OrchestraMessages::CHANGE_MOVEMENT, pMovementID);
}


Movement* Orchestra::getMovement(int pMovementID) {
    return mPlaylist->getMovement(pMovementID);
}


void Orchestra::changeMovement(int pMovementID) {
    Serial.printf("(O) -> changeMovement(): change to movement %i\n", pMovementID);

    // https://en.wikipedia.org/wiki/Delete_(C%2B%2B)
    if (mCurrentMovement) {
        Serial.print("(O) -> changeMovement(): deleting movement: ");
        Serial.println(mCurrentMovement->getName());
        delete mCurrentMovement;
        mCurrentMovement = nullptr;
    }

    mCurrentMovement = getMovement(pMovementID);
    if (mCurrentMovement) {
        Serial.print("(O) -> changeMovement(): created movement: ");
        Serial.print(mCurrentMovement->getName());
        Serial.print("  --  at address: 0x");
        Serial.println((int) (mCurrentMovement), HEX);
    } else {
        Serial.printf("(O) -> changeMovement(): ERROR: no movement created! Check playlist for logic related to movement id %i\n.", pMovementID);
    }
}


void Orchestra::setupDeviceParameters(String pMacAddress) {
    // save mac address
    mMacAddress = pMacAddress;

    // select the default arrangement setup.
    Arrangement::getInstance().init();

    // parse member array for matching configuration based on the device's MAC address
    orchestra_member_t* mList = Arrangement::getInstance().getList();
    if (mList != NULL) {
        for (unsigned int i = 0; i < Arrangement::getInstance().getSize(); i++) {
            if (pMacAddress.equals(mList[i].macAddress)) {
                mMidiChannel = mList[i].midiChannel;
                mKlockMeister = mList[i].klockMeister;
            }
        }
    }
}