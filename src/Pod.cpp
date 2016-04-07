/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Pod.cpp
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

#include "Pod.h"

void Pod::receiveNoteOn(byte channel, byte note, byte velocity) {
    //        Serial.printf("Pod::receiveNoteOn() -- channel: @i", channel);
    if (channel != mParent->getChannel()) {
        sendNoteOn(channel, note, velocity);
    }
}

void Pod::receiveNoteOff(byte channel, byte note, byte velocity) {
    //        Serial.printf("Pod::receiveNoteOff() -- channel: @i", channel);
    if (channel != mParent->getChannel()) {
        sendNoteOff(channel, note, velocity);
    }
}

void Pod::receiveControlChange(byte channel, byte control, byte value) {
    //Serial.printf("Pod::receiveControlChange() -- channel: %d, control: %d\n", channel, control);
    if (channel != mParent->getChannel()) {
        sendControlChange(channel, control, value);
    }
    // Is the below still a problem?
    /*
     * TODO causes problems when changing movements?
     else {
        Midi.controller(channel, control, value);
    }*/
}

void Pod::sendNoteOn(MidiNote* note) {
    sendNoteOn(note->getChannel(), note->getKey(), note->getVelocity());
}

void Pod::sendNoteOn(byte channel, byte note, byte velocity) {
    Midi.sendNoteOn(channel, note, velocity);
}

void Pod::sendNoteOff(MidiNote* note) {
    sendNoteOff(note->getChannel(), note->getKey(), note->getVelocity());
}

void Pod::sendNoteOff(byte channel, byte note, byte velocity) {
    Midi.sendNoteOff(channel, note, velocity);
}

void Pod::sendControlChange(uint8_t channel, uint8_t number, uint8_t value) {
    Midi.sendController(channel, number, value);
}

void Pod::playNoteOn(MidiNote* note) {
//        Serial.printf("playNoteOn - key: %d - velocity: %d\n", note->getKey(), note->getVelocity());
    playNoteOn(note->getKey(), note->getVelocity());
}

void Pod::playNoteOn(byte note, byte velocity) {
    Music.noteOn(note, velocity);
    mParent->turnOnLED();
}

void Pod::playNoteOff(MidiNote* note) {
//        Serial.printf("playNoteOff - key: %d\n", note->getKey());
    playNoteOffByte(note->getKey());
}

void Pod::playNoteOffByte(byte note) {
    Music.noteOff(note);
    mParent->turnOffLED();
}

void Pod::playNoteOff() {
    Music.noteOff();
    mParent->turnOffLED();
}

void Pod::applyPresets(const uint8_t pProgramPresets[]) {
    for (uint8_t i = 2; i < 128; i++) {
        if (i >= AutomaticOrchestra::AO_CONTROL_MESSAGES &&
                i <= AutomaticOrchestra::AO_CONTROL_MESSAGES_MAX) {
            continue;
        }
        Midi.controller(mParent->getChannel(), i, pProgramPresets[i]);
        usbMIDI.sendControlChange(i, pProgramPresets[i], mParent->getChannel() + 1);
    }
}
