/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 CFOSynthesizer.h
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

#ifndef CFOSYNTHESIZER_H
#define CFOSYNTHESIZER_H

#include <Arduino.h>
#include <EEPROM.h>

// Useful bit constants
#define BIT_8 256
#define BIT_12 4096
#define BIT_16 65536
#define BIT_20 1048576
#define BIT_24 16777216
#define BIT_28 268435456
#define BIT_32 4294967296
#define MAX_SAMPLE 32767
#define MIN_SAMPLE -32768

// set order of filters. MAX/MSP interface maps to 0, 1, 2, 3
#define LP6 0
#define HP6 1
#define BP6 2
#define LP24 6
#define HP24 4
#define BP24 5
#define MOOG 3
#define THRU 7

#define SAMPLE_RATE 48000
#define CPU_FREQ 96 // in MHz
#define PERIOD_MAX BIT_32

// Specify highest and lowest pitch in Hz
#define LOW_PITCH 55
#define HIGH_PITCH 1000

// Shortnames for waveforms
#define SINE 0
#define	SQUARE 1
#define PULSE 2
#define TRIANGLE 3
#define SAW 4
#define FUZZ 5
#define DIGI1 6
#define DIGI2 7
#define DIGI3 8
#define DIGI4 9
#define NOISE 10
#define DIGI6 11
#define TAN1 12
#define TAN2 13
#define TAN3 14
#define TAN4 15

// #define WAVEFORM_TRIANGLE 0
// #define WAVEFORM_SAW 1
// #define WAVEFORM_SQUARE 2
// #define WAVEFORM_ALTERNATE 3

// Maximum possible value for amplification envelope in audio code
#define MAX_ENV_GAIN 65535
#define MIN_ENV_GAIN -65535

// parameters for modulation
#define MOD_FULL 0
#define MOD_ENV1 1
#define MOD_ENV2 2
//#define MOD_ENV0 9
#define MOD_OSC1 3
#define MOD_OSC2 4
#define MOD_OSC3 5

// parameters for presets || the two parameters below should multiply to 2048.
#define MAX_PRESETS 16
#define PRESET_SIZE 128

// preset bank offsets
#define BANK_U 0
#define BANK_A 16
#define BANK_B 32
#define BANK_C 48


// MMusic class for handling sound engine
class MMusic {
public:
	// INITIALIZER
  void init();
	void spi_setup();
	void set12bit(bool b);
	bool is12bit;

	// PRESETS
	void getPreset(uint8_t p);
	void savePreset(uint8_t p);
	void sendInstrument();
	void loadAllPresets();

	// AUDIO INTERRUPT SERVICE ROUTINE
	void synthInterrupt8bitFM();
	void synthInterrupt12bitSineFM();

	void envelope1();
	void envelope2();
	void amplifier();

  void output2T3DAC();    // sending sample to Teensy3.1 DAC on pin 14

	// FILTER FUNCTIONS
	void filterLP6dB();
	void filterHP6dB();
  void filterLP24dB();
  void filterHP24dB();
  void filterMoogLadder();
	void setCutoff(uint16_t c);
	void setResonance(uint32_t res);
  void setFilterType(uint8_t type);
	void setCutoffModAmount(int32_t amount);
	void setCutoffModDirection(int32_t direction);
	void setCutoffModSource(uint8_t source);
	void setResonanceModSource(uint8_t source);
	void setCutoffModShape(uint8_t shape);
	void setResonanceModShape(uint8_t shape);

  bool lowpass;
  bool highpass;
  bool lowpass24dB;
  bool highpass24dB;
  bool moogLadder;

	// FREQUENCY AND DETUNE FUNCTIONS
	void setFrequency(float frequency);
	void setFrequency1(float frequency1);
	void setFrequency2(float frequency2);
	void setFrequency3(float frequency3);
	void setSemitone1(int8_t semi);
	void setSemitone2(int8_t semi);
	void setSemitone3(int8_t semi);
	void setDetune(float detune);
	void setDetune1(float detune);
	void setDetune2(float detune);
	void setDetune3(float detune);
	void setOsc1LFO(bool lfo);
	void setOsc2LFO(bool lfo);
	void setOsc3LFO(bool lfo);
	void setFM1(uint8_t fm);
	void setFM2(uint8_t fm);
	void setFM3(uint8_t fm);
	void setFMoctaves(uint8_t octs);	// THIS SHOULD PROBABLY BE CALLED SOMETHING ELSE
	void setFM1octaves(uint8_t octs);
	void setFM2octaves(uint8_t octs);
	void setFM3octaves(uint8_t octs);
	void setFM1Source(uint8_t source);
	void setFM2Source(uint8_t source);
	void setFM3Source(uint8_t source);
	void setFM1Shape(uint8_t shape);
	void setFM2Shape(uint8_t shape);
	void setFM3Shape(uint8_t shape);
	void fmToZeroHertz(bool);			// THIS SHOULD PROBABLY BE CALLED SOMETHING ELSE
	void pitchBend(float b); // NOT IMPLEMENTED
	void setPortamento(int32_t port);

	// WAVEFORM FUNCTIONS
	void setWaveform(uint16_t waveForm);    // JUST FOR 8bit WAVEFORMS
	void setWaveform1(uint16_t waveForm);   //
	void setWaveform2(uint16_t waveForm);   //
	void setWaveform3(uint16_t waveForm);   //

	// GAIN FUNCTIONS
	void setGain(float value); // 0.0 - 1.0
	void setGain1(float value); // 0.0 - 1.0
	void setGain2(float value); // 0.0 - 1.0
	void setGain3(float value); // 0.0 - 1.0
	float getGain();       // 0.0 - 1.0
	float getGain1();       // 0.0 - 1.0
	float getGain2();       // 0.0 - 1.0
	float getGain3();       // 0.0 - 1.0

	// NOTE FUNCTIONS
	void noteOn(uint8_t note, uint8_t vel); // 0 - 127
	void noteOn(uint8_t note); // 0 - 127
	void noteOff(uint8_t note); // 0 - 127
	void noteOff();
	float getNoteFrequency(uint8_t note); // 0 - 127

	// ENVELOPE FUNCTIONS
	void enableEnvelope1();
	void disableEnvelope1();
	void setEnv1Stage(uint8_t stage1); // 0 - 4
	void setEnv1Attack(uint8_t att); // 0 - 127
	void setEnv1Decay(uint8_t dec); // 0 - 127
	void setEnv1Sustain(uint8_t sus); // 0 - 127
	void setEnv1Release(uint8_t rel); // 0 - 127
	void setEnv1VelSustain(uint8_t vel); // 0 - 127
	void setEnv1VelPeak(uint8_t vel); // 0 - 127

	void enableEnvelope2();
	void disableEnvelope2();
	void setEnv2Stage(uint8_t stage); // 0 - 4
	void setEnv2Attack(uint8_t att); // 0 - 127
	void setEnv2Decay(uint8_t dec); // 0 - 127
	void setEnv2Sustain(uint8_t sus); // 0 - 127
	void setEnv2Release(uint8_t rel); // 0 - 127
	void setEnv2VelSustain(uint8_t vel); // 0 - 127
	void setEnv2VelPeak(uint8_t vel); // 0 - 127

	bool osc1LFO;
	bool osc2LFO;
	bool osc3LFO;

  int32_t oscil1;
	int32_t oscil2;
	int32_t oscil3;

  // TODO: Evaluate if this function is really needed.
  // It only exists, because MMidi::controller is calling it.
  void setInstrument(uint8_t number, uint8_t value);

private:
	// TIMER VARIABLES
	uint32_t sampleRate;

	// WAVEFORM VARIABLES
	uint16_t waveForm1;
	uint16_t waveForm2;
	uint16_t waveForm3;
	uint16_t waveForm;
  uint16_t waveform;

	// FREQUENCY VARIABLES
	uint16_t frequency16bit;
	float frequency;
	float frequency1;
	float frequency2;
	float frequency3;
	float semi1;
	float semi2;
	float semi3;
	float detune1;
	float detune2;
	float detune3;
	float bend;

	// OSCILLATOR VARIABLES
	int32_t period1;
	int32_t period2;
	int32_t period3;
	int32_t portamento;
	volatile int32_t dPhase1;
	volatile int32_t dPhase2;
	volatile int32_t dPhase3;
	uint32_t accumulator1;
	uint32_t accumulator2;
	uint32_t accumulator3;
	int32_t index1;
	int32_t index2;
	int32_t index3;
	int64_t modulator1;
	int64_t modulator2;
	int64_t modulator3;
	int32_t fullSignal;
	int32_t invertSignal;
	int32_t noSignal;
	int32_t *osc1modSource_ptr;
	int32_t *osc2modSource_ptr;
	int32_t *osc3modSource_ptr;
	int32_t *amp_modSource_ptr;
	int32_t *osc1modShape_ptr;
	int32_t *osc2modShape_ptr;
	int32_t *osc3modShape_ptr;
	int32_t *amp_modShape_ptr;
	int32_t zeroFM;
	int32_t fmAmount1;
	int32_t fmAmount2;
	int32_t fmAmount3;
	int32_t fmOctaves1;
	int32_t fmOctaves2;
	int32_t fmOctaves3;


	int32_t gain;
	int32_t gain1;
	int32_t gain2;
	int32_t gain3;

	// FILTER VARIABLES
  int64_t a0;
  int64_t a1;
  int64_t a2;
  int64_t a3;
  int64_t a4;

  int64_t b0;
  int64_t b1;
  int64_t b2;
  int64_t b3;
  int64_t b4;

  int64_t x0;
  int64_t x1;
  int64_t x2;
  int64_t x3;
  int64_t x4;

  int64_t y0;
  int64_t y1;
  int64_t y2;
  int64_t y3;
  int64_t y4;

  int64_t xNew;
  int64_t xOld;
  int64_t yNew;
  int64_t yOld;
  int64_t feedbackSample;

  volatile int64_t u;
  int64_t g;
  int64_t gg;
  int64_t ggg;
  int64_t G;
  int64_t Gstage;
  volatile int64_t S;

  volatile int64_t k;
  int64_t v1;
  int64_t v2;
  int64_t v3;
  int64_t v4;
  int64_t z1;
  int64_t z2;
  int64_t z3;
  int64_t z4;

  uint16_t cutoff;
	uint32_t resonance;

	int32_t cutoffModAmount;
	int32_t cutoffModDirection;
	int32_t *cutoffModSource_ptr;
	int32_t *resonanceModSource_ptr;
	int32_t *cutoffModShape_ptr;
	int32_t *resonanceModShape_ptr;

  int64_t lastSampleOutLP;
  int64_t lastSampleInLP;
  int64_t sampleOutLP;
  int64_t sampleInLP;
  int64_t lastSampleOutHP;
  int64_t lastSampleInHP;
  int64_t sampleOutHP;
  int64_t sampleInHP;

	// ENVELOPE VARIABLES
	bool envelopeOn1;
	int32_t env1;
	int32_t env1Stage;
	int32_t attack1;
	int32_t decay1;
	int32_t sustain1;
	int32_t release1;
	int32_t velSustain1;
	int32_t velPeak1;
    int32_t envTarget;

	bool envelopeOn2;
	int32_t env2;
	int32_t env2Stage;
	int32_t attack2;
	int32_t decay2;
	int32_t sustain2;
	int32_t release2;
	int32_t velSustain2;
	int32_t velPeak2;

	// NOTE VARIABLE
	uint8_t notePlayed;

	// final sample that goes to the DAC
	volatile int64_t sample;

	// the two bytes that go to the DAC over SPI for VCF and VCA
	volatile uint8_t dacSPIA0;
	volatile uint8_t dacSPIA1;
	volatile uint8_t dacSPIB0;
	volatile uint8_t dacSPIB1;
	volatile uint8_t dacSetA;
	volatile uint8_t dacSetB;
};

extern MMusic Music;

#endif //CFOSYNTHESIZER_H
