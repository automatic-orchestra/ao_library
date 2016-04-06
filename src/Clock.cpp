/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Clock.cpp
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

#include "Clock.h"
#include "CFOMidi.h"

//#define C_DEBUG


// ////////////////////////////////
// Constructors
// ////////////////////////////////

Clock::Clock() {
    setBeatsPerMinute(120);
    setTickCount(COUNT_4);
}

Clock::Clock(unsigned int pBPM, clock_tick_count_t pTickCount) {
    setBeatsPerMinute(pBPM);
    setTickCount(pTickCount);
}

// ////////////////////////////////
// Public Methods
// ////////////////////////////////

void Clock::setBeatsPerMinute(unsigned int pBPM) {
    if (pBPM > 0 && mBPM != pBPM) {
        mTickTime = (60 * 1000000) / (pBPM * TICKS_PER_QUARTER_NOTE);
        mBPM = pBPM;
#ifdef C_DEBUG
        Serial.printf("### (C) new bpm: %d", mBPM);
#endif
    }
}

void Clock::setTickCount(clock_tick_count_t pTickCount) {
    //TODO refactor clock tick count setup
    switch (pTickCount) {
        case COUNT_1:
            mBeatsPerBar = 1;
            break;
        case COUNT_1DOT:
            mBeatsPerBar = 1;
            break;
        case COUNT_2:
            mBeatsPerBar = 2;
            break;
        case COUNT_3:
            mBeatsPerBar = 3;
            break;
        case COUNT_4:
            mBeatsPerBar = 4;
            break;
        case COUNT_6:
            mBeatsPerBar = 6;
            break;
        case COUNT_8:
            mBeatsPerBar = 8;
            break;
        case COUNT_12:
            mBeatsPerBar = 12;
            break;
        case COUNT_16:
            mBeatsPerBar = 16;
            break;
        case COUNT_24:
            mBeatsPerBar = 24;
            break;
        case COUNT_32:
            mBeatsPerBar = 32;
            break;
        case COUNT_48:
            mBeatsPerBar = 48;
            break;
        case COUNT_96:
            mBeatsPerBar = 96;
            break;
    }
    mTickCount = pTickCount;
}

void Clock::setBarCallback(void (*pBarCallback)(unsigned long)) {
    p_barCallback = pBarCallback;
}

void Clock::setBeatCallback(void (*pBeatCallback)(unsigned long)) {
    p_beatCallback = pBeatCallback;
}

void Clock::update() {
    mTimeNow = micros();
    if (mTimeNow - mTimeLast >= mTickTime) {
        updateTick();
        mTimeLast = mTimeNow;
    }
}

void Clock::start() {
    reset();
    Midi.sendStart();
}

void Clock::reset() {
    mTicks = 0;
    mTimeNow = 0;
    mTimeLast = 0;
    mBarCount = 0;
    mBeatCount = 0;    
}

void Clock::updateTick() {
    mTicks++;

#ifdef C_DEBUG
    Serial.print("### (C) -- time: ");
    Serial.print(millis());
    Serial.print(" -- tick: ");
    Serial.println(mTicks);
#endif
    
    // forward midi clock signal to next pod
    Midi.sendClock();

    if (mTicks == mTickCount) {
        // C = A % B is equivalent to C = A – B * (A / B)
        if ((mBeatCount - mBeatsPerBar * (mBeatCount / mBeatsPerBar)) == 0) {        
        // if (mBeatCount % mBeatsPerBar == 0) {
            mBarCount++;
            doBarCallback();
        }
        
        // make beat count relative
        if(mBeatCount == mBeatsPerBar){
            mBeatCount = 0;
        }
        
        mBeatCount++;
        
        doBeatCallback();
        mTicks = 0;
    }
}

// ////////////////////////////////
// Private Methods
// ////////////////////////////////

void Clock::doBarCallback() {
    if (p_barCallback != NULL) {
        (*p_barCallback)(mBarCount - 1);
    }
}

void Clock::doBeatCallback() {
    if (p_beatCallback != NULL) {
        (*p_beatCallback)(mBeatCount - 1);
    }
}