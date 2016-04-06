/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 CFOMidi.h
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

#include <Arduino.h>

#ifndef CFOMIDI_H
#define CFOMIDI_H

// MIDI specific constants
#define MIDI_SERIAL Serial1
#define MIDI_THROUGH true

#ifndef MIDI_CHANNEL
  #define MIDI_CHANNEL 1
#elif (MIDI_CHANNEL > 0)&&(MIDI_CHANNEL < 17)
#else
  #error MIDI_CHANNEL should be between 1 - 16
#endif

// Constants defined for MIDI CLOCK
#define MIDI_CLOCK 0xF8     // 248
#define MIDI_START 0xFA     // 250;
#define MIDI_CONTINUE 0xFB  // 251;
#define MIDI_STOP 0xFC      // 252;

//synth functions and parameters as MIDI controller numbers
#define PRESET_SAVE 0
#define PRESET_RECALL 1

#define IS_12_BIT 3
#define CUTOFF 4
#define ZERO_HZ_FM 5
#define FM_OCTAVES 6
#define RESONANCE_DECLARED_TWICE 7
#define PORTAMENTO 8
#define FILTER_TYPE 9

#define LFO1 10
#define SEMITONE1 11
#define DETUNE1 12
#define GAIN1 13
#define WAVEFORM1 14
#define FM1 15
#define FM1_OCTAVES 16
#define FM1_SOURCE 17
#define FM1_SHAPE 18
#define FREQUENCY1 19

#define LFO2 20
#define SEMITONE2 21
#define DETUNE2 22
#define GAIN2 23
#define WAVEFORM2 24
#define FM2 25
#define FM2_OCTAVES 26
#define FM2_SOURCE 27
#define FM2_SHAPE 28
#define FREQUENCY2 29

#define LFO3 30
#define SEMITONE3 31
#define DETUNE3 32
#define GAIN3 33
#define WAVEFORM3 34
#define FM3 35
#define FM3_OCTAVES 36
#define FM3_SOURCE 37
#define FM3_SHAPE 38
#define FREQUENCY3 39

#define SONG_PART 40
#define SONG_KEY 41     // essentially offset of MIDI note number
#define SONG_SCALE 42   // minor, major, etc

#define CUTOFF_MOD_AMOUNT 70
#define CUTOFF_MOD_DIRECTION 71
#define CUTOFF_SOURCE 72
#define CUTOFF_SHAPE 73
#define RESONANCE_MOD_AMOUNT 74
#define RESONANCE_MOD_DIRECTION 75
#define RESONANCE_SOURCE 76
#define RESONANCE_SHAPE 77
#define CUTOFF_FREQUENCY 78
#define RESONANCE 79

#define ENV0_VELOCITY 102
#define ENV0_ENABLE 103
#define ENV0_ATTACK 104
#define ENV0_DECAY 105
#define ENV0_SUSTAIN 106
#define ENV0_RELEASE 107

#define ENV1_VELOCITY 112
#define ENV1_ENABLE 113
#define ENV1_ATTACK 114
#define ENV1_DECAY 115
#define ENV1_SUSTAIN 116
#define ENV1_RELEASE 117

#define ENV2_VELOCITY 122
#define ENV2_ENABLE 123
#define ENV2_ATTACK 124
#define ENV2_DECAY 125
#define ENV2_SUSTAIN 126
#define ENV2_RELEASE 127

// MMidi class for handling MIDI implementation
class MMidi {

public:
  void init();
  void checkSerialMidi();
  void setChannel(uint8_t channel);
  uint8_t getChannel();
  uint8_t midiChannel;
  void midiHandler();
  void midiRealTimeHandler(uint8_t data);
  void noteOff(uint8_t channel, uint8_t note, uint8_t vel);
  void noteOn(uint8_t channel, uint8_t note, uint8_t vel);
  void aftertouch(uint8_t channel, uint8_t note, uint8_t pressure);
  void controller(uint8_t channel, uint8_t number, uint8_t value);
  void programChange(uint8_t channel, uint8_t number);
  void channelPressure(uint8_t channel, uint8_t pressure);
  void pitchWheel(uint8_t channel, uint8_t highBits, uint8_t lowBits);
  void pitchChange(uint8_t channel, int pitch); // extra pitchWheel function for USB MIDI interfacing

  void clock();
  void stop();
  void start();
  void continues();

  void sendNoteOff(uint8_t channel, uint8_t note);
  void sendNoteOff(uint8_t channel, uint8_t note, uint8_t vel);
  void sendNoteOn(uint8_t channel, uint8_t note, uint8_t vel);
  void sendController(uint8_t channel, uint8_t number, uint8_t value);

  void sendClock();
  void sendStart();
  void sendContinue();
  void sendStop();

  void setMidiIn(bool i);
  bool getMidiIn();
  void setMidiOut(bool o);
  bool getMidiOut();
  void setMidiThru(bool t);
  bool getMidiThru();
  void setMidiClockIn(bool i);
  bool getMidiClockIn();
  void setMidiClockOut(bool o);
  bool getMidiClockOut();
  void setMidiClockThru(bool t);
  bool getMidiClockThru();

  // callback function to interact with AO main sketch
  void setClockTickCallback( void (*pClockClickCallback)(void) );
  void setClockStartCallback( void (*pClockStartCallback)(void) );
  void setNoteOnCallback( void (*pNoteOnCallback)(uint8_t channel, uint8_t note, uint8_t vel) );
  void setNoteOffCallback( void (*pNoteOffCallback)(uint8_t channel, uint8_t note, uint8_t vel) );
  void setControllerCallback( void (*pControllerCallback)(uint8_t channel, uint8_t number, uint8_t value) );

private:
  void (*p_clockClickCallback)(void) = NULL;
  void (*p_clockStartCallback)(void) = NULL;
  void (*p_noteOnCallback)(uint8_t channel, uint8_t note, uint8_t vel) = NULL;
  void (*p_noteOffCallback)(uint8_t channel, uint8_t note, uint8_t vel) = NULL;
  void (*p_controllerCallback)(uint8_t channel, uint8_t number, uint8_t value) = NULL;

  // MIDI
  uint8_t data;
  uint8_t midiBuffer[3];
  bool midiIn;
  bool midiOut;
  bool midiThru;
  bool midiClockIn;
  bool midiClockOut;
  bool midiClockThru;
  int midiBufferIndex;
  uint16_t frequency;
  uint8_t notePlayed;
  bool midiRead;
};

extern MMidi Midi;

#endif //CFOMIDI_H
