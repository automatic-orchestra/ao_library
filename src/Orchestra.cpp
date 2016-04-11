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
#include "Arrangement.h"


Orchestra::Orchestra() {
}


Orchestra::~Orchestra() {
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


void Orchestra::onNoteOn(byte channel, byte note, byte velocity) {
}


void Orchestra::onNoteOff(byte channel, byte note, byte velocity) {
}


void Orchestra::onControlChange(byte channel, byte control, byte value) {
}


void Orchestra::onClockTick() {
}


void Orchestra::onClockStart() {
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