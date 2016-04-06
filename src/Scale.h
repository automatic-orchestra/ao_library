/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Scale.h
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

#ifndef SCALE_H
#define	SCALE_H

#include <Arduino.h>

class Scale {
public:
    static char test_array[];

    static int HALF_TONE[];
    static int FIFTH[];
    static int MINOR[];
    static int MAJOR[];
    static int MINOR_CHORD[];
    static int MAJOR_CHORD[];
    static int MINOR_CHORD_7[];
    static int MAJOR_CHORD_7[];
    static int MINOR_PENTATONIC[];
    static int MAJOR_PENTATONIC[];
    static int OCTAVE[];
    static int DIMINISHED[];
    static int FIBONACCI[];

    static const int HALF_TONE_LEN = 12;
    static const int FIFTH_LEN = 2;
    static const int MINOR_LEN = 7;
    static const int MAJOR_LEN = 7;
    static const int MINOR_CHORD_LEN = 3;
    static const int MAJOR_CHORD_LEN = 3;
    static const int MINOR_CHORD_7_LEN = 4;
    static const int MAJOR_CHORD_7_LEN = 4;
    static const int MINOR_PENTATONIC_LEN = 5;
    static const int MAJOR_PENTATONIC_LEN = 5;
    static const int OCTAVE_LEN = 1;
    static const int DIMINISHED_LEN = 4;
    static const int FIBONACCI_LEN = 6;

    static const int NOTE_A1 = 21;
    static const int NOTE_A2 = 33;
    static const int NOTE_A3 = 45;
    static const int NOTE_A4 = 57;
    static const int NOTE_A5 = 69;
    static const int NOTE_A6 = 81;
    static const int NOTE_A7 = 93;
    static const int NOTE_C1 = 12;
    static const int NOTE_C2 = 24;
    static const int NOTE_C3 = 36;
    static const int NOTE_C4 = 48;
    static const int NOTE_C5 = 60;
    static const int NOTE_C6 = 72;
    static const int NOTE_C7 = 84;

    static int note(int pScale[], int pScaleLength, int pBaseNote, int pNoteStepOffset) {
        if (pNoteStepOffset >= 0) {
            int i = pNoteStepOffset % pScaleLength;
            int mOctave = pNoteStepOffset / pScaleLength;
            return pBaseNote + mOctave * 12 + pScale[i];
        } else {
            int mOctave = (int) ceil(abs((float) pNoteStepOffset / pScaleLength)) - 1;
            int i = ((pScaleLength - 1) - abs((pNoteStepOffset + 1) % pScaleLength));
            int mOffset = 12 - pScale[i];
            return pBaseNote - mOffset - mOctave * 12;
        }
    }
};

#endif //SCALE_H

