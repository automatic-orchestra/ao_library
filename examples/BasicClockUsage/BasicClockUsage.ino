/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 BasicClockUsage.ino
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

#include "Clock.h"

// Varaible to hold pointer to Clock instance.
Clock* mClock;

// Forward declaration of clock callback functions.
// Alternative: Implement these function before the setup() function.
void onClockBarChange(unsigned long bar);
void onClockBeatChange(unsigned long beat);


void setup() {
    Serial.begin(9600);

    // wait for the serial interface to be initialized
    delay(500);

    // Create new clock.
    // In this example: 120bpm at 16 beats per bar.
    mClock = new Clock(120, COUNT_16);

    // Setup callback functions.
    mClock->setBarCallback(&onClockBarChange);
    mClock->setBeatCallback(&onClockBeatChange);

    // start the clock.
    mClock->start();
}

void loop() {
    // update the clock.
    mClock->update();
}

// Callback function for each bar.
void onClockBarChange(unsigned long bar) {
    Serial.print("bar: ");
    Serial.println(bar);
}

// Callback function for each beat.
void onClockBeatChange(unsigned long beat) {
    Serial.print("beat: ");
    Serial.println(beat);
}