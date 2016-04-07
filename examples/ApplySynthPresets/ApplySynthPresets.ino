/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 ApplySynthPresets.ino
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

#include "CFOMidi.h"
#include "CFOSynthesizer.h"

int MAX_NOTE_DURATION = 500;

const uint8_t mSequencerPreset[] = {
        23, 6, 0, 0, 27, 0, 127, 0, 0, 3, 0, 88, 64, 85, 5, 18, 0, 2, 2, 0, 0, 76, 81, 127, 3, 9, 0, 0, 2, 0, 0, 64, 51, 127, 3, 126, 0, 2, 2, 0, 5, 0, 0, 0, 0, 36, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 111, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 64, 0, 64, 0, 0, 0, 0, 0, 1, 3, 31, 0, 64,
};

uint16_t c = 0;

void setup() {
    Music.init();
    Music.enableEnvelope1();
    Music.enableEnvelope2();
    applyPresets(mSequencerPreset);
    delay(1000);
}

void loop() {
    int mRandomNote = 50 + random(0, 5) * 7;
    int mRandomDuration = random(MAX_NOTE_DURATION);

    /* play a note. pitch is conform with midi protocol */
    Music.noteOn(mRandomNote, 127);
    delay(mRandomDuration);

    /* turn off a note. since this is a monophonic synth, it is not required to specify the actual pitch */
    Music.noteOff(mRandomNote);
    delay(MAX_NOTE_DURATION - mRandomDuration);
}

void applyPresets(const uint8_t pProgramPresets[]) {
    for (uint8_t i = 2; i < 128; i++) {
        Midi.controller(0, i, pProgramPresets[i]);
    }
}