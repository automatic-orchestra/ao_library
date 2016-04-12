/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Orchestra.h
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

#ifndef ORCHESTRA_H
#define ORCHESTRA_H

#include <Arduino.h>

// Forward declaration of classes due to cross-linking between
// this class and declared classes.
class Movement;
class Playlist;


/*
some methods nedd to be virtual so implementations of these functions
in classes which inherit from this class are called!
*/
class Orchestra {

public:
  Orchestra();
  ~Orchestra();
  // setup, start and update
  void setPlaylist(Playlist* pPlaylist);
  void start();
  void update();
  // configuration methods
  String getMacAddress();
  int getChannel();
  bool isKlockMeister();
  void printConfiguration();
  // led
  void turnOnLED();
  void turnOffLED();
  // MIDI message methods
  virtual void onNoteOn(byte channel, byte note, byte velocity);
  virtual void onNoteOff(byte channel, byte note, byte velocity);
  virtual void onControlChange(byte channel, byte control, byte value);
  virtual void onClockTick();
  virtual void onClockStart();
  // movement handling
  virtual void changeMovement(int pMovementID);

protected:
  Playlist* mPlaylist = nullptr;
  // configuration
  void setupDeviceParameters(String pMacAddress);
  String mMacAddress;
  int mMidiChannel;
  bool mKlockMeister;
  // led
  void setupLED();
  const static int LED_DURATION_LOOP_CYCLES = 100;
  int mLEDState = 0;
  // movement handling
  void sendChangeMovement(int pMovementID);
  Movement* getMovement(int pMovementID);
  Movement* mCurrentMovement = nullptr;
  int mChangeMovementFromControlMessage;

};


#endif //ORCHESTRA_H