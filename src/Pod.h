/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Pod.h
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

#ifndef POD_H
#define	POD_H

#include "Orchestra.h"
#include "CFOMidi.h"
#include "CFOSynthesizer.h"
#include "MidiNote.h"

class Pod {
public:

    Pod(Orchestra* pParent) { mParent = pParent; }
    virtual ~Pod() {}

    virtual void receiveNoteOn(byte channel, byte note, byte velocity);
    virtual void receiveNoteOff(byte channel, byte note, byte velocity);
    virtual void receiveControlChange(byte channel, byte control, byte value);
    virtual void onClockBarChange(unsigned long bar) {}
    virtual void onClockBeatChange(unsigned long beat) {}
    virtual void loop() {}

protected:


    void sendNoteOn(MidiNote* note);
    void sendNoteOn(byte channel, byte note, byte velocity);
    void sendNoteOff(MidiNote* note);
    void sendNoteOff(byte channel, byte note, byte velocity);
    void sendControlChange(uint8_t channel, uint8_t number, uint8_t value);
    virtual void playNoteOn(byte note, byte velocity);
    void playNoteOn(MidiNote* note);
    void playNoteOff(MidiNote* note);
    void playNoteOffByte(byte note);
    void playNoteOff();
    void applyPresets(const uint8_t pProgramPresets[]);

    Orchestra* getParent() { return mParent; }
    Orchestra* mParent;
};

#endif //POD_H
