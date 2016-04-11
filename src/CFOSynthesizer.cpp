/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 CFOSynthesizer.cpp
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

#include "CFOSynthesizer.h"
#include "CFOMidi.h"

IntervalTimer synthTimer;
unsigned long timerCounter1 = 0;
unsigned long timerCounter2 = 0;

MMusic Music;
MMidi Midi;

uint8_t instrument[128];
uint8_t userPresets[MAX_PRESETS][PRESET_SIZE];

int64_t filterSamplesLP24dB[4];
int64_t filterSamplesHP24dB[8];
int64_t filterSamplesMoogLadder[4];

const uint8_t programPresets[] = {
#include <inc/Presets.h>
};

const uint16_t sineTable[] = {
#include <inc/SineTable.inc>
};

const uint16_t waveTable[] = {
#include <inc/WaveTable.inc>
};

// Table of MIDI note values to frequency in Hertz
const float hertzTable[] = {
#include <inc/HertzTable.inc>
};

const int64_t filterCoefficient[] = {
#include <inc/filterCoefficients_1poleLP.inc>
};

const int64_t filterCoefficientsMoogLadder[] = {
#include <inc/filterCoefficientsMoogLadder.inc>
};

// Used in the functions that set the envelope timing
const uint32_t envTimeTable[] = {1,5,9,14,19,26,34,42,53,65,79,95,113,134,157,182,211,243,278,317,359,405,456,511,570,633,702,776,854,939,1029,1124,1226,1333,1448,1568,1695,1829,1971,2119,2274,2438,2610,2789,2977,3172,3377,3590,3813,4044,4285,4535,4795,5065,5345,5635,5936,6247,6569,6902,7247,7602,7970,8349,8740,9143,9559,9986,10427,10880,11347,11827,12321,12828,13349,13883,14433,14996,15574,16167,16775,17398,18036,18690,19359,20045,20746,21464,22198,22949,23716,24501,25303,26122,26959,27813,28686,29577,30486,31413,32359,33325,34309,35312,36335,37378,38440,39522,40625,41748,42892,44056,45241,46448,47675,48925,50196,51489,52803,54141,55500,56883,58288,59716,61167,62642,64140,65662};

const float semitoneTable[] = {0.25,0.2648658,0.2806155,0.29730177,0.31498027,0.33370996,0.35355338,0.37457678,0.39685026,0.4204482,0.44544938,0.47193715,0.5,0.5297315,0.561231,0.59460354,0.62996054,0.6674199,0.70710677,0.74915355,0.7937005,0.8408964,0.8908987,0.9438743,1.0,1.0594631,1.122462,1.1892071,1.2599211,1.3348398,1.4142135,1.4983071,1.587401,1.6817929,1.7817974,1.8877486,2.0,2.1189263,2.244924,2.3784142,2.5198421,2.6696796,2.828427,2.9966142,3.174802,3.3635857,3.563595,3.7754972,4.0};




//////////////////////////////////////////////////////////
//
// SYNTH INTERRUPT
//
//////////////////////////////////////////////////////////

void synth_isr(void) {
  // First we output the sample calculated last time
  Music.output2T3DAC();

  // Update Envelopes
	Music.envelope1();
	Music.envelope2();

  // Update Oscillators
  if(Music.is12bit) Music.synthInterrupt12bitSineFM();
	else Music.synthInterrupt8bitFM();

  // Run the output sample through the amplifying function (VCA style)
	Music.amplifier();

  // And then through the filter (VCF style)
	if(Music.lowpass) Music.filterLP6dB();
	if(Music.highpass) Music.filterHP6dB();
  if(Music.lowpass24dB) Music.filterLP24dB();
  if(Music.highpass24dB) Music.filterHP24dB();
  if(Music.moogLadder) Music.filterMoogLadder();
}




/////////////////////////////////////////////////////////
//
//	8 BIT OSCILLATOR - WAVETABLE - 16 WAVEFORMS
//
/////////////////////////////////////////////////////////



void MMusic::synthInterrupt8bitFM ()
{

	dPhase1 = dPhase1 + (period1 - dPhase1) / portamento;
	modulator1 = (fmAmount1 * fmOctaves1 * (*osc1modSource_ptr))>>10;
	modulator1 = (modulator1 * (*osc1modShape_ptr))>>16;
	modulator1 = (modulator1 * int64_t(dPhase1))>>16;
	modulator1 = (modulator1>>((modulator1>>31)&zeroFM));
	accumulator1 = accumulator1 + dPhase1 + modulator1;
	index1 = accumulator1 >> 24;
	oscil1 = waveTable[index1 + waveForm1];
	oscil1 -= 128;
	oscil1 <<= 8;
	sample = (oscil1 * gain1);

	dPhase2 = dPhase2 + (period2 - dPhase2) / portamento;
	modulator2 = (fmAmount2 * fmOctaves2 * (*osc2modSource_ptr))>>10;
	modulator2 = (modulator2 * (*osc2modShape_ptr))>>16;
	modulator2 = (modulator2 * int64_t(dPhase2))>>16;
	modulator2 = (modulator2>>((modulator2>>31)&zeroFM));
	accumulator2 = accumulator2 + dPhase2 + modulator2;
	index2 = accumulator2 >> 24;
	oscil2 = waveTable[index2 + waveForm2];
	oscil2 -= 128;
	oscil2 <<= 8;
	sample += (oscil2 * gain2);

	dPhase3 = dPhase3 + (period3 - dPhase3) / portamento;
	modulator3 = (fmAmount3 * fmOctaves3 * (*osc3modSource_ptr))>>10;
	modulator3 = (modulator3 * (*osc3modShape_ptr))>>16;
	modulator3 = (modulator3 * int64_t(dPhase3))>>16;
	modulator3 = (modulator3>>((modulator3>>31)&zeroFM));
	accumulator3 = accumulator3 + dPhase3 + modulator3;
	index3 = accumulator3 >> 24;
	oscil3 = waveTable[index3 + waveForm3];
	oscil3 -= 128;
	oscil3 <<= 8;
	sample += (oscil3 * gain3);

	sample >>= 18;

}





/////////////////////////////////////////////////////////
//
//	12 BIT OSCILLATOR - SINETABLE
//
/////////////////////////////////////////////////////////


void MMusic::synthInterrupt12bitSineFM()
{

	dPhase1 = dPhase1 + (period1 - dPhase1) / portamento;
	modulator1 = (fmAmount1 * fmOctaves1 * (*osc1modSource_ptr))>>10;
	modulator1 = (modulator1 * (*osc1modShape_ptr))>>16;
	modulator1 = (modulator1 * int64_t(dPhase1))>>16;
	modulator1 = (modulator1>>((modulator1>>31)&zeroFM));
	accumulator1 = accumulator1 + dPhase1 + modulator1;
	index1 = accumulator1 >> 20;
	oscil1 = sineTable[index1];
	index1 = accumulator1 >> 20;
	oscil1 -= 32768;
	sample = (oscil1 * gain1);

	dPhase2 = dPhase2 + (period2 - dPhase2) / portamento;
	modulator2 = (fmAmount2 * fmOctaves2 * (*osc2modSource_ptr))>>10;
	modulator2 = (modulator2 * (*osc2modShape_ptr))>>16;
	modulator2 = (modulator2 * int64_t(dPhase2))>>16;
	modulator2 = (modulator2>>((modulator2>>31)&zeroFM));
	accumulator2 = accumulator2 + dPhase2+ modulator2;
	index2 = accumulator2 >> 20;
	oscil2 = sineTable[index2];
	oscil2 -= 32768;
	sample += (oscil2 * gain2);

	dPhase3 = dPhase3 + (period3 - dPhase3) / portamento;
	modulator3 = (fmAmount3 * fmOctaves3 * (*osc3modSource_ptr))>>10;
	modulator3 = (modulator3 * (*osc3modShape_ptr))>>16;
	modulator3 = (modulator3 * int64_t(dPhase3))>>16;
	modulator3 = (modulator3>>((modulator3>>31)&zeroFM));
	accumulator3 = accumulator3 + dPhase3 + modulator3;
	index3 = accumulator3 >> 20;
	oscil3 = sineTable[index3];
	oscil3 -= 32768;
	sample += (oscil3 * gain3);

	sample >>= 18;

}


void MMusic::set12bit(bool b) {
	is12bit = b;
}




/////////////////////////////////////////////////////////
//
//	ENVELOPES
//
/////////////////////////////////////////////////////////


void MMusic::envelope1() {

	if(envelopeOn1) {

		// Attack
		if(env1Stage == 1) {
			env1 += 1; // to make sure the envelope increases when (MAX_ENV_GAIN-env1) is smaller than attack1
			env1 += (MAX_ENV_GAIN - env1)/attack1;
			if(velPeak1 < env1) {
				env1 = velPeak1;
				env1Stage = 2;
			}
		}
		// Decay
		else if(env1Stage == 2) {
			env1 += -1;	// to make sure the envelope decreases when (velSustain1-env1) is smaller than decay1
			env1 += (velSustain1-env1)/decay1;
			if(env1 < velSustain1 || MAX_ENV_GAIN < env1) {
				env1 = velSustain1;
				env1Stage = 3;
			}
		}
		// Sustain
		else if (env1Stage == 3) {
			env1 = velSustain1;
		}

		// Release
		else if (env1Stage == 4) {
			env1 += -1; // to make sure the envelope decreases when (0-env1) is smaller than release1
			env1 += (0 - env1) / release1;
			if(env1 < 0 || MAX_ENV_GAIN < env1) {
				env1 = 0;
				env1Stage = 0;
			}
		}

		// No gain
		else if (env1Stage == 0) {
			env1 = 0;
		}

	} else {
		env1 = 65535;
	}

}


void MMusic::envelope2() {

	if(envelopeOn2) {

		// Attack
		if(env2Stage == 1) {
			env2 += 1; // to make sure the envelope increases when (MAX_ENV_GAIN-env2) is smaller than attack1
			env2 += (MAX_ENV_GAIN-env2)/attack2;
			if(velPeak2 < env2) {
				env2 = velPeak2;
				env2Stage = 2;
			}
		}
		// Decay
		else if(env2Stage == 2) {
			env2 += -1;	// to make sure the envelope decreases when (velSustain2-env2) is smaller than decay2
			env2 += (velSustain2-env2)/decay2;
			if(env2 < velSustain2 || MAX_ENV_GAIN < env2) {
				env2 = velSustain2;
				env2Stage = 3;
			}
		}
		// Sustain
		else if (env2Stage == 3) {
			env2 = velSustain2;
		}

		// Release
		else if (env2Stage == 4) {
			env2 += -1; // to make sure the envelope decreases when (0-env2) is smaller than release2
			env2 += (0 - env2) / release2;
			if(env2 < 0 || MAX_ENV_GAIN < env2) {
				env2 = 0;
				env2Stage = 0;
			}
		}

		// No gain
		else if (env2Stage == 0) {
			env2 = 0;
		}

	} else {
		env2 = 65535;
	}

}


void MMusic::amplifier() {

	sample = (env1 * sample) >> 16;

}


/////////////////////////////////////////////////////////
//
//	SEND SAMPLE TO DAC ON TEENSY 3.1 PIN A14
//
/////////////////////////////////////////////////////////


void MMusic::output2T3DAC() {
	sample += 32768;
    analogWrite(A14, sample>>4);
}




/////////////////////////////////////
//
//	INITIALIZING FUNCTION
//
/////////////////////////////////////


void MMusic::getPreset(uint8_t p)
{

    const static int AO_CONTROL_MESSAGES = 40;
    const static int AO_CONTROL_MESSAGES_MAX = 69;

	if(p < MAX_PRESETS) {
//		Serial.print("GETTING PRESET NUMBER : ");
//		Serial.println(p);
		for(uint8_t i=2; i<128; i++) {
            if (i >= AO_CONTROL_MESSAGES && i < AO_CONTROL_MESSAGES_MAX) continue;
			instrument[i] = userPresets[p][i];
			Midi.controller(Midi.midiChannel, i, instrument[i]);
//			Serial.println(userPresets[p][i]);
		}
	} else {
		for(uint8_t i=2; i<128; i++) {
            if (i >= AO_CONTROL_MESSAGES && i < AO_CONTROL_MESSAGES_MAX) continue;
			instrument[i] = programPresets[(p-MAX_PRESETS)*PRESET_SIZE + i];
			Midi.controller(Midi.midiChannel, i, instrument[i]);
//			Serial.println(userPresets[p][i]);
		}

	}
}


#if defined(USB_MIDI)

void MMusic::sendInstrument()
{
	Serial.print("SENDING CURRENT INSTRUMENT ");
	Serial.print(" OVER MIDI on channel ");
    Serial.println(Midi.midiChannel+1);
	cli();
	for(uint8_t i=2; i<128; i++) {
		usbMIDI.sendControlChange(i, instrument[i], Midi.midiChannel+1);
	}
	sei();
}

#else

void MMusic::sendInstrument(){;}

#endif


void MMusic::savePreset(uint8_t p)
{
	if(p < MAX_PRESETS) {
		Serial.print("SAVING PRESET NUMBER : ");
		Serial.println(p);
		for(uint8_t i=0; i<128; i++) {
			//Serial.print(i);
			//Serial.print(" : ");
			userPresets[p][i] = instrument[i];
			//Serial.println(userPresets[p][i]);
			//insert code for saving instrument sequence here
			cli();
			EEPROM.write(p * PRESET_SIZE + i, instrument[i]);
			sei();
		}
	}
	else {
		Serial.println("CAN NOT SAVE PRESET TO EEPROM - COPY/PASTE BELOW TO FILE");
		for(uint8_t i=0; i<128; i++) {
			Serial.print(instrument[i]);
			Serial.print(", ");
		}
	}

}


void MMusic::loadAllPresets()
{
	for(uint8_t i=2; i<128; i++) {
		for(uint8_t p=0; p<MAX_PRESETS;p++) {
			userPresets[p][i] = EEPROM.read(p * PRESET_SIZE + i);
		}
	}
}


void MMusic::init()
{
	for(uint8_t i=0; i<128; i++) {
		instrument[i] = 0;
		for(uint8_t p=0; p<MAX_PRESETS;p++) {
			userPresets[p][i] = 0;
		}
	}

	sampleRate = SAMPLE_RATE;
	sample = 0;
	set12bit(false);
	setPortamento(0);

	dPhase1 = 0;
	dPhase2 = 0;
	dPhase3 = 0;
	modulator1 = 0;
	modulator2 = 0;
	modulator3 = 0;
	fullSignal = 65535;
	invertSignal = -65535;
	noSignal = 0;

	osc1modSource_ptr = &oscil3;
	osc2modSource_ptr = &oscil1;
	osc3modSource_ptr = &oscil2;
	osc1modShape_ptr = &fullSignal;
	osc2modShape_ptr = &fullSignal;
	osc3modShape_ptr = &fullSignal;

	amp_modSource_ptr = &env1;
	amp_modShape_ptr = &fullSignal;

	setFM1Source(3);
	setFM2Source(1);
	setFM3Source(2);
	setFM1Shape(0);
	setFM2Shape(0);
	setFM3Shape(0);

	fmToZeroHertz(true);
	accumulator1 = 0;
	accumulator2 = 0;
	accumulator3 = 0;
	index1 = 0;
	index2 = 0;
	index3 = 0;
	oscil1 = 0;
	oscil2 = 0;
	oscil3 = 0;

	// waveform setup
	setWaveform(0);

	// frequency setup
	setFrequency(440);
	setSemitone1(0);
	setSemitone2(0);
	setSemitone3(0);
	setDetune(0);
	setOsc1LFO(false);
	setOsc2LFO(false);
	setOsc3LFO(false);

	// gain setup
	setGain(1.0f);
	setGain1(1.0f);
	setGain2(1.0f);
	setGain3(1.0f);

	// envelope setup
	setEnv1Stage(0);
	disableEnvelope1();
	env1 = 0;

	setEnv1Attack(4);
	setEnv1Decay(90);
	setEnv1Sustain(32);
	setEnv1Release(64);
	setEnv1VelSustain(0);

	setEnv2Stage(0);
	disableEnvelope2();
	env2 = 0;

	setEnv2Attack(8);
	setEnv2Decay(36);
	setEnv2Sustain(0);
	setEnv2Release(64);
	setEnv2VelSustain(0);

	//FM setup
	setFM1(0);
	setFM2(0);
	setFM3(0);
	setFMoctaves(0);

	// DAC setup
  analogWriteResolution(12);

	loadAllPresets();

	// filter setup
  setCutoff((uint16_t)BIT_16);
	setResonance(BIT_16);
  setFilterType(0);

	cutoffModSource_ptr = &env2;
	resonanceModSource_ptr = &fullSignal;
	cutoffModShape_ptr = &fullSignal;
	resonanceModShape_ptr = &fullSignal;

	setCutoffModSource(2);
	setCutoffModAmount(BIT_16);
	setCutoffModDirection(1);

	cli();
	synthTimer.begin(synth_isr, 1000000.0 / sampleRate);
	sei();

}




/////////////////////////////////////
//
//	FILTER FUNCTIONS
//
/////////////////////////////////////

void MMusic::setCutoff(uint16_t c)
{
    cutoff = c;
}


void MMusic::setResonance(uint32_t res)
{
	resonance = res;
  k = res;
}


void MMusic::setCutoffModAmount(int32_t amount) {
	if(amount >= 65536) cutoffModAmount = 65535;
	else if(amount < -65536) cutoffModAmount = -65536;
	else cutoffModAmount = amount;
}

void MMusic::setCutoffModDirection(int32_t direction) {
	if(direction >= 0) cutoffModDirection = 1;
	else cutoffModDirection = -1;
}


void MMusic::filterLP6dB() {

	int64_t mod = (int64_t(cutoffModAmount) * (int64_t(*cutoffModSource_ptr)))>>16;
	int64_t c = (mod + int64_t(cutoff));
	if(c > 65535) c = 65535;
	else if(c < 0) c = 0;
//	c = ((((c * 32768) >> 15) + 65536) >> 1);

    b1 = filterCoefficient[c>>8];
    a0 = BIT_32 - b1;

    sample = (a0 * sample + b1 * lastSampleOutLP) >> 32;
    lastSampleOutLP = sample;

}


void MMusic::filterHP6dB() {

    sampleInHP = sample;

	int64_t mod = (int64_t(cutoffModAmount) * (int64_t(*cutoffModSource_ptr)))>>16;
	int64_t c = (mod + int64_t(cutoff));
	if(c > 65535) c = 65535;
	else if(c < 0) c = 0;
    //	c = ((((c * 32768) >> 15) + 65536) >> 1);

    b1 = filterCoefficient[c>>8];
    a0 = (BIT_32 + b1) >> 1;
    a1 = -a0;

    sampleOutHP = (a0 * sampleInHP + a1 * lastSampleInHP + b1 * lastSampleOutHP) >> 32;

    lastSampleInHP = sampleInHP;
    lastSampleOutHP = sampleOutHP;
    sample = sampleOutHP;

}


void MMusic::filterLP24dB() {  // BROKEN?????

	int64_t mod = (int64_t(cutoffModAmount) * (int64_t(*cutoffModSource_ptr)))>>16;
	int64_t c = (mod + int64_t(cutoff));
	if(c > 65535) c = 65535;
	else if(c < 0) c = 0;

    b1 = filterCoefficient[c>>8];
    a0 = BIT_32 - b1;

    x0 = sample;

    y1 = filterSamplesLP24dB[0];
    y2 = filterSamplesLP24dB[1];
    y3 = filterSamplesLP24dB[2];
    y4 = filterSamplesLP24dB[3];

    y1 = (a0 * x0 + b1 * y1) >> 32;
    y2 = (a0 * y1 + b1 * y2) >> 32;
    y3 = (a0 * y2 + b1 * y3) >> 32;
    y4 = (a0 * y3 + b1 * y4) >> 32;

    filterSamplesLP24dB[0] = y1;
    filterSamplesLP24dB[1] = y2;
    filterSamplesLP24dB[2] = y3;
    filterSamplesLP24dB[3] = y4;

    sample = y4;
}



void MMusic::filterHP24dB() {

    int64_t mod = (int64_t(cutoffModAmount) * (int64_t(*cutoffModSource_ptr)))>>16;
    int64_t c = (mod + int64_t(cutoff));
    if(c > 65535) c = 65535;
    else if(c < 0) c = 0;

    b1 = filterCoefficient[c>>8];
    a0 = (BIT_32 + b1) >> 1;
    a1 = -a0;

    xNew = sample;
    xOld = filterSamplesHP24dB[0];
    yOld = filterSamplesHP24dB[4];
    yNew = (a0 * xNew + a1 * xOld + b1 * yOld) >> 32;
    x1 = xNew;
    y1 = yNew;

    xNew = y1;
    xOld = filterSamplesHP24dB[1];
    yOld = filterSamplesHP24dB[5];
    yNew = (a0 * xNew + a1 * xOld + b1 * yOld) >> 32;
    x2 = xNew;
    y2 = yNew;

    xNew = y2;
    xOld = filterSamplesHP24dB[2];
    yOld = filterSamplesHP24dB[6];
    yNew = (a0 * xNew + a1 * xOld + b1 * yOld) >> 32;
    x3 = xNew;
    y3 = yNew;

    xNew = y3;
    xOld = filterSamplesHP24dB[3];
    yOld = filterSamplesHP24dB[7];
    yNew = (a0 * xNew + a1 * xOld + b1 * yOld) >> 32;
    x4 = xNew;
    y4 = yNew;

    filterSamplesHP24dB[0] = x1;
    filterSamplesHP24dB[1] = x2;
    filterSamplesHP24dB[2] = x3;
    filterSamplesHP24dB[3] = x4;

    filterSamplesHP24dB[4] = y1;
    filterSamplesHP24dB[5] = y2;
    filterSamplesHP24dB[6] = y3;
    filterSamplesHP24dB[7] = y4;

    sample = y4;

}


void MMusic::filterMoogLadder() {

  int64_t mod = (int64_t(cutoffModAmount) * (int64_t(*cutoffModSource_ptr)))>>16;
	int64_t c = (mod + int64_t(cutoff));
	if(c > 65535) c = 65535;
	else if(c < 0) c = 0;

    int fc = c>>8;
    if(fc > 234) fc = 234;
    x0 = sample;
    u = x0;
    Gstage = filterCoefficientsMoogLadder[1024 + fc];

    v1 = ((u - z1) * Gstage) >> 32;
    y1 = (v1 + z1);
    z1 = y1 + v1;

    v2 = ((y1 - z2) * Gstage) >> 32;
    y2 = (v2 + z2);
    z2 = y2 + v2;

    v3 = ((y2 - z3) * Gstage) >> 32;
    y3 = (v3 + z3);
    z3 = y3 + v3;

    v4 = ((y3 - z4) * Gstage) >> 32;
    y4 = (v4 + z4);
    z4 = y4 + v4;

    sample = y4;

}


void MMusic::setFilterType(uint8_t type) {

    switch (type) {
        case LP6:
            lowpass = true;
            highpass = false;
            lowpass24dB = false;
            highpass24dB = false;
            moogLadder = false;
            break;
        case HP6:
            lowpass = false;
            highpass = true;
            lowpass24dB = false;
            highpass24dB = false;
            moogLadder = false;
            break;
        case BP6:
            lowpass = true;
            highpass = true;
            lowpass24dB = false;
            highpass24dB = false;
            moogLadder = false;
            break;
        case THRU:
            lowpass = false;
            highpass = false;
            lowpass24dB = false;
            highpass24dB = false;
            moogLadder = false;
            break;
        case LP24:
            lowpass = false;
            highpass = false;
            lowpass24dB = true;
            highpass24dB = false;
            moogLadder = false;
            break;
        case HP24:
            lowpass = false;
            highpass = false;
            lowpass24dB = false;
            highpass24dB = true;
            moogLadder = false;
            break;
        case BP24:
            lowpass = false;
            highpass = false;
            lowpass24dB = true;
            highpass24dB = true;
            moogLadder = false;
            break;
        case MOOG:
            lowpass = false;
            highpass = false;
            lowpass24dB = false;
            highpass24dB = false;
            moogLadder = true;
            break;
        default:
            break;
    }
}


void MMusic::setCutoffModShape(uint8_t shape) {
	switch(shape) {
		case 0:
			cutoffModShape_ptr = &fullSignal;
			break;
		case 1:
			cutoffModShape_ptr = &env1;
			break;
		case 2:
			cutoffModShape_ptr = &env2;
			break;
		case 3:
			cutoffModShape_ptr = &oscil1;
			break;
		case 4:
			cutoffModShape_ptr = &oscil2;
			break;
		case 5:
			cutoffModShape_ptr = &oscil3;
			break;
		default:
			cutoffModShape_ptr = &fullSignal;
			break;
	}
}


void MMusic::setCutoffModSource(uint8_t source) {
	switch(source) {
		case 0:
			cutoffModSource_ptr = &fullSignal;
			break;
		case 1:
			cutoffModSource_ptr = &env1;
			break;
		case 2:
			cutoffModSource_ptr = &env2;
			break;
		case 3:
			cutoffModSource_ptr = &oscil1;
			break;
		case 4:
			cutoffModSource_ptr = &oscil2;
			break;
		case 5:
			cutoffModSource_ptr = &oscil3;
			break;
		default:
			cutoffModSource_ptr = &fullSignal;
			break;
	}
}


void MMusic::setResonanceModShape(uint8_t shape) {
	switch(shape) {
		case 0:
			resonanceModShape_ptr = &fullSignal;
			break;
		case 1:
			resonanceModShape_ptr = &env1;
			break;
		case 2:
			resonanceModShape_ptr = &env2;
			break;
		case 3:
			resonanceModShape_ptr = &oscil1;
			break;
		case 4:
			resonanceModShape_ptr = &oscil2;
			break;
		case 5:
			resonanceModShape_ptr = &oscil3;
			break;
		default:
			resonanceModShape_ptr = &fullSignal;
			break;
	}
}


void MMusic::setResonanceModSource(uint8_t source) {
	switch(source) {
		case 0:
			resonanceModSource_ptr = &fullSignal;
			break;
		case 1:
			resonanceModSource_ptr = &env1;
			break;
		case 2:
			resonanceModSource_ptr = &env2;
			break;
		case 3:
			resonanceModSource_ptr = &oscil1;
			break;
		case 4:
			resonanceModSource_ptr = &oscil2;
			break;
		case 5:
			resonanceModSource_ptr = &oscil3;
			break;
		default:
			resonanceModSource_ptr = &fullSignal;
			break;
	}
}




/////////////////////////////////////
//
//	FREQUENCY AND DETUNE FUNCTIONS
//
/////////////////////////////////////

void MMusic::setFrequency(float freq)
{
	frequency = freq;
	if(!osc1LFO) setFrequency1(freq);
	if(!osc2LFO) setFrequency2(freq);
	if(!osc3LFO) setFrequency3(freq);
}


void MMusic::setFrequency1(float freq)
{
	frequency1 = freq;
	period1 = int32_t(((frequency1 * semi1 * (1 + detune1 + bend)) * PERIOD_MAX) / SAMPLE_RATE);
}


void MMusic::setFrequency2(float freq)
{
	frequency2 = freq;
	period2 = int32_t(((frequency2 * semi2 * (1 + detune2 + bend)) * PERIOD_MAX) / SAMPLE_RATE);
}


void MMusic::setFrequency3(float freq)
{
	frequency3 = freq;
	period3 = int32_t(((frequency3 * semi3 * (1 + detune3 + bend)) * PERIOD_MAX) / SAMPLE_RATE);
}


void MMusic::setSemitone1(int8_t semi)
{
	if(-25 < semi && semi < 25){
		semi1 = semitoneTable[semi+24];
	} else if (semi < -24) {
		semi1 = semitoneTable[0];
	} else {
		semi1 = semitoneTable[48];
	}
	setFrequency1(frequency1);
}


void MMusic::setSemitone2(int8_t semi)
{
	if(-25 < semi && semi < 25){
		semi2 = semitoneTable[semi+24];
	} else if (semi < -24) {
		semi2 = semitoneTable[0];
	} else {
		semi2 = semitoneTable[48];
	}
	setFrequency2(frequency2);
}


void MMusic::setSemitone3(int8_t semi)
{
	if(-25 < semi && semi < 25){
		semi3 = semitoneTable[semi+24];
	} else if (semi < -24) {
		semi3 = semitoneTable[0];
	} else {
		semi3 = semitoneTable[48];
	}
	setFrequency3(frequency3);
}


void MMusic::setDetune(float detune)
{
	detune1 = 0.0;
	detune2 = detune*0.123456789;
	detune3 = -detune;
	setFrequency2(frequency2);
	setFrequency3(frequency3);
}


void MMusic::setDetune1(float detune)
{
	detune1 = detune;
	setFrequency1(frequency1);
}


void MMusic::setDetune2(float detune)
{
	detune2 = detune;
	setFrequency2(frequency2);
}


void MMusic::setDetune3(float detune)
{
	detune3 = detune;
	setFrequency3(frequency3);
}


void MMusic::pitchBend(float b)
{
	bend = b;
	setFrequency1(frequency1);
	setFrequency2(frequency2);
	setFrequency3(frequency3);
}


void MMusic::setOsc1LFO(bool lfo) {
	osc1LFO = lfo;
}


void MMusic::setOsc2LFO(bool lfo) {
	osc2LFO = lfo;
}


void MMusic::setOsc3LFO(bool lfo) {
	osc3LFO = lfo;
}


void MMusic::setFM1(uint8_t fm) {
	fmAmount1 = fm;
}


void MMusic::setFM2(uint8_t fm) {
	fmAmount2 = fm;
}


void MMusic::setFM3(uint8_t fm) {
	fmAmount3 = fm;
}


void MMusic::setFMoctaves(uint8_t octs) {
	fmOctaves1 = octs;
	fmOctaves2 = octs;
	fmOctaves3 = octs;
}


void MMusic::setFM1octaves(uint8_t octs) {
	if(octs < 1) octs = 1;
	fmOctaves1 = octs;
}


void MMusic::setFM2octaves(uint8_t octs) {
	if(octs < 1) octs = 1;
	fmOctaves2 = octs;
}


void MMusic::setFM3octaves(uint8_t octs) {
	if(octs < 1) octs = 1;
	fmOctaves3 = octs;
}


void MMusic::setFM1Source(uint8_t source) {
	switch(source) {
		case 0:
			osc1modSource_ptr = &fullSignal;
			break;
		case 1:
			osc1modSource_ptr = &oscil1;
			break;
		case 2:
			osc1modSource_ptr = &oscil2;
			break;
		case 3:
			osc1modSource_ptr = &oscil3;
			break;
		default:
			osc1modSource_ptr = &fullSignal;
			break;
	}
}


void MMusic::setFM2Source(uint8_t source) {
	switch(source) {
		case 0:
			osc2modSource_ptr = &fullSignal;
			break;
		case 1:
			osc2modSource_ptr = &oscil1;
			break;
		case 2:
			osc2modSource_ptr = &oscil2;
			break;
		case 3:
			osc2modSource_ptr = &oscil3;
			break;
		default:
			osc1modSource_ptr = &fullSignal;
			break;
	}
}


void MMusic::setFM3Source(uint8_t source) {
	switch(source) {
		case 0:
			osc3modSource_ptr = &fullSignal;
			break;
		case 1:
			osc3modSource_ptr = &oscil1;
			break;
		case 2:
			osc3modSource_ptr = &oscil2;
			break;
		case 3:
			osc3modSource_ptr = &oscil3;
			break;
		default:
			osc1modSource_ptr = &fullSignal;
			break;
	}
}


void MMusic::setFM1Shape(uint8_t shape) {
	switch(shape) {
		case 0:
			osc1modShape_ptr = &fullSignal;
			break;
		case 1:
			osc1modShape_ptr = &env1;
			break;
		case 2:
			osc1modShape_ptr = &env2;
			break;
		case 3:
			osc1modShape_ptr = &oscil1;
			break;
		case 4:
			osc1modShape_ptr = &oscil2;
			break;
		case 5:
			osc1modShape_ptr = &oscil3;
			break;
		default:
			osc1modShape_ptr = &fullSignal;
			break;
	}
}


void MMusic::setFM2Shape(uint8_t shape) {
	switch(shape) {
		case 0:
			osc2modShape_ptr = &fullSignal;
			break;
		case 1:
			osc2modShape_ptr = &env1;
			break;
		case 2:
			osc2modShape_ptr = &env2;
			break;
		case 3:
			osc2modShape_ptr = &oscil1;
			break;
		case 4:
			osc2modShape_ptr = &oscil2;
			break;
		case 5:
			osc2modShape_ptr = &oscil3;
			break;
		default:
			osc2modShape_ptr = &fullSignal;
			break;
	}
}


void MMusic::setFM3Shape(uint8_t shape) {
	switch(shape) {
		case 0:
			osc3modShape_ptr = &fullSignal;
			break;
		case 1:
			osc3modShape_ptr = &env1;
			break;
		case 2:
			osc3modShape_ptr = &env2;
			break;
		case 3:
			osc3modShape_ptr = &oscil1;
			break;
		case 4:
			osc3modShape_ptr = &oscil2;
			break;
		case 5:
			osc3modShape_ptr = &oscil3;
			break;
		default:
			osc3modShape_ptr = &fullSignal;
			break;
	}
}


void MMusic::fmToZeroHertz(bool zero) {
	if(!zero) zeroFM = 1;
	else zeroFM = 0;
}

void MMusic::setPortamento(int32_t port) {
	if(port == 0) port = 1;
	portamento = port;
}



/////////////////////////////////////
//
//	WAVEFORM FUNCTIONS
//
/////////////////////////////////////


void MMusic::setWaveform(uint16_t waveForm)
{
	waveForm1 = waveForm * 256;
	waveForm2 = waveForm * 256;
	waveForm3 = waveForm * 256;
}


void MMusic::setWaveform1(uint16_t waveForm)
{
	waveForm1 = waveForm * 256;
}


void MMusic::setWaveform2(uint16_t waveForm)
{
	waveForm2 = waveForm * 256;
}


void MMusic::setWaveform3(uint16_t waveForm)
{
	waveForm3 = waveForm * 256;
}




/////////////////////////////////////
//
//	GAIN FUNCTIONS
//
/////////////////////////////////////


void MMusic::setGain(float value)
{
	gain = uint16_t(value * 65535);
	gain1 = gain;
	gain2 = gain;
	gain3 = gain;
}


void MMusic::setGain1(float value)
{
	gain1 = uint16_t(value * 65535);
}


void MMusic::setGain2(float value)
{
	gain2 = uint16_t(value * 65535);
}


void MMusic::setGain3(float value)
{
	gain3 = uint16_t(value * 65535);
}


float MMusic::getGain()
{
	return float(gain)/65535.0;
}


float MMusic::getGain1()
{
	return float(gain1)/65535.0;
}


float MMusic::getGain2()
{
	return float(gain2)/65535.0;
}


float MMusic::getGain3()
{
	return float(gain3)/65535.0;
}




/////////////////////////////////////
//
//	NOTE_ON/OFF FUNCTIONS
//
/////////////////////////////////////


void MMusic::noteOn(uint8_t note, uint8_t vel)
{
	env1Stage = 1;
	env2Stage = 1;
	setEnv1VelSustain(vel);
	setEnv2VelSustain(vel);
	setEnv1VelPeak(vel);
	setEnv2VelPeak(vel);
	notePlayed = note;
	frequency16bit = hertzTable[notePlayed];
	setFrequency(frequency16bit);
}


void MMusic::noteOn(uint8_t note)
{
	int vel = 127;
	env1Stage = 1;
	env2Stage = 1;
	setEnv1VelSustain(vel);
	setEnv2VelSustain(vel);
	setEnv1VelPeak(vel);
	setEnv2VelPeak(vel);
	notePlayed = note;
	frequency16bit = hertzTable[notePlayed];
	setFrequency(frequency16bit);
}


void MMusic::noteOff(uint8_t note)
{
  notePlayed = note;
	if(notePlayed) {  // only responding if noteOff note is _not_ 0
//	if(notePlayed = note) {  //
		env1Stage = 4;
		env2Stage = 4;
	}
}


void MMusic::noteOff()
{
	env1Stage = 4;
	env2Stage = 4;
}


float MMusic::getNoteFrequency(uint8_t note)
{
	return hertzTable[note];
}




/////////////////////////////////////
//
//	ENVELOPE FUNCTIONS
//
/////////////////////////////////////


// ENVELOPE 1

void MMusic::enableEnvelope1()
{
	envelopeOn1 = true;
}


void MMusic::disableEnvelope1()
{
	envelopeOn1 = false;
}


void MMusic::setEnv1Stage(uint8_t stage)
{
	env1Stage = stage;
}


void MMusic::setEnv1Attack(uint8_t att)
{
	if(att>127) att = 127;
	attack1 = envTimeTable[att];
}


void MMusic::setEnv1Decay(uint8_t dec)
{
	if(dec>127) dec = 127;
	decay1 = envTimeTable[dec];
}


void MMusic::setEnv1Sustain(uint8_t sus)
{
	sustain1 = ((sus * MAX_ENV_GAIN)/128);
}


void MMusic::setEnv1Release(uint8_t rel)
{
	if(rel>127) rel = 127;
	release1 = envTimeTable[rel];
}


void MMusic::setEnv1VelSustain(uint8_t vel)
{
	velSustain1 = vel * (sustain1 / 128);
}


void MMusic::setEnv1VelPeak(uint8_t vel)
{
	velPeak1 = vel * (MAX_ENV_GAIN / 128);
}


// ENVELOPE 2

void MMusic::enableEnvelope2()
{
	envelopeOn2 = true;
}


void MMusic::disableEnvelope2()
{
	envelopeOn2 = false;
}


void MMusic::setEnv2Stage(uint8_t stage)
{
	env2Stage = stage;
}


void MMusic::setEnv2Attack(uint8_t att)
{
	if(att>127) att = 127;
	attack2 = envTimeTable[att];
}


void MMusic::setEnv2Decay(uint8_t dec)
{
	if(dec>127) dec = 127;
	decay2 = envTimeTable[dec];
}


void MMusic::setEnv2Sustain(uint8_t sus)
{
	sustain2 = ((sus * MAX_ENV_GAIN)/128);
}


void MMusic::setEnv2Release(uint8_t rel)
{
	if(rel>127) rel = 127;
	release2 = envTimeTable[rel];
}


void MMusic::setEnv2VelSustain(uint8_t vel)
{
	velSustain2 = vel * (sustain2 / 128);
}


void MMusic::setEnv2VelPeak(uint8_t vel)
{
	velPeak2 = vel * (MAX_ENV_GAIN / 128);
}


// TODO: Evaluate if this function is really needed.
// It only exists, because MMidi::controller is calling it.
void MMusic::setInstrument(uint8_t number, uint8_t value){
	instrument[number] = value;
}
