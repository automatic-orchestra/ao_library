/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 CFOMidi.cpp
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

#include "CFOMidi.h"
#include "CFOSynthesizer.h"

const extern uint32_t portamentoTimeTable[] = {1,5,9,13,17,21,26,30,35,39,44,49,54,59,64,69,74,79,85,90,96,101,107,113,119,125,132,138,144,151,158,165,172,179,187,194,202,210,218,226,234,243,252,261,270,279,289,299,309,320,330,341,353,364,376,388,401,414,427,440,455,469,484,500,516,532,549,566,584,602,622,642,663,684,706,729,753,778,804,831,859,888,919,951,984,1019,1056,1094,1134,1176,1221,1268,1317,1370,1425,1484,1547,1614,1684,1760,1841,1929,2023,2125,2234,2354,2484,2627,2785,2959,3152,3368,3611,3886,4201,4563,4987,5487,6087,6821,7739,8918,10491,12693,15996,21500,32509,65535};

/////////////////////////////////////
//
//  MIDI specific functions
//
/////////////////////////////////////

void MMidi::init()
{
    pinMode(0, INPUT);
    pinMode(1, OUTPUT);
    Serial.begin(115200);
    MIDI_SERIAL.begin(115200);

    setMidiIn(true);
    setMidiOut(true);
    setMidiThru(true);
    setMidiClockIn(true);
    setMidiClockThru(true);
    setMidiClockOut(true);

    midiBufferIndex = 0;
    midiChannel = MIDI_CHANNEL - 1;
    midiRead = false;
//    Serial.println("MIDI intialised on channel 1. Use Midi.setChannel(channel) to set to other channel");
}

void MMidi::setClockTickCallback( void (*pClockClickCallback)(void) ){
  p_clockClickCallback = pClockClickCallback;
}

void MMidi::setClockStartCallback( void (*pClockStartCallback)(void) ){
  p_clockStartCallback = pClockStartCallback;
}

void MMidi::setNoteOnCallback( void (*pNoteOnCallback)(uint8_t channel, uint8_t note, uint8_t vel) ) {
  p_noteOnCallback = pNoteOnCallback;
}

void MMidi::setNoteOffCallback( void (*pNoteOffCallback)(uint8_t channel, uint8_t note, uint8_t vel) ) {
  p_noteOffCallback = pNoteOffCallback;
}

void MMidi::setControllerCallback( void (*pControllerCallback)(uint8_t channel, uint8_t number, uint8_t value) ) {
  p_controllerCallback = pControllerCallback;
}

void MMidi::setChannel(uint8_t channel)
{
  /*
    if(channel < 1 || channel > 16) {
        Serial.println("MIDI channel must be set to a number between 1 and 16");
    }
    else midiChannel = channel - 1;
    */

    // Refactoring starting MIDI channels from 0
    if(channel < 0 || channel > 15) {
        Serial.println("MIDI channel must be set to a number between 0 and 15");
    }
    else midiChannel = channel;
}

uint8_t MMidi::getChannel() {
    return midiChannel;
}


void MMidi::sendNoteOff(uint8_t channel, uint8_t note) {
    if(getMidiOut()) {
        MIDI_SERIAL.write(0x80 | (channel & 0x0F));
        MIDI_SERIAL.write(byte(note & 0x7F));
        MIDI_SERIAL.write(0x00);
    }
}


void MMidi::sendNoteOff(uint8_t channel, uint8_t note, uint8_t vel) {
    if(getMidiOut()) {
      // Serial.println("### (Midi) -> sendNoteOff");
        MIDI_SERIAL.write(0x80 | (channel & 0x0F));
        MIDI_SERIAL.write(byte(note & 0x7F));
        MIDI_SERIAL.write(byte(vel & 0x7F));
    }
}


void MMidi::sendNoteOn(uint8_t channel, uint8_t note, uint8_t vel) {
    if(getMidiOut()) {
      // Serial.println("### (Midi) -> sendNoteOn");
        MIDI_SERIAL.write(0x90 | (channel & 0x0F));
        MIDI_SERIAL.write(byte(note & 0x7F));
        MIDI_SERIAL.write(byte(vel & 0x7F));
    }
}


void MMidi::sendController(uint8_t channel, uint8_t number, uint8_t value) {
    if(getMidiOut()) {
        MIDI_SERIAL.write(0xB0 | (channel & 0x0F));
        MIDI_SERIAL.write(byte(number & 0x7F));
        MIDI_SERIAL.write(byte(value & 0x7F));
    }
}


void MMidi::sendClock() {
    if(getMidiClockOut()) {
        MIDI_SERIAL.write(MIDI_CLOCK);
    }
}


void MMidi::sendStart() {
    if(getMidiClockOut()) {
        MIDI_SERIAL.write(MIDI_START);
    }
}


void MMidi::sendContinue() {
    if(getMidiClockOut()) {
        MIDI_SERIAL.write(MIDI_CONTINUE);
    }
}


void MMidi::sendStop() {
    if(getMidiClockOut()) {
        MIDI_SERIAL.write(MIDI_STOP);
    }
}


void MMidi::setMidiIn(bool i)
{
    midiIn = i;
}


bool MMidi::getMidiIn()
{
    return midiIn;
}


void MMidi::setMidiOut(bool o)
{
    midiOut = o;
}


bool MMidi::getMidiOut()
{
    return midiOut;
}


void MMidi::setMidiThru(bool t)
{
    midiThru = t;
}


bool MMidi::getMidiThru()
{
    return midiThru;
}


void MMidi::setMidiClockIn(bool i)
{
    midiClockIn = i;
}


bool MMidi::getMidiClockIn()
{
    return midiClockIn;
}


void MMidi::setMidiClockOut(bool o)
{
    midiClockOut = o;
}


bool MMidi::getMidiClockOut()
{
    return midiClockOut;
}


void MMidi::setMidiClockThru(bool t)
{
    midiClockThru = t;
}


bool MMidi::getMidiClockThru()
{
    return midiClockThru;
}


void MMidi::checkSerialMidi()
{
  // link to MIDI messages table:
  // http://www.midi.org/techspecs/midimessages.php

    while( MIDI_SERIAL.available() ) {
    data = MIDI_SERIAL.read();

    // check whether it's a clocking message
        if(data >= 0xF8) {
            midiRealTimeHandler(data);
      //RealTimeSystem(byte(data));
            continue;
        }

//    if(data & 0x80 && (data & 0x0F) == midiChannel) { // bitmask with 10000000 to see if byte is over 127 (data&0x80)
//      midiBufferIndex = 0;              // and check if the midi channel corresponds to the midiChannel
//      midiRead = true;                // the device is set to listen to.
//    } else if(data & 0x80) {              // Else if the byte is over 127 (but not on the device's
//      midiRead = false;               // midiChannel, don't read this or any following bytes.
//    }

    // check whether the the first signal is one of the following messages:
    // 0x80 - 0x8F => Note off messages
    // 0x90 - 0x9F => Note on messages
    // 0xA0 - 0xAF => Polyphonic Aftertouch messages
    // 0xB0 - 0xBF => Control/Mode Change messages
    // 0xC0 - 0xCF => Program Change messages
    // 0xD0 - 0xDF => Channel Aftertouch messages
    // 0xE0 - 0xEF => Pitch Bend Change messages
    if(data >= 0x80 && data <= 0xEF) {       // check if incoming byte is a status byte (above 127)but less than sysEx (0xF0)
            // if((data & 0x0F) == midiChannel) {  // if it is, check if it is the right MIDI channel
                midiBufferIndex = 0;
                midiRead = true;
            // } else if(data >= 0x80) {           // if above check fails, check if it is still a status byte
            //     midiRead = false;
            // } else {}                           // if it is below 128 just continue
        }

        // If the previous condition is met buffer the next two bytes.
        if(midiRead) {
      midiBuffer[midiBufferIndex] = data;
      midiBufferIndex++;
      if (midiBufferIndex > 2) {
        midiRead = false;
        midiHandler();
//                Serial.println("MIDI RECEIVED");
//                Serial.println(midiBuffer[0], HEX);
//                Serial.println(midiBuffer[1], HEX);
//                Serial.println(midiBuffer[2], HEX);
      }
    }
  }
}


void MMidi::midiRealTimeHandler(uint8_t data) {
    //Serial.print("MMidi::midiRealTimeHandler() -- getMidiThru(): ");
  //Serial.print(getMidiThru());
  //Serial.print(" -- getMidiIn(): ");
  //Serial.println(getMidiIn());

    if(getMidiClockThru()) {
        MIDI_SERIAL.write(data);
    }
    if(getMidiClockIn()) {
        switch(data) {
            case 0xF8:
                clock();
                break;

            case 0xFA:
                start();
                break;

            case 0xFB:
                continues();
                break;

            case 0xFC:
                stop();
                break;

            default:
                break;
        }

    }
}


void MMidi::clock()
{
    // quick hack to circumvent sequencer.
    if(p_clockClickCallback != NULL){
            (*p_clockClickCallback)();
    }
//    Sequencer.clock();
}


void MMidi::start()
{
    // quick hack to circumvent sequencer.
    if(p_clockStartCallback != NULL){
            (*p_clockStartCallback)();
    }
//    Sequencer.start();
}


void MMidi::continues()
{
//    Sequencer.continues();
}


void MMidi::stop()
{
//    Sequencer.stop();
}


void MMidi::midiHandler() {
  // Serial.print("MMidi::midiHandler() -- getMidiThru(): ");
  // Serial.print(getMidiThru());
  // Serial.print(" -- getMidiIn(): ");
  // Serial.println(getMidiIn());

    if(getMidiThru()) {
        MIDI_SERIAL.write(midiBuffer[0]);
        MIDI_SERIAL.write(midiBuffer[1]);
        MIDI_SERIAL.write(midiBuffer[2]);
    }
    if(getMidiIn()) {
        // if((midiBuffer[0] & 0x0F) == midiChannel) {
            switch(midiBuffer[0] & 0xF0) { // bit mask with &0xF0
                case 0x80:
                    noteOff     (midiBuffer[0] & 0x0F,     // midi channel 0-15
                                     midiBuffer[1] & 0x7F,   // note value 0-127
                                     midiBuffer[2] & 0x7F);  // note velocity 0-127
                    break;

                case 0x90:
                    noteOn      (midiBuffer[0] & 0x0F,     // midi channel 0-15
                                     midiBuffer[1] & 0x7F,   // note value 0-127
                                     midiBuffer[2] & 0x7F);  // note velocity 0-127
                    break;

                case 0xA0:
                    aftertouch    (midiBuffer[0] & 0x0F,   // midi channel 0-15
                                     midiBuffer[1] & 0x7F, // note value 0-127
                                     midiBuffer[2] & 0x7F);// note velocity 0-127
                    break;

                case 0xB0:
                    if (p_controllerCallback != NULL) {
                        (*p_controllerCallback)(
                            midiBuffer[0] & 0x0F,
                            midiBuffer[1] & 0x7F,
                            midiBuffer[2] & 0x7F
                        );
                    }
                    controller(
                        midiBuffer[0] & 0x0F, // midi channel 0-15
                        midiBuffer[1] & 0x7F, // controller number 0-127
                        midiBuffer[2] & 0x7F  // controller value 0-127
                    );
                    break;

                case 0xC0:
                    programChange (midiBuffer[0]  & 0x0F,    // midi channel 0-15
                                     midiBuffer[1] & 0x7F);  // program number 0-127
                    break;

                case 0xD0:
                    channelPressure (midiBuffer[0]  & 0x0F,    // midi channel 0-15
                                     midiBuffer[1] & 0x7F);  // pressure amount 0-127
                    break;

                case 0xE0:
                    pitchWheel    (midiBuffer[0] & 0x0F,   // midi channel 0-15
                                     midiBuffer[1] & 0x7F, // higher bits 0-6
                                     midiBuffer[2] & 0x7F);// lower bits 7-13
                    break;

                default:
                    break;
            }
        // }
        // else Serial.println("Skipped MIDI message on other channel");
    }
}


void MMidi::noteOff(uint8_t channel, uint8_t note, uint8_t vel) {
    // Serial.print("### (Midi) NoteOff received on channel: ");
    // Serial.println(channel + 1, HEX);
  if( p_noteOffCallback != NULL ) {
    (*p_noteOffCallback)( channel, note, vel );
  } else {
    Music.noteOff(note);
  }
}


void MMidi::noteOn(uint8_t channel, uint8_t note, uint8_t vel) {
    // Serial.print("### (Midi) NoteOn received on channel: ");
    // Serial.println(channel + 1, HEX);
  if( p_noteOnCallback != NULL ) {
    (*p_noteOnCallback)( channel, note, vel );
  } else {
    Music.noteOn(note, vel);
  }
}


void MMidi::aftertouch(uint8_t channel, uint8_t note, uint8_t pressure) {
  // Write code here for Aftertouch
}


//TODO move this method into the synthesizer class where it belongs
void MMidi::controller(uint8_t channel, uint8_t number, uint8_t value) {

  if(value > 127) value = 127;

  // Since instrument exists in CFOSynthesizer.cpp a function to set
  // the instrument's value was introduced. To be evaluated.
  Music.setInstrument(number, value);
  // Before:
  //instrument[number] = value;

  switch(number) {
    case IS_12_BIT:
      if(value) Music.set12bit(true);
      else Music.set12bit(false);
      break;
    case PORTAMENTO:
      Music.setPortamento(portamentoTimeTable[value]);
      break;
    case CUTOFF:
      Music.setCutoff(value * 512);
      break;
    case RESONANCE:
      Music.setResonance(value * 512);
      break;
    case FILTER_TYPE:
      Music.setFilterType(value);
      break;
    case CUTOFF_MOD_AMOUNT:
      Music.setCutoffModAmount((value-64) * 1024);
      break;
    case CUTOFF_SOURCE:
      Music.setCutoffModSource(value);
      break;
    case CUTOFF_SHAPE:
      Music.setCutoffModShape(value);
      break;
    case ZERO_HZ_FM:
      if(value) Music.fmToZeroHertz(true);
      else Music.fmToZeroHertz(false);
      break;
    case FM_OCTAVES:
      Music.setFMoctaves(value+1);
      break;
    case LFO1:
      if(value) {
        Music.setOsc1LFO(true);
        Music.setFrequency1(Music.getNoteFrequency(value)/1024.0);
      } else {
        Music.setOsc1LFO(false);
      }
      break;
    case LFO2:
      if(value) {
        Music.setOsc2LFO(true);
        Music.setFrequency2(Music.getNoteFrequency(value)/1024.0);
      } else {
        Music.setOsc2LFO(false);
      }
      break;
    case LFO3:
      if(value) {
        Music.setOsc3LFO(true);
        Music.setFrequency3(Music.getNoteFrequency(value)/1024.0);
      } else {
        Music.setOsc3LFO(false);
      }
      break;
    case DETUNE1:
      Music.setDetune1(map(value,0,127,-100,100)*0.0005946);
      break;
    case DETUNE2:
      Music.setDetune2(map(value,0,127,-100,100)*0.0005946);
      //Music.setDetune2((value-64.0)*0.0005946);
      //Music.setDetune2(value/5120.0);
      break;
    case DETUNE3:
      Music.setDetune3(map(value,0,127,-100,100)*0.0005946);
      //Music.setDetune3((value-64.0)*0.0005946);
      //Music.setDetune3(value/5120.0);
      break;
    case SEMITONE1:
//      if(15 < value && value < 113) {
//        int8_t val = (((value-16)/2)-24);
//        Music.setSemitone1(val);
//      } else if (value < 16) {
//        Music.setSemitone1(-24);
//      } else {
//        Music.setSemitone1(24);
//      }
      if(40 <= value && value <= 88) {
        Music.setSemitone1(value-64);
      } else if (value < 40) {
        Music.setSemitone1(-24);
      } else {
        Music.setSemitone1(24);
      }
      break;
    case SEMITONE2:
//      if(15 < value && value < 113) {
//        int8_t val = (((value-16)/2)-24);
//        Music.setSemitone2(val);
//      } else if (value < 16) {
//        Music.setSemitone2(-24);
//      } else {
//        Music.setSemitone2(24);
//      }
      if(40 <= value && value <= 88) {
        Music.setSemitone2(value-64);
      } else if (value < 40) {
        Music.setSemitone2(-24);
      } else {
        Music.setSemitone2(24);
      }
      break;
    case SEMITONE3:
//      if(15 < value && value < 113) {
//        int8_t val = (((value-16)/2)-24);
//        Music.setSemitone3(val);
//      } else if (value < 16) {
//        Music.setSemitone3(-24);
//      } else {
//        Music.setSemitone3(24);
//      }
      if(40 <= value && value <= 88) {
        Music.setSemitone3(value-64);
      } else if (value < 40) {
        Music.setSemitone3(-24);
      } else {
        Music.setSemitone3(24);
      }
      break;
    case GAIN1:
      Music.setGain1(value / 127.0);
      break;
    case GAIN2:
      Music.setGain2(value / 127.0);
      break;
    case GAIN3:
      Music.setGain3(value / 127.0);
      break;
    case WAVEFORM1:
      Music.setWaveform1(value);
      break;
    case WAVEFORM2:
      Music.setWaveform2(value);
      break;
    case WAVEFORM3:
      Music.setWaveform3(value);
      break;
    case FM1:
      Music.setFM1(value);
      break;
    case FM2:
      Music.setFM2(value);
      break;
    case FM3:
      Music.setFM3(value);
      break;
//    case FM1_OCTAVES:
//      Music.setFM1octaves(value+1);
//      break;
//    case FM2_OCTAVES:
//      Music.setFM2octaves(value+1);
//      break;
//    case FM3_OCTAVES:
//      Music.setFM3octaves(value+1);
//      break;
    case FM1_SOURCE:
      Music.setFM1Source(value);
      break;
    case FM2_SOURCE:
      Music.setFM2Source(value);
      break;
    case FM3_SOURCE:
      Music.setFM3Source(value);
      break;
    case FM1_SHAPE:
      Music.setFM1Shape(value);
      break;
    case FM2_SHAPE:
      Music.setFM2Shape(value);
      break;
    case FM3_SHAPE:
      Music.setFM3Shape(value);
      break;
    case ENV1_ENABLE:
      if(value) Music.enableEnvelope1();
      else Music.disableEnvelope1();
      break;
    case ENV1_ATTACK:
      Music.setEnv1Attack(value);
      break;
    case ENV1_DECAY:
      Music.setEnv1Decay(value);
      break;
    case ENV1_SUSTAIN:
      Music.setEnv1Sustain(value);
      break;
    case ENV1_RELEASE:
      Music.setEnv1Release(value);
      break;
    case ENV2_ENABLE:
      if(value) Music.enableEnvelope2();
      else Music.disableEnvelope2();
      break;
    case ENV2_ATTACK:
      Music.setEnv2Attack(value);
      break;
    case ENV2_DECAY:
      Music.setEnv2Decay(value);
      break;
    case ENV2_SUSTAIN:
      Music.setEnv2Sustain(value);
      break;
    case ENV2_RELEASE:
      Music.setEnv2Release(value);
      break;
    case PRESET_SAVE:
      Music.savePreset(value);
      break;
    case PRESET_RECALL:
      Music.getPreset(value);
      Music.sendInstrument();
      break;
    default:
      break;
  }
}


void MMidi::programChange(uint8_t channel, uint8_t number) {
//  Music.getPreset(number);
}


void MMidi::channelPressure(uint8_t channel, uint8_t pressure) {
  // Write code here for Channel Pressure
}


void MMidi::pitchWheel(uint8_t channel, uint8_t highBits, uint8_t lowBits) {
  // Write code here for Pitch Wheel
}
