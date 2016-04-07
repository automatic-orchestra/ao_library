/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Arrangement.cpp
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

#include "Arrangement.h"


// Code for singleton derived from here:
// http://stackoverflow.com/questions/270947/can-any-one-provide-me-a-sample-of-singleton-in-c/271104#271104
Arrangement& Arrangement::getInstance() {
    // create static instance of Arrangement
    static Arrangement instance;
    return instance;
}


void Arrangement::init(uint8_t pSize) {
    // load default mac addresses
    String macAddresses[] = {
        #include "inc/MacAddresses.inc"
    };

    init(macAddresses, pSize);
}


void Arrangement::init(String macAddresses[], uint8_t pSize) {
    // only initialize once!
    if(mSize != 0) {
        return;
    }

    if(pSize == 0){
        mSize = DEFAULT_SIZE;
    } else {
        mSize = pSize;
    }

    mList = new orchestra_member_t[mSize];
    // MAC Address, MIDI Channel, Klockmeister
    for(unsigned int i = 0; i < mSize; i++) {
        mList[i] = {
            macAddresses[i], // MAC Address
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

uint8_t Arrangement::getSize() {
    return mSize;
}
