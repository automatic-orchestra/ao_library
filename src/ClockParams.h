/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 ClockParams.h
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

#ifndef CLOCKPARAMS_H
#define CLOCKPARAMS_H

//TODO unify location of this definition
// also defined in Sequencer.h - line 30
#ifndef TICKS_PER_QUARTER_NOTE
#define TICKS_PER_QUARTER_NOTE 24
#endif

//TODO unify location of this definitions
// Constants defined for MIDI Clock
// also defined in CFOSynthesizer.h - line 147-150
#ifndef MIDI_CLOCK
#define MIDI_CLOCK 0xF8     // 248
#endif
#ifndef MIDI_START
#define MIDI_START 0xFA     // 250;
#endif
#ifndef MIDI_CONTINUE
#define MIDI_CONTINUE 0xFB  // 251;
#endif
#ifndef MIDI_STOP
#define MIDI_STOP 0xFC      // 252;
#endif

enum clock_tick_count_t {
    COUNT_1 = (TICKS_PER_QUARTER_NOTE * 4), // 96
    COUNT_1DOT = (TICKS_PER_QUARTER_NOTE * 3), // 72
    COUNT_2 = (TICKS_PER_QUARTER_NOTE * 2), // 48
    COUNT_3 = ((TICKS_PER_QUARTER_NOTE * 3) / 2), // 36
    COUNT_4 = (TICKS_PER_QUARTER_NOTE * 1), // 24
    COUNT_6 = ((TICKS_PER_QUARTER_NOTE * 2) / 3), // 16
    COUNT_8 = (TICKS_PER_QUARTER_NOTE / 2), // 12
    COUNT_12 = (TICKS_PER_QUARTER_NOTE / 3), //  8
    COUNT_16 = (TICKS_PER_QUARTER_NOTE / 4), //  6
    COUNT_24 = (TICKS_PER_QUARTER_NOTE / 6), //  4
    COUNT_32 = (TICKS_PER_QUARTER_NOTE / 8), //  3
    COUNT_48 = (TICKS_PER_QUARTER_NOTE / 12), //  2
    //COUNT_64 = (TICKS_PER_QUARTER_NOTE / 16),	//  1.5 <-- PROBLEMATIC / NO HALF TICKS
    COUNT_96 = (TICKS_PER_QUARTER_NOTE / 24) //  1
};

#endif //CLOCKPARAMS_H