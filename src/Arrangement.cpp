/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Arrangement.cpp
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

#include "Arrangement.h"

// Code for singleton derived from here:
// http://stackoverflow.com/questions/270947/can-any-one-provide-me-a-sample-of-singleton-in-c/271104#271104

Arrangement& Arrangement::getInstance() {
    static Arrangement instance;
    return instance;
}

void Arrangement::setupDefault() {
    String MacAddresses[] = {
// CREATE THIS FILE FROM MacAddresses.original IN THE FOLDER inc/
#include "inc/MacAddresses.inc"
    };

    mList = new orchestra_member_t[MEMBER_COUNT];
    // MAC Address, MIDI Channel, Klockmeister
    for(unsigned int i = 0; i < MEMBER_COUNT; i++) {
        mList[i] = {
            MacAddresses[i], // MAC Address
            i, // MIDI Channel:  starting at 0
            i == 0 // Klockmeister: always the first mac address is true, all others are false!!!
        };
    }
}

orchestra_member_t* Arrangement::getList() {
    if (mList == NULL) {
        Serial.println("Arrangement::getList() => execute a setup method before retrieving the list");
        return NULL;
    } else {
        return mList;
    }
}
