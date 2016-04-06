/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 NoteSequencePlayback.ino
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

static const int STEP_DURATION_MILLISECS = 1000 / (120 * 4 / 60); // for 120 bpm
int mStepCounter = 0;
int mSequence[16] = {
        45, 45, 45, 45,
        45, 57, 45, 57,
        57, 69, 57, 69,
        45, 57, 45, 56,
};

void setup(){
    // initialize synthesizer.
    Music.init();
    //TODO evaluate if envelopes are needed.
    Music.enableEnvelope1();
    Music.enableEnvelope2();
    //TODO evaluate if delay is needed (when Midi initialization is removed from synth).
    delay(1000);
}

void loop(){
    // start playback of current note.
    Music.noteOn(mSequence[mStepCounter], 127);
    delay(STEP_DURATION_MILLISECS / 2);

    // stop playback of current note.
    Music.noteOff(mSequence[mStepCounter]);
    delay(STEP_DURATION_MILLISECS / 2);

    // increment step counter (incl. safe guard for out of bounds read access).
    ++mStepCounter %= 16;
}