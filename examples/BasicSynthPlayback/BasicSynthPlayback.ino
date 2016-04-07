/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 BasicSynthPlayback.ino
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

#include "CFOSynthesizer.h"

int MAX_NOTE_DURATION = 500;


void setup() {
    // initialize synthesizer.
    Music.init();
    //TODO evaluate if envelopes are needed.
    Music.enableEnvelope1();
    Music.enableEnvelope2();
    //TODO evaluate if delay is needed (when Midi initialization is removed from synth).
    delay(1000);
}

void loop() {
    int mRandomNote = 50 + random(0, 5) * 7;
    int mRandomDuration = random(MAX_NOTE_DURATION);

    // start playback of current note.
    Music.noteOn(mRandomNote, 127);
    delay(mRandomDuration);

    // stop playback of current note.
    Music.noteOff(mRandomNote);
    delay(MAX_NOTE_DURATION - mRandomDuration);
}