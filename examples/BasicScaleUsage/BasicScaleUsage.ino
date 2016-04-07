/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 BasicScaleUsage.ino
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

#include "CFOSynthesizer.h"
#include "Scale.h"

static const int STEP_DURATION_MILLISECS = 1000 / (120 * 4 / 60); // for 120 bpm
static const int PATTERN_LEN = 16;
static const int NON = -127;

int mStepCounter = 0;
int mLastPitch = NON;
int mPitches[PATTERN_LEN] = {
        NON, 0, NON, 3,
        NON, 6, NON, 2,
        NON, 7, NON, 5,
        NON, -1, NON, 12,
};


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
    int pitchOffset = mPitches[mStepCounter];

    if(pitchOffset != NON) {
        // get pitch based on scale.
        mLastPitch = getPitch(pitchOffset);
        // start playback of current note.
        Music.noteOn(mLastPitch, 127);
    }

    if(pitchOffset == NON && mLastPitch != NON){
        // stop playback of current note.
        Music.noteOff(mLastPitch);
        // reset last pitch buffer.
        mLastPitch = NON;
    }

    // increment step counter (incl. safe guard for out of bounds read access).
    ++mStepCounter %= PATTERN_LEN;

    // delay loop based on step duration.
    delay(STEP_DURATION_MILLISECS);
}

int getPitch(int pPitchOffset) {
    // The Scale class provides the functionality to calculate notes based on musical scales.
    // The pitch is calculated based on a base note and and offset with a given scale.
    // The offset value can be negative or even greater than the number steps in the scale scale - see mPitches array.
    // The Scale class compensates theses offset and returns the correct pitch.
    // The predefined scales and base notes can be found in Scale.h.
    return Scale::note(Scale::MINOR_PENTATONIC, Scale::MINOR_PENTATONIC_LEN, Scale::NOTE_C4, pPitchOffset);
}