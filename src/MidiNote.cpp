/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 MidiNote.cpp
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

#include "MidiNote.h"

MidiNote::MidiNote() {
    // nothing to do...
}

MidiNote::MidiNote(unsigned int channel, unsigned int key, unsigned int velocity) {
    setChannel(channel);
    setKey(key);
    setVelocity(velocity);
    setDurationInBeats(0);
    resetBeatCounter();
}

MidiNote::MidiNote(unsigned int channel, unsigned int key, unsigned int velocity, unsigned int durationInBeats) {
    setChannel(channel);
    setKey(key);
    setVelocity(velocity);
    setDurationInBeats(durationInBeats);
    resetBeatCounter();
}

void MidiNote::setChannel(unsigned int channel) {
    if (channel >= 0 && channel <= 15) {
        mChannel = channel;
    } else {
        Serial.println("ERROR in MidiNote::setChannel(): Channel must be within the range of 0 and 15.");
    }
}

void MidiNote::setKey(unsigned int key) {
    if (key >= 0 && key <= 127) {
        mKey = key;
    } else {
        Serial.println("ERROR in MidiNote::setKey(): Key must be within the range of 0 and 127.");
    }
}

void MidiNote::setVelocity(unsigned int velocity) {
    if (velocity >= 0 && velocity <= 127) {
        mVelocity = velocity;
    } else {
        Serial.println("ERROR in MidiNote::setVelocity(): Velocity must be within the range of 0 and 127.");
    }
}

void MidiNote::setDurationInBeats(unsigned int durationInBeats) {
    mDurationInBeats = durationInBeats;
}

unsigned int MidiNote::getChannel() {
    return mChannel;
}

unsigned int MidiNote::getKey() {
    return mKey;
}

unsigned int MidiNote::getVelocity() {
    return mVelocity;
}

unsigned int MidiNote::getDurationInBeats() {
    return mDurationInBeats;
}

void MidiNote::incrementBeatCounter() {
    if (mDurationInBeats != 0) {
        mBeatCounter++;
    }
}

void MidiNote::resetBeatCounter() {
    mBeatCounter = 0;
}

bool MidiNote::isDone() {
    if (mDurationInBeats == 0) {
        return false;
    } else {
        return (mBeatCounter >= mDurationInBeats);
    }
}

void MidiNote::printDescription() {
    Serial.printf("[MidiNote( key:%d, velocity:%d, duration:%d )]", mKey, mVelocity, mDurationInBeats);
}