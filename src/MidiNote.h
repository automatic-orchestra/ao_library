/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 MidiNote.h
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

#ifndef MIDINOTE_H
#define MIDINOTE_H

#include <Arduino.h>

class MidiNote {
public:
	MidiNote();
	MidiNote( unsigned int channel, unsigned int key, unsigned int velocity );
	MidiNote( unsigned int channel, unsigned int key, unsigned int velocity, unsigned int durationInBeats );
	void setChannel( unsigned int channel );
	void setKey( unsigned int key );
	void setVelocity( unsigned int velocity );
	void setDurationInBeats( unsigned int durationInBeats );
	unsigned int getChannel();
	unsigned int getKey();
	unsigned int getVelocity();
	unsigned int getDurationInBeats();
	void incrementBeatCounter();
	void resetBeatCounter();
	bool isDone();
        void printDescription();
private:
	unsigned int mChannel = 0;
	unsigned int mKey = 0;
	unsigned int mVelocity = 0;
	unsigned int mDurationInBeats = 0;
	unsigned int mBeatCounter = 0;
};

#endif //MIDINOTE_H