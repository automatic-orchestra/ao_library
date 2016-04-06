/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 PodInputPlayback.h
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

#ifndef PODINPUTPLAYBACK_H
#define	PODINPUTPLAYBACK_H

#include "Pod.h"

/**
 * Use this class with any Music software that can output MIDI
 * to hear the note playback.
 */
class PodInputPlayback : public Pod {
public:
    PodInputPlayback(AutomaticOrchestra* pParent) : Pod(pParent) {
    }
    
    void receiveNoteOn(byte channel, byte note, byte velocity) {
        playNoteOn(note, velocity);
    }
    
    void receiveNoteOff(byte channel, byte note, byte velocity) {
        playNoteOffByte(note);
    }
    
    void receiveControlChange(byte channel, byte control, byte value) {
        Midi.controller(channel, control, value);
    }
private:
};

#endif //PODINPUTPLAYBACK_H

