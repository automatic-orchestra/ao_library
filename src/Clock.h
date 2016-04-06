/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Clock.h
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

#ifndef CLOCK_H
#define CLOCK_H

#include "ClockParams.h"
#include <Arduino.h>

class Clock {
public:
    Clock();
    Clock(unsigned int pBPM, clock_tick_count_t pClockTickCount);
    void setBeatsPerMinute(unsigned int pBPM);
    unsigned int getBeatsPerMinute(){
        return mBPM;
    }
    void setTickCount(clock_tick_count_t pTickCount);
    void setBarCallback(void (*pBarCallback)(unsigned long));
    void setBeatCallback(void (*pBeatCallback)(unsigned long));
    void update();
    void start();
    void reset();
    void updateTick();
private:
    void doBarCallback();
    void doBeatCallback();
    unsigned int mBPM = 0;
    clock_tick_count_t mTickCount;
    void (*p_barCallback)(unsigned long) = NULL;
    void (*p_beatCallback)(unsigned long) = NULL;
    unsigned long mTickTime = 0;
    unsigned long mTicks = 0;
    unsigned long mTimeNow = 0;
    unsigned long mTimeLast = 0;
    unsigned long mBeatsPerBar = 0;
    unsigned long mBeatCount = 0;
    unsigned long mBarCount = 0;
};

#endif //CLOCK_H