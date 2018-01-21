/*
Name:    Teensy_Squares.ino
Created: 10/4/2016 1:24:27 PM
Author:  dehab
*/
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#include "Arduino.h"
#define USE_OCTOWS2811
#include <OctoWS2811.h>
#include <FastLED.h>
#include <Statistic.h>
#include <Button.h>
#define NUM_LEDS 343
CRGB leds[NUM_LEDS];
#define inputSwitch 4
#define reactiveSwitch 10
#define brightnessPot A14
#define buttonOne 12
#define buttonTwo 30

// Pin layouts on the teensy 3:
// OctoWS2811: 2,14,7,8,6,20,21,5

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>
// GUItool: begin automatically generated code
AudioInputI2S i2s1; //xy=219.1999969482422,183
AudioMixer4 mixer1; //xy=403.1999969482422,203.1999969482422
AudioAnalyzeFFT1024 fft1024; //xy=547.1999969482422,239
AudioConnection patchCord1(i2s1, 0, mixer1, 0);
AudioConnection patchCord2(i2s1, 1, mixer1, 1);
AudioConnection patchCord3(mixer1, fft1024);
AudioControlSGTL5000 audioShield; //xy=446.1999969482422,317
								  // GUItool: end automatically generated code
								  //const int myInput = AUDIO_INPUT_LINEIN;
								  //const int myInput = AUDIO_INPUT_MIC;

int spectrumValue[15];
int spectrumValueOld[15];
int spectrumValueOldOld[15];
int lowThresh, highThresh, midThresh;
int ledx[NUM_LEDS], ledy[NUM_LEDS];                // Used for giving every LED an x and y coordinate

int cor0 = 0, cor1 = 26, cor2 = 35, cor3 = 45, cor4 = 54, cor5 = 63, cor6 = 71, cor7 = 79, cor8 = 87, cor9 = 97, cor10 = 105;
int cor11 = 121, cor12 = 138, cor13 = 147, cor14 = 172, cor15 = 197, cor16 = 206, cor17 = 222, cor18 = 240, cor19 = 248;
int cor20 = 257, cor21 = 267, cor22 = 276, cor23 = 286, cor24 = 294, cor25 = 302, cor26 = 310, cor27 = 319, cor28 = 343;

// lightSection()
int start, end;

// CircleOne
elapsedMillis time1, time2;
double radius;
int brightness;
int side;

// Boucning Zack
int topRightVar = 30;
int botLeftVar = 30;
int topRightColor, topRightCase;
int botLeftColor, botLeftCase;
int start1, start2, start3;
int end1, end2, end3;
int start0, start00, end0, end00;
int botLeft1, botLeft2, botLeft3, botLeft4;
int topRight1, topRight2, topRight3, topRight4;
int mainLight;
int var;
int z = 1;
int topRightPP = 0;
int botLeftbBegin = 0;

double speed1 = 100;
double speed2 = 100;
double speed3 = 100;

// CoordinatesToBeat
int slope, coordMode;
int yint;
int eq;
// Experimental
int coordModeMain = 1;
bool pingPong = 0;
int coordModeTimer;
bool sweepStatus;

int pot;

// Vertical Bars
int length, length1;
int color;

// Coordinates
int a;

// Rainbow beat
int r, g, b;
int counter;

// Dynamic Fading
int val;
int Rcounter = 100;
int Gcounter = 100;
int Bcounter = 100;
int redFadeVar;
int greenFadeVar;
int blueFadeVar;

/// TRAVELING DOTS
int meterMode = 1;
elapsedMillis meterModeTimer;
int meterModeDelayTime = 10000;
int bottomDots[] = { cor10, cor12 };
int bottomDotZones[] = { 1, 1 };
double bottomDotSpeed;
elapsedMillis bottomDotTime;
/// Top TRAVELING DOTS
int topDotZones[] = { 1, 1 };
int topDots[] = { cor16, cor18 };
double topDotSpeed;
elapsedMillis topDotTime;

// Random Squares
int volume, volumeOld, box;

// Confetti
uint8_t colorOne = 0;
uint8_t colorTwo = 160;
// and the speed1,2,3, under bouncing zack ^

// PingPongSquares
bool lowFreqSquare = 0;
bool highFreqSquare = 0;

// Inputs
elapsedMillis pennyDelay;
int pennyCounter;
int masterBrightness;
int pennyNew, pennyOld;
int mode = 15;

// Ambient Modes
elapsedMillis time;
double modeSpeed = 10000;
double speed;
int sat;
int pennyTwo;
bool autoSwitch = 1;  // 1 = not switching, 0 = autoswitch mode

					  // Beat Meter
int yintMid = 0;
int yintLow = -50;
int yintHigh = 50;
double highSpeed;
double lowSpeed;

int myInput;

// STATS
Statistic freq[14];
Statistic freq15;

// Automation
bool lowBeats = false;
bool highBeats = false;
bool midBeats = false;
bool silence = false;
uint8_t lowBeatCounter = 0;
uint8_t midBeatCounter = 0;
uint8_t highBeatCounter = 0;
uint8_t musicState = 0;
uint8_t musicStateOld = 0;

// Transition
bool transitionTime = false;

Button button1(buttonOne);
Button button2(buttonTwo);

void setup() {
	//pinMode(13, OUTPUT);
	//digitalWrite(13, LOW);
	button1.begin();
	button2.begin();

	pinMode(inputSwitch, INPUT_PULLUP);
	pinMode(reactiveSwitch, INPUT_PULLUP);
	if (digitalRead(inputSwitch) == 1) {
		myInput = AUDIO_INPUT_LINEIN;
		lowThresh = 250;  // old 400 but 300 as of 6/1
		highThresh = 300; // old 450 but 350 as of 6/1
		midThresh = 150;  // old 250
		Serial.println("Input is Aux");
	}
	else {
		myInput = AUDIO_INPUT_MIC;
		lowThresh = 100;
		highThresh = 100;
		midThresh = 80;
		Serial.println("Input is Mic");
	}
	// AUDIO STUFF
	AudioMemory(12);
	audioShield.enable();
	audioShield.inputSelect(myInput);

	// configure the mixer to equally add left & right
	mixer1.gain(0, 0.5);
	mixer1.gain(1, 0.5);

	Serial.begin(9600);
	LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS);
	for (int i = 0; i < NUM_LEDS; i++) leds[i] = 0;

	for (int i = cor0; i < cor1; i++) { ledx[i] = i - 17;            ledy[i] = -17; }
	for (int i = cor1; i < cor2; i++) { ledx[i] = 8;                 ledy[i] = i - cor1 - 16; }
	for (int i = cor2; i < cor3; i++) { ledx[i] = i - cor2 + 9;      ledy[i] = -8; }
	for (int i = cor3; i < cor4; i++) { ledx[i] = 18;                ledy[i] = -i + cor3 - 10; }
	for (int i = cor4; i < cor5; i++) { ledx[i] = -i + cor4 + 17;    ledy[i] = -18; }
	for (int i = cor5; i < cor6; i++) { ledx[i] = i - cor5 + 9;      ledy[i] = -16; }
	for (int i = cor6; i < cor7; i++) { ledx[i] = 16;                ledy[i] = i - cor6 - 16; }
	for (int i = cor7; i < cor8; i++) { ledx[i] = -i + cor7 + 16;    ledy[i] = -10; }
	for (int i = cor8; i < cor9; i++) { ledx[i] = 10;                ledy[i] = -i + cor8 - 10; }
	for (int i = cor9; i < cor10; i++) { ledx[i] = 10;               ledy[i] = -i + cor9 - 18; }
	for (int i = cor10; i < cor11; i++) { ledx[i] = i - cor10 + 10;  ledy[i] = -25; }
	for (int i = cor11; i < cor12; i++) { ledx[i] = 25;              ledy[i] = i - cor11 - 24; }
	for (int i = cor12; i < cor13; i++) { ledx[i] = -i + cor12 + 24;  ledy[i] = -8; }
	for (int i = cor13; i < cor14; i++) { ledx[i] = 17;              ledy[i] = i - cor13 - 8; }
	for (int i = cor14; i < cor15; i++) { ledx[i] = -i + cor14 + 16; ledy[i] = 17; }
	for (int i = cor15; i < cor16; i++) { ledx[i] = -10;             ledy[i] = i - cor15 + 18; }
	for (int i = cor16; i < cor17; i++) { ledx[i] = -i + cor16 - 10; ledy[i] = 25; }
	for (int i = cor17; i < cor18; i++) { ledx[i] = -25;             ledy[i] = -i + cor17 + 24; }
	for (int i = cor18; i < cor19; i++) { ledx[i] = i - cor18 - 25;   ledy[i] = 10; }
	for (int i = cor19; i < cor20; i++) { ledx[i] = -18;             ledy[i] = i - cor19 + 10; }
	for (int i = cor20; i < cor21; i++) { ledx[i] = i - cor20 - 16;  ledy[i] = 18; }
	for (int i = cor21; i < cor22; i++) { ledx[i] = -8;              ledy[i] = -i + cor21 + 16; }
	for (int i = cor22; i < cor23; i++) { ledx[i] = -i + cor22 - 10; ledy[i] = 8; }
	for (int i = cor23; i < cor24; i++) { ledx[i] = i - cor23 - 16;  ledy[i] = 10; }
	for (int i = cor24; i < cor25; i++) { ledx[i] = -10;             ledy[i] = i - cor24 + 10; }
	for (int i = cor25; i < cor26; i++) { ledx[i] = -i + cor25 - 10; ledy[i] = 16; }
	for (int i = cor26; i < cor27; i++) { ledx[i] = -16;             ledy[i] = -i + cor26 + 16; }
	for (int i = cor27; i < cor28; i++) { ledx[i] = -17;             ledy[i] = -i + cor27 + 8; }

	//STATS
	clearStats();
	freq15.clear();

	Serial.println("Setup Complete.");
}

// The current average and standard deviation for each of 15 frequencies
double average[14];
double stDev[14];

// These are lists of functions that will be displayed during different types of music / no music.
// Music is playing but no beats are being detected
typedef void(*musicWithNoBeatPatternsList[])();
musicWithNoBeatPatternsList musicWithNoBeatPatterns = { confetti, sixFrequencyGlitter, beatMeter, verticalBars, sixFrequencyGlitter };
// No music is playing, so display an ambient function
typedef void(*ambientPatternList[])();
ambientPatternList ambientPatterns = { ambient_rainbow, ambient_rainbowFade, ambient_confetti, coordinates, rainbowBeat };
// Both high frequency beats and low frequency beats are being detected
typedef void(*spectrumPatternList[])();
spectrumPatternList spectrumPatterns = { travelingDotsToHighAndLow, pingPongSquares, rainbowBeat, sixFrequencyGlitter, confetti, coordinatesToBeatExperimental, beatMeter };
// Only beats in the low frequency are being detected
typedef void(*lowBeatPatternList[])();
lowBeatPatternList lowBeatPatterns = { travelingDotsToLowBeats, coordinatesToBeat, bouncingZack_withFade, coordinatesToBeatExperimental, circle_midOut_One, bouncingZack_withoutFade, circle_midOut_One_Experimental };

// The variables used to play a function within the lists above
uint8_t musicWithNoBeatPatternNumber = 0;
uint8_t currentLowBeatPatternNumber = 0;
uint8_t currentSpectrumPatternNumber = 0;
uint8_t currentPatternNumber = 0;
uint8_t currentAmbPatternNumber = 0;
uint8_t hue = 0;

elapsedMillis silenceDuration;

// These functions choose a random pattern within the lists above
void nextMusicWithNoBeatPattern() {
	// add one to the current pattern number, and wrap around at the end
	//musicWithNoBeatPatternNumber = (musicWithNoBeatPatternNumber + 1) % ARRAY_SIZE(musicWithNoBeatPatterns);
	musicWithNoBeatPatternNumber = random8(ARRAY_SIZE(musicWithNoBeatPatterns));
}

void nextAmbPattern() {
	//currentAmbPatternNumber = (currentAmbPatternNumber + 1) % ARRAY_SIZE(ambientPatterns);
	currentAmbPatternNumber = random8(ARRAY_SIZE(ambientPatterns));
}

void nextLowBeatPattern() {
	//currentLowBeatPatternNumber = (currentLowBeatPatternNumber + 1) % ARRAY_SIZE(lowBeatPatterns);
	currentLowBeatPatternNumber = random8(ARRAY_SIZE(lowBeatPatterns));
}

void nextSpectrumPattern() {
	//currentSpectrumPatternNumber = (currentSpectrumPatternNumber + 1) % ARRAY_SIZE(spectrumPatterns);
	currentSpectrumPatternNumber = random8(ARRAY_SIZE(spectrumPatterns));
}

void clearStats() {
	for (int i = 0; i < 15; i++) freq[i].clear();
}

void fillStats() {
	spectrumValue[0] = fft1024.read(0) * 1000;
	spectrumValue[1] = fft1024.read(1) * 1000;
	spectrumValue[2] = fft1024.read(2, 3) * 1000;
	spectrumValue[3] = fft1024.read(4, 6) * 1000;
	spectrumValue[4] = fft1024.read(7, 10) * 1000;
	spectrumValue[5] = fft1024.read(11, 15) * 1000;
	spectrumValue[6] = fft1024.read(16, 22) * 1000;
	spectrumValue[7] = fft1024.read(23, 32) * 1000;
	spectrumValue[8] = fft1024.read(33, 46) * 1000;
	spectrumValue[9] = fft1024.read(47, 66) * 1000;
	spectrumValue[10] = fft1024.read(67, 93) * 1000;
	spectrumValue[11] = fft1024.read(94, 131) * 1000;
	spectrumValue[12] = fft1024.read(132, 184) * 1000;
	spectrumValue[13] = fft1024.read(185, 257) * 1000;
	spectrumValue[14] = fft1024.read(258, 359) * 1000;

	// Adjust the standard deviation and average of each array value as the song goes
	for (int i = 0; i < 14; i++) {
		freq[i].add(spectrumValue[i]);
		average[i] = freq[i].average();
		stDev[i] = freq[i].pop_stdev();
	}

	uint8_t silenceCounter = 0;
	// If over 13 of the 15 frequencies are silent, 
	// reset the statistics because it's likely the song ended
	for (int i = 0; i < 15; i++) {
		if (spectrumValue[i] < 3) // 3 is an arbitrary number but it means it is basically silent
			silenceCounter++;
		else
			silenceDuration = 0;
		break;
	}
	if (silenceCounter > 13) {
		clearStats();
	}

	if (silenceDuration > 5000) {
		silence =
			true;
		lowBeats = false;
		midBeats = false;
		highBeats = false;
	}
	else silence = false;

	//Serial.print("  Count: ");
	//Serial.print(freq2.count());
	//Serial.print("  Average: ");
	//Serial.print(freq2.average(), 4);
	//Serial.print("  Std deviation: ");
	//Serial.print(freq2.pop_stdev(), 4);
	//Serial.print("   Value: ");
	//Serial.print(fft1024.read(2, 3) * 1000);
	//Serial.println();
}

int beatDetected[14] = {};
void beatDetection() {
	// 0 = no beat detected
	// 1 = beat hasn't dropped / reset yet
	// 2 = beat detected
	for (int i = 0; i < 15; i++) {
		// Beat is detected here. Must be greater than the average+(3*st.dev) and greater than 100 which is pretty quiet
		if (spectrumValue[i] > average[i] + 3 * stDev[i] && spectrumValue[i] > 100) {
			if (beatDetected[i] == 2)
				beatDetected[i] = 1;
			if (beatDetected[i] == 0)
				beatDetected[i] = 2;
			// if it's == 1, it stays 1. 
		}
		// This is where 1's get reset to 0. If beat is not detected in that frequency, set it's status to 0.
		else
			//if (spectrumValue[i] < average[i] + 1*stDev[i]) 
			beatDetected[i] = 0;
	}

	// If there is beat detected in 1 of these 3 frequencies, then increment their counter
	if (beatDetected[2] == 2) lowBeatCounter++;
	if (beatDetected[8] == 2) midBeatCounter++;
	if (beatDetected[12] == 2) highBeatCounter++;

	if (musicState == 0) { // If there is silence and any of the counters go from 0 to 1, a music reactive mode starts
		if (lowBeatCounter > 0 || midBeatCounter > 0 || highBeatCounter > 0) musicState = 3;
	}

	EVERY_N_BSECONDS(10) {
		musicStateOld = musicState;

		lowBeats = (lowBeatCounter > 5) ? true : false;
		midBeats = (midBeatCounter > 5) ? true : false;
		highBeats = (highBeatCounter > 5) ? true : false;

		lowBeatCounter = 0;
		midBeatCounter = 0;
		highBeatCounter = 0;

		/*  if (lowBeats > 5 && lowBeats > 2 * highBeats) musicState = 2;
		else if ()*/

		// loop() {} uses music states to choose which function list to pull a function from
		if (lowBeats && highBeats) musicState = 3;
		else if (lowBeats) musicState = 2;
		else if (silence) musicState = 0;
		else if (!silence && !lowBeats && !midBeats && !highBeats) musicState = 4;
		else musicState = 1;

		if (musicState == musicStateOld) {
			if (musicState == 1 || musicState == 3)
				nextSpectrumPattern();
			else if (musicState == 2)
				nextLowBeatPattern();
			else nextAmbPattern();
		}
	}
}

void transition() {
	slope = 0;
	yint = 28 - var;
	if (transitionTime) {
		var++;

		delay(8);

		for (int i = 0; i < NUM_LEDS; i++) {
			if (slope * ledy[i] + yint - ledx[i] < 1 && slope *ledy[i] + yint - ledx[i] > -1) leds[i] = CHSV(255, 255, 255);
		}
	}
	if (yint == -28)
		transitionTime = false;
}

void loop() {
	masterBrightness = map(analogRead(brightnessPot), 0, 1023, 0, 100);
	LEDS.setBrightness(masterBrightness);

	fillStats();
	beatDetection();

	// SECOND PENNY = autoswitching toggle
	if (button1.released()) {
	  leds[0] = CRGB::White;
	  LEDS.show();
	  autoSwitch = !autoSwitch;
	}
	if (autoSwitch == 1) {  // If in autoswitch mode, change the mode every 20 seconds
	  if (digitalRead(inputSwitch) == 1) {   // If switch is in position for music reacting mode, do this
		  if (button2.released()) {
	      //mode++;
	      nextAmbPattern();
	      nextMusicWithNoBeatPattern();
	      nextLowBeatPattern();
	      nextSpectrumPattern();

	      leds[0] = CRGB::White;
	      LEDS.show();
	    }
	    if (musicState == 1 || musicState == 3) spectrumPatterns[currentSpectrumPatternNumber]();
	    else if (musicState == 4) musicWithNoBeatPatterns[musicWithNoBeatPatternNumber]();
	    else if (musicState == 0) ambientPatterns[currentAmbPatternNumber]();
	    else lowBeatPatterns[currentLowBeatPatternNumber]();
	  }
	else {  // Else, if the switch is in position for ambient modes, do this
	  if (button2.released()) nextAmbPattern();
	  ambientPatterns[currentAmbPatternNumber]();
		}
	}
	//else {
	//  ambient_confetti();
	//}


	//  switch (mode)
	//  {
	//  default:
	//    mode = 1;
	//    break;
	//  case 1:
	//    //coordinatesToBeat();
	//    coordinatesToBeatExperimental();
	//    break;
	//  case 2:
	//    circle_midOut_One();
	//    break;
	//  case 3:
	//    verticalBars();
	//    break;
	//  case 4:
	//    pingPongSquares();
	//    //randomSquares();
	//    break;
	//  case 5:
	//    bouncingZack(254, .3);
	//    break;
	//  case 6:
	//    bouncingZack(255, .3);
	//    break;
	//  case 7:
	//    travelingDots(1);
	//    break;
	//  case 8:
	//    travelingDots(2);
	//    break;
	//  case 9:
	//    travelingDots(3);
	//    break;
	//  case 10:
	//    rainbowBeat();
	//    break;
	//  case 11:
	//    confetti();
	//    break;
	//  case 12:
	//    coordinatesToBeat();
	//    break;
	//  case 13:
	//    coordinatesToBeatExperimental();
	//    break;
	//  case 14:
	//    sixFrequencyGlitter();
	//    break;
	//  case 15:
	//    beatMeter();
	//    break;
	//  case 16:
	//    circle_midOut_One_Experimental();
	//    break;
	//  case 17:
	//    mode = 1;
	//    break;
	//  }
	//}
	//else {
	//  if (pennyPress(1)) mode++;
	//  if (mode >= 8) mode = 1;
	//  switch (mode) {
	//  case 1:
	//    ambient_confetti();
	//    break;
	//  case 2:
	//    ambient_rainbowFade();
	//    break;
	//  case 3:
	//    ambient_solidColor();
	//    break;
	//  case 4:
	//    var = 0;
	//    mode = 5;
	//    break;
	//  case 5:
	//    coordinates();
	//    break;
	//  case 6:
	//    ambient_rainbow();
	//    break;
	//  case 7:
	//    mode = 1;
	//    break;
	//  }
	//}

	////// DIAGNOSTIC FUNCTIONS
	//forTesting();
	//displayCorners();
	//printSpectrum();
}

void getSpectrum(int spectrumValue[], int spectrumValueOld[], int spectrumValueOldOld[]) {

	for (int i = 0; i < 15; i++) {
		spectrumValueOldOld[i] = spectrumValueOld[i];    //Create data set 3
		spectrumValueOld[i] = spectrumValue[i];          //Create data set 2
	}
	//Create data set 1
	spectrumValue[0] = fft1024.read(0) * 1000;           //Old value 0
	spectrumValue[1] = fft1024.read(1) * 1000;
	spectrumValue[2] = fft1024.read(2, 3) * 1000;         //Old Value 1
	spectrumValue[3] = fft1024.read(4, 6) * 1000;
	spectrumValue[4] = fft1024.read(7, 10) * 1000;        //Old Value 2
	spectrumValue[5] = fft1024.read(11, 15) * 1000;
	spectrumValue[6] = fft1024.read(16, 22) * 1000;       //Old Value 3
	spectrumValue[7] = fft1024.read(23, 32) * 1000;
	spectrumValue[8] = fft1024.read(33, 46) * 1000;       //Old Value 4
	spectrumValue[9] = fft1024.read(47, 66) * 1000;
	spectrumValue[10] = fft1024.read(67, 93) * 1000;      //Old Value 5
	spectrumValue[11] = fft1024.read(94, 131) * 1000;
	spectrumValue[12] = fft1024.read(132, 184) * 1000;    //Old Value 6
	spectrumValue[13] = fft1024.read(185, 257) * 1000;
	spectrumValue[14] = fft1024.read(258, 359) * 1000;    //Old Value 7
	spectrumValue[15] = fft1024.read(360, 511) * 1000;
}

void printNumber(float n) {
	if (n >= 0.004) {
		Serial.print(n, 3);
		Serial.print(" ");
	}
	else {
		//  Serial.print("   -  "); // don't print "0.00"
		Serial.print("   0  "); // print 0 for excel purposes
	}
}

void printSpectrum() {
	////// UNCOMMENT THEse LINES TO PRINT THE FFT WHILE PLAYING A SONG
	if (fft1024.available()) {
		// each time new FFT data is available
		// print to the Arduino Serial Monitor
		//Serial.print("FFT: ");
		printNumber(fft1024.read(0) * 1000);
		printNumber(fft1024.read(1) * 1000);
		printNumber(fft1024.read(2, 3) * 1000);
		printNumber(fft1024.read(4, 6) * 1000);
		printNumber(fft1024.read(7, 10) * 1000);
		printNumber(fft1024.read(11, 15) * 1000);
		printNumber(fft1024.read(16, 22) * 1000);
		printNumber(fft1024.read(23, 32) * 1000);
		printNumber(fft1024.read(33, 46) * 1000);
		printNumber(fft1024.read(47, 66) * 1000);
		printNumber(fft1024.read(67, 93) * 1000);
		printNumber(fft1024.read(94, 131) * 1000);
		printNumber(fft1024.read(132, 184) * 1000);
		printNumber(fft1024.read(185, 257) * 1000);
		printNumber(fft1024.read(258, 359) * 1000);
		printNumber(fft1024.read(360, 511) * 1000);
		Serial.println();
	}
}

void circle_midOut_One() {   // This mode shows a circle expanding from the middle

	if (beatDetected[2] == 2) {
		radius = 10;
		time1 = 0;
	}

	brightness = 255 - (radius - 10) * 6;   // Makes brightness decrease as radius goes up (radius goes up)

	if (time1 > radius - 10) {
		radius = radius + .5;
		time1 = 0;
	}

	if (radius < 36) {
		hue = hue + 2;
	}

	switch (side) {
	case 0:
		for (int i = cor0; i < cor14; i++) {
			if (pow(ledx[i], 2) + pow(ledy[i], 2) - pow(radius, 2) > -20 && pow(ledx[i], 2) + pow(ledy[i], 2) - pow(radius, 2) < 20) leds[i] = CHSV(hue, 255, brightness);
		}
		side = 1;
		break;
	case 1:
		for (int i = cor14; i < cor28; i++) {
			if (pow(ledx[i], 2) + pow(ledy[i], 2) - pow(radius, 2) > -20 && pow(ledx[i], 2) + pow(ledy[i], 2) - pow(radius, 2) < 20) leds[i] = CHSV(hue, 255, brightness);
		}
		side = 0;
		break;
	}

	LEDS.show();
}

void bouncingZack(int fade, double friction) {
	getSpectrum(spectrumValue, spectrumValueOld, spectrumValueOldOld);

	if (spectrumValueOld[2] > 1.3 * spectrumValueOldOld[2] && spectrumValueOld[2] > spectrumValue[2] * .8 && spectrumValueOld[2] > lowThresh) {
		//if (beatDetectRed(spectrumValue[2], spectrumValueOld[2], spectrumValueOldOld[2]) == 1) {
		speed1 -= .85*speed1;
	}

	if (time1 > speed1) { // If enough time has passed (time1), then move the light
		mainLight++;
		topRightVar++;
		botLeftVar++;
		time1 = 0;
	}
	switch (z) {
	case 1: // Move left on bottom and up on right
		leds[cor13 + mainLight] = CHSV(hue, 255, 255);
		if (cor13 + mainLight < cor15 - 1) leds[cor13 + mainLight + 1] = CHSV(hue + 1 * 2, 255, 200);
		if (cor13 + mainLight < cor15 - 2) leds[cor13 + mainLight + 2] = CHSV(hue + 2 * 2, 255, 140);
		if (cor13 + mainLight < cor15 - 3) leds[cor13 + mainLight + 3] = CHSV(hue + 3 * 2, 255, 90);
		if (cor13 + mainLight < cor15 - 4) leds[cor13 + mainLight + 4] = CHSV(hue + 4 * 2, 255, 65);

		if (cor1 - mainLight > 0) {
			leds[cor1 - mainLight - 1] = CHSV(hue, 255, 255);  // As long as the light, starting at cor3 and moving right, is still infront of the bottom right corner, we're good
			if (cor1 - mainLight > 1) {
				leds[cor1 - mainLight - 2] = CHSV(hue, 0, 20);  // Light up the LED in front of the dot if there is room (aka it hasn't hit the end yet)
			}
		}
		else leds[cor28 - mainLight + 26] = CHSV(hue, 255, 255);
		if (cor13 + mainLight == cor15 - 1) {
			topRightVar = 0;
			mainLight = 0;
			topRightColor = hue;
			z = 2;
		}
		break;
	case 2: // Move right on top and down on left
		leds[cor15 - 1 - mainLight] = CHSV(hue, 255, 255);
		if (cor15 - 1 - mainLight > cor13) leds[cor15 - 2 - mainLight] = CHSV(hue + 1 * 2, 255, 200);
		if (cor15 - 1 - mainLight > cor13 + 1) leds[cor15 - 3 - mainLight] = CHSV(hue + 2 * 2, 255, 140);
		if (cor15 - 1 - mainLight > cor13 + 2) leds[cor15 - 4 - mainLight] = CHSV(hue + 3 * 2, 255, 90);
		if (cor15 - 1 - mainLight > cor13 + 3) leds[cor15 - 5 - mainLight] = CHSV(hue + 4 * 2, 255, 65);  // Don't know how to fix it
		if (cor27 + mainLight < NUM_LEDS) leds[cor27 + mainLight] = CHSV(hue, 255, 255);
		else leds[cor0 + mainLight - 24] = CHSV(hue, 255, 255);
		if (cor15 - 1 - mainLight == cor13) {
			botLeftVar = 0;
			mainLight = 0;
			botLeftColor = hue;
			z = 1;
		}
		break;
	}

	bouncingZackSmallSquares(botLeftColor, topRightColor);

	hue++;
	if (hue == 255) hue = 0;
	speed1 = constrain(speed1 + friction + .01*speed1, 0, 200);
	var = map(speed1, 0, 200, 244, 250);
	fadeBlue(250);
	fadeRange(fade, cor1, cor13); // Bottom right square fade
	fadeRange(fade, cor15, cor27); // Top left sqaure fade
	LEDS.show();
}

void bouncingZack_withFade() {
	bouncingZack(254, .3);
	//double friction = .3;

	//if (beatDetected[2] == 2) {
	//  //if (beatDetectRed(spectrumValue[2], spectrumValueOld[2], spectrumValueOldOld[2]) == 1) {
	//  speed1 -= .85*speed1;
	//}

	//if (time1 > speed1) { // If enough time has passed (time1), then move the light
	//  mainLight++;
	//  topRightVar++;
	//  botLeftVar++;
	//  time1 = 0;
	//}
	//switch (z) {
	//case 1: // Move left on bottom and up on right
	//  leds[cor13 + mainLight] = CHSV(hue, 255, 255);
	//  if (cor13 + mainLight < cor15 - 1) leds[cor13 + mainLight + 1] = CHSV(hue + 1 * 2, 255, 200);
	//  if (cor13 + mainLight < cor15 - 2) leds[cor13 + mainLight + 2] = CHSV(hue + 2 * 2, 255, 140);
	//  if (cor13 + mainLight < cor15 - 3) leds[cor13 + mainLight + 3] = CHSV(hue + 3 * 2, 255, 90);
	//  if (cor13 + mainLight < cor15 - 4) leds[cor13 + mainLight + 4] = CHSV(hue + 4 * 2, 255, 65);

	//  if (cor1 - mainLight > 0) {
	//    leds[cor1 - mainLight - 1] = CHSV(hue, 255, 255);  // As long as the light, starting at cor3 and moving right, is still infront of the bottom right corner, we're good
	//    if (cor1 - mainLight > 1) {
	//      leds[cor1 - mainLight - 2] = CHSV(hue, 0, 20);  // Light up the LED in front of the dot if there is room (aka it hasn't hit the end yet)
	//    }
	//  }
	//  else leds[cor28 - mainLight + 26] = CHSV(hue, 255, 255);
	//  if (cor13 + mainLight == cor15 - 1) {
	//    topRightVar = 0;
	//    mainLight = 0;
	//    topRightColor = hue;
	//    z = 2;
	//  }
	//  break;
	//case 2: // Move right on top and down on left
	//  leds[cor15 - 1 - mainLight] = CHSV(hue, 255, 255);
	//  if (cor15 - 1 - mainLight > cor13) leds[cor15 - 2 - mainLight] = CHSV(hue + 1 * 2, 255, 200);
	//  if (cor15 - 1 - mainLight > cor13 + 1) leds[cor15 - 3 - mainLight] = CHSV(hue + 2 * 2, 255, 140);
	//  if (cor15 - 1 - mainLight > cor13 + 2) leds[cor15 - 4 - mainLight] = CHSV(hue + 3 * 2, 255, 90);
	//  if (cor15 - 1 - mainLight > cor13 + 3) leds[cor15 - 5 - mainLight] = CHSV(hue + 4 * 2, 255, 65);  // Don't know how to fix it
	//  if (cor27 + mainLight < NUM_LEDS) leds[cor27 + mainLight] = CHSV(hue, 255, 255);
	//  else leds[cor0 + mainLight - 24] = CHSV(hue, 255, 255);
	//  if (cor15 - 1 - mainLight == cor13) {
	//    botLeftVar = 0;
	//    mainLight = 0;
	//    botLeftColor = hue;
	//    z = 1;
	//  }
	//  break;
	//}

	//bouncingZackSmallSquares(botLeftColor, topRightColor);

	//hue++;
	//speed1 = constrain(speed1 + friction + .01*speed1, 0, 200);
	//var = map(speed1, 0, 200, 244, 250);
	//fadeBlue(250);
	//fadeRange(254, cor1, cor13); // Bottom right square fade
	//fadeRange(254, cor15, cor27); // Top left sqaure fade
	//LEDS.show();
}

void bouncingZack_withoutFade() {
	double friction = .3;

	if (beatDetected[2] == 2) {
		//if (beatDetectRed(spectrumValue[2], spectrumValueOld[2], spectrumValueOldOld[2]) == 1) {
		speed1 -= .85*speed1;
	}

	if (time1 > speed1) { // If enough time has passed (time1), then move the light
		mainLight++;
		topRightVar++;
		botLeftVar++;
		time1 = 0;
	}
	switch (z) {
	case 1: // Move left on bottom and up on right
		leds[cor13 + mainLight] = CHSV(hue, 255, 255);
		if (cor13 + mainLight < cor15 - 1) leds[cor13 + mainLight + 1] = CHSV(hue + 1 * 2, 255, 200);
		if (cor13 + mainLight < cor15 - 2) leds[cor13 + mainLight + 2] = CHSV(hue + 2 * 2, 255, 140);
		if (cor13 + mainLight < cor15 - 3) leds[cor13 + mainLight + 3] = CHSV(hue + 3 * 2, 255, 90);
		if (cor13 + mainLight < cor15 - 4) leds[cor13 + mainLight + 4] = CHSV(hue + 4 * 2, 255, 65);

		if (cor1 - mainLight > 0) {
			leds[cor1 - mainLight - 1] = CHSV(hue, 255, 255);  // As long as the light, starting at cor3 and moving right, is still infront of the bottom right corner, we're good
			if (cor1 - mainLight > 1) {
				leds[cor1 - mainLight - 2] = CHSV(hue, 0, 20);  // Light up the LED in front of the dot if there is room (aka it hasn't hit the end yet)
			}
		}
		else leds[cor28 - mainLight + 26] = CHSV(hue, 255, 255);
		if (cor13 + mainLight == cor15 - 1) {
			topRightVar = 0;
			mainLight = 0;
			topRightColor = hue;
			z = 2;
		}
		break;
	case 2: // Move right on top and down on left
		leds[cor15 - 1 - mainLight] = CHSV(hue, 255, 255);
		if (cor15 - 1 - mainLight > cor13) leds[cor15 - 2 - mainLight] = CHSV(hue + 1 * 2, 255, 200);
		if (cor15 - 1 - mainLight > cor13 + 1) leds[cor15 - 3 - mainLight] = CHSV(hue + 2 * 2, 255, 140);
		if (cor15 - 1 - mainLight > cor13 + 2) leds[cor15 - 4 - mainLight] = CHSV(hue + 3 * 2, 255, 90);
		if (cor15 - 1 - mainLight > cor13 + 3) leds[cor15 - 5 - mainLight] = CHSV(hue + 4 * 2, 255, 65);  // Don't know how to fix it
		if (cor27 + mainLight < NUM_LEDS) leds[cor27 + mainLight] = CHSV(hue, 255, 255);
		else leds[cor0 + mainLight - 24] = CHSV(hue, 255, 255);
		if (cor15 - 1 - mainLight == cor13) {
			botLeftVar = 0;
			mainLight = 0;
			botLeftColor = hue;
			z = 1;
		}
		break;
	}

	bouncingZackSmallSquares(botLeftColor, topRightColor);

	hue++;
	speed1 = constrain(speed1 + friction + .01*speed1, 0, 200);
	var = map(speed1, 0, 200, 244, 250);
	fadeBlue(250);
	fadeRange(255, cor1, cor13); // Bottom right square fade
	fadeRange(255, cor15, cor27); // Top left sqaure fade
	LEDS.show();
}

void bouncingZackSmallSquares(int botLeftColor, int topRightColor) {
	if (topRightVar < 10) {
		leds[cor23 + topRightVar] = CHSV(topRightColor, 255, 255);
		leds[cor25 - topRightVar - 1] = CHSV(topRightColor, 255, 255);
		leds[cor21 + topRightVar] = CHSV(topRightColor, 255, 255);
		leds[cor23 - topRightVar - 1] = CHSV(topRightColor, 255, 255);
	}
	if (topRightVar < 26) {
		leds[cor19 - topRightVar - 1] = CHSV(topRightColor, 255, 255);
		leds[cor15 + topRightVar] = CHSV(topRightColor, 255, 255);
	}
	if (botLeftVar < 10) {
		leds[cor9 - botLeftVar - 1] = CHSV(botLeftColor, 255, 255);
		leds[cor7 + botLeftVar] = CHSV(botLeftColor, 255, 255);
		leds[cor1 + botLeftVar] = CHSV(botLeftColor, 255, 255);
		leds[cor3 - botLeftVar - 1] = CHSV(botLeftColor, 255, 255);
	}
	if (botLeftVar < 26) {
		leds[cor9 + botLeftVar] = CHSV(botLeftColor, 255, 255);
		leds[cor13 - botLeftVar] = CHSV(botLeftColor, 255, 255);
	}
}

void fadeRed(int fadeVar) {                           // THESE HAVE NOTHING TO DO WITH THE COLORS. red = bottom square, blue = big square, green = top square
	for (int i = cor9; i < cor13; i++) leds[i].nscale8(fadeVar);
	for (int i = cor7; i < cor9; i++) leds[i].nscale8(fadeVar);
}

void fadeGreen(int fadeVar) {
	for (int i = cor15; i < cor19; i++) leds[i].nscale8(fadeVar);
	for (int i = cor23; i < cor25; i++) leds[i].nscale8(fadeVar);
}

void fadeBlue(int fadeVar) {
	for (int i = cor0; i < cor1; i++) leds[i].nscale8(fadeVar);
	for (int i = cor5; i < cor7; i++) leds[i].nscale8(fadeVar);
	for (int i = cor13; i < cor15; i++) leds[i].nscale8(fadeVar);
	for (int i = cor25; i < cor28; i++) leds[i].nscale8(fadeVar);
}

void fadeRange(int fadeVar, int start, int stop) {
	for (int i = start; i < stop; i++) leds[i].nscale8(fadeVar);
}

void findSide(int part, int &start, int &end) {
	switch (part) {
	case 1:
		start = cor2;
		end = cor3;
		break;
	case 2:
		start = cor3;
		end = cor4;
		break;
	case 3:
		start = cor4;
		end = cor5;
		break;
	case 4:
		start = cor5;
		end = cor6;
		break;
	case 5:
		start = cor6;
		end = cor7;
		break;
	case 6:
		start = cor7;
		end = cor8;
		break;
	case 7:
		start = cor8;
		end = cor9;
		break;
	case 8:
		start = cor9;
		end = cor10;
		break;
	case 9:
		start = cor10;
		end = cor11;
		break;
	case 10:
		start = cor11;
		end = cor12;
		break;
	case 11:
		start = cor12;
		end = cor13;
		break;
	case 12:
		start = cor13;
		end = cor14;
		break;
	case 13:
		start = cor14;
		end = cor15;
		break;
	case 14:
		start = cor15;
		end = cor16;
		break;
	case 15:
		start = cor16;
		end = cor17;
		break;
	case 16:
		start = cor17;
		end = cor18;
		break;
	case 17:
		start = cor18;
		end = cor19;
		break;
	case 18:
		start = cor19;
		end = cor20;
		break;
	case 19:
		start = cor20;
		end = cor21;
		break;
	case 20:
		start = cor21;
		end = cor22 - 1;
		break;
	}
}

int beatDetect(int freq) {
	int returnThis = 0;
	if (freq == 2) {
		if (spectrumValueOld[freq] > 1.3 * spectrumValueOldOld[freq] && spectrumValueOld[freq] > spectrumValue[freq] * .8 && spectrumValueOld[freq] > lowThresh) {
			//return 1;
			returnThis = 1;
		}
	}
	else if (freq == 8) {
		if (spectrumValue[freq] > 1.4 * spectrumValueOld[freq] && spectrumValue[freq] > midThresh) {
			returnThis = 1;
			//return 1;
		}
	}
	else if (freq == 12) {
		if (spectrumValueOld[freq] > 1.3 * spectrumValueOldOld[freq] && spectrumValueOld[freq] > spectrumValue[freq] * .8 && spectrumValueOld[freq] > highThresh - 100) {
			returnThis = 1;
			//return 1;
		}
	}
	else {
		if (spectrumValue[freq] > 1.4 * spectrumValueOld[freq] && spectrumValueOld[freq] > spectrumValue[freq] * .8 && spectrumValue[freq] > midThresh) {
			return 1;
		}
	}
	return returnThis;
}

void fadeAll(int fadeVar) {
	for (int i = 0; i < NUM_LEDS; i++) leds[i].nscale8(fadeVar);
}

void verticalBars() {
	fadeAll(246);

	//RED
	newLightBar(spectrumValue[2], cor11, cor12, 0);
	//ORANGE
	newLightBar(spectrumValue[4], cor6, cor7, 23);
	newLightBar(spectrumValue[4], cor8, cor9, 23);
	newLightBar(spectrumValue[4], cor2, cor3, 23);
	newLightBar(spectrumValue[4], cor4, cor5, 23);
	//YELLOW
	newLightBar(spectrumValue[7], cor0, cor1, 54);
	//GREEN
	newLightBar(spectrumValue[7], cor14, cor15, 96);
	//BLUE
	newLightBar(spectrumValue[10], cor22, cor23, 160);
	newLightBar(spectrumValue[10], cor26, cor27, 160);
	newLightBar(spectrumValue[10], cor20, cor21, 160);
	newLightBar(spectrumValue[10], cor24, cor25, 160);
	//PURPLE
	newLightBar(spectrumValue[12], cor17, cor18, 209);

	LEDS.show();
}

void newLightBar(int spectrumValue, int startCorner, int endCorner, int hue) {
	int length = endCorner - startCorner;
	color = constrain(map(spectrumValue, 0, 500, 0, length), 0, length);
	for (int i = 0; i < color; i++) {
		leds[startCorner + i] = CHSV(hue, 255, 255);
		if ((startCorner - 1 - i) < 0) leds[cor28 - i] = CHSV(hue, 255, 255);
		else leds[startCorner - 1 - i] = CHSV(hue, 255, 255);
	}
	for (int j = 0; j < 4; j++) {
		if ((startCorner + 1 - j) < 0) leds[cor28 - j + 1] = CHSV(hue, 255, 255);
		else leds[startCorner + 1 - j] = CHSV(hue, 255, 255);
	}

}

void lightSide(int part, int h, int s, int v, int beg_offset, int end_offset) {
	switch (part) {
	case 1:
		start = cor0 + beg_offset;
		end = cor1 - 1 - end_offset;
		break;
	case 2:
		start = cor1 + beg_offset;
		end = cor2 - 1 - end_offset;
		break;
	case 3:
		start = cor2 + beg_offset;
		end = cor3 - 1 - end_offset;
		break;
	case 4:
		start = cor3 + beg_offset;
		end = cor4 - 1 - end_offset;
		break;
	case 5:
		start = cor4 + beg_offset;
		end = cor5 - 1 - end_offset;
		break;
	case 6:
		start = cor5 + beg_offset;
		end = cor6 - 1 - end_offset;
		break;
	case 7:
		start = cor6 - 1 + beg_offset;
		end = cor7 - end_offset;
		break;
	case 8:
		start = cor7 - 0 + beg_offset;
		end = cor8 - end_offset;
		break;
	case 9:
		start = cor8 + beg_offset;
		end = cor9 - 1 - end_offset;
		break;
	case 10:
		start = cor9 + beg_offset;
		end = cor10 - 1 - end_offset;
		break;
	case 11:
		start = cor10 + beg_offset;
		end = cor11 - 1 - end_offset;
		break;
	case 12:
		start = cor11 + beg_offset;
		end = cor12 - 1 - end_offset;
		break;
	case 13:
		start = cor12 + beg_offset;
		end = cor13 - 1 - end_offset;
		break;
	case 14:
		start = cor13 + beg_offset;
		end = cor14 - 1 - end_offset;
		break;
	case 15:
		start = cor14 + beg_offset;
		end = cor15 - 1 - end_offset;
		break;
	case 16:
		start = cor15 + beg_offset;
		end = cor16 - 1 - end_offset;
		break;
	case 17:
		start = cor16 + beg_offset;
		end = cor17 - 1 - end_offset;
		break;
	case 18:
		start = cor17 + beg_offset;
		end = cor18 - 1 - end_offset;
		break;
	case 19:
		start = cor18 + beg_offset;
		end = cor19 - 1 - end_offset;
		break;
	case 20:
		start = cor19 + beg_offset;
		end = cor20 - 1 - end_offset;
		break;
	case 21:
		start = cor20 + beg_offset;
		end = cor21 - 1 - end_offset;
		break;
	case 22:
		start = cor21 + beg_offset;
		end = cor22 - 1 - end_offset;
		break;
	case 23:
		start = cor22 + beg_offset;
		end = cor23 - 1 - end_offset;
		break;
	case 24:
		start = cor23 + beg_offset;
		end = cor24 - 1 - end_offset;
		break;
	case 25:
		start = cor24 + beg_offset;
		end = cor25 - 1 - end_offset;
		break;
	case 26:
		start = cor25 + beg_offset;
		end = cor26 - 1 - end_offset;
		break;
	case 27:
		start = cor26 + beg_offset;
		end = cor27 - 1 - end_offset;
		break;
	case 28:
		start = cor27 + beg_offset;
		end = cor28 - 1 - end_offset;
		break;
	}

	for (int x = start; x <= end; x++) {
		leds[x] = CHSV(h, s, v);
	}
}

void coordinates() {

	////// SWEEPING UP TO THE RIGHT
	if (coordMode == 0) {
		eq = 0;
		slope = -1;
		yint = -50 + var;
		var++;
		if (yint == 50) {
			var = 0;
			coordMode = random8(0, 8);
		}
	}

	////// SWEEPING DOWN TO THE LEFT
	if (coordMode == 1) {
		eq = 0;
		slope = -1;
		yint = 50 - var;
		var++;
		if (yint == -50) {
			var = 0;
			coordMode = random8(0, 8);
		}
	}

	////// SWEEPING UP FROM THE BOTTOM
	if (coordMode == 2) {
		eq = 0;
		slope = 0;
		yint = -28 + var;
		var++;
		delay(8);
		if (yint == 28) {
			var = 0;
			coordMode = random8(0, 8);
		}
	}

	////// SWEEPING DOWN FROM THE TOP
	if (coordMode == 3) {
		eq = 0;
		slope = 0;
		yint = 28 - var;
		var++;
		delay(8);
		if (yint == -28) {
			var = 0;
			coordMode = random8(0, 8);
		}
	}

	////// SWEEPING LEFT TO RIGHT
	if (coordMode == 4) {
		eq = 1;
		slope = 0;
		yint = -28 + var;
		var++;
		delay(8);
		if (yint == 28) {
			var = 0;
			coordMode = random8(0, 8);
		}
	}

	////// SWEEPING RIGHT TO LEFT
	if (coordMode == 5) {
		eq = 1;
		slope = 0;
		yint = 28 - var;
		var++;
		delay(8);
		if (yint == -28) {
			var = 0;
			coordMode = random8(0, 8);
		}
	}

	////// SWEEPING DOWN TO THE RIGHT
	if (coordMode == 6) {
		eq = 0;
		slope = 1;
		yint = -50 + var;
		var++;
		if (yint == 50) {
			var = 0;
			coordMode = random8(0, 8);
		}
	}

	////// SWEEPING UP TO THE LEFT
	if (coordMode == 7) {
		eq = 0;
		slope = 1;
		yint = 50 - var;
		var++;
		if (yint == -50) {
			var = 0;
			coordMode = random8(0, 8);
		}
	}

	hue++;

	if (eq == 0) {  // n = normal
		for (int i = 0; i < NUM_LEDS; i++) {
			if (slope * ledx[i] + yint - ledy[i] < 1 && slope *ledx[i] + yint - ledy[i] > -1)  leds[i] = CHSV(hue, 255, 255);
		}
	}
	if (eq == 1) {  // r = rotated
		for (int i = 0; i < NUM_LEDS; i++) {
			if (slope * ledy[i] + yint - ledx[i] < 1 && slope *ledy[i] + yint - ledx[i] > -1)  leds[i] = CHSV(hue, 255, 255);
		}
	}
	LEDS.show();
}

void rainbowBeat() {
	//a = analogRead(8, 0, 1023, 0, 10);
	a = 2;

	counter++;  // This exists to make the animation slower. hue only gets incremented every 3 goes arounds
	if (counter == 6) {
		hue++;
		counter = 0;
	}
	fill_rainbow(leds, NUM_LEDS, hue, 5);

	if (beatDetected[2] == 2) {
		r = 0;
	}
	if (beatDetected[8] == 2) {
		b = 0;
	}
	if (beatDetected[12] == 2) {
		g = 0;
	}

	if (r < 255) r = constrain(r + a, 0, 180);
	if (g < 255) g = constrain(g + a, 0, 170);
	if (b < 255) b = constrain(b + a, 0, 170);

	for (int i = 0; i < NUM_LEDS; i++) leds[i] -= CRGB(r, g, b);

	LEDS.show();
}

void coordinatesToBeat() {

	if (beatDetected[2] == 2) {
		coordMode = random8(0, 8);
		var = 0;
	}

	//brightness = 255;
	eq = 1;

	switch (coordMode) {
	case 0:     ////// SWEEPING UP TO THE RIGHT
		slope = -1;
		yint = -52 + var;
		var++;
		break;
	case 1:      ////// SWEEPING DOWN TO THE LEFT
		slope = -1;
		yint = 53 - var;
		var++;
		break;
	case 2:      ////// SWEEPING UP FROM THE BOTTOM
		slope = 0;
		yint = -28 + var;
		var++;
		delay(8);
		break;
	case 3:       ////// SWEEPING DOWN FROM THE TOP
		slope = 0;
		yint = 28 - var;
		var++;
		delay(8);
		break;
	case 4:      ////// SWEEPING LEFT TO RIGHT
		eq = 2;
		slope = 0;
		yint = -28 + var;
		var++;
		delay(8);
		break;
	case 5:      ////// SWEEPING RIGHT TO LEFT
		eq = 2;
		slope = 0;
		yint = 28 - var;
		var++;
		delay(8);
		break;
	case 6:   ////// SWEEPING DOWN TO THE RIGHT
		slope = 1;
		yint = 50 + var;
		var--;
		break;
	case 7:      ////// SWEEPING UP TO THE LEFT
		slope = 1;
		yint = 50 - var;
		var++;
		break;
	}

	hue++;

	if (eq == 1) {  // 1 = normal
		for (int i = 0; i < NUM_LEDS; i++) {
			//bool isLEDOnLine = slope * ledx[i] + yint - ledy[i] < 1 && slope *ledx[i] + yint - ledy[i] > -1
			if (slope * ledx[i] + yint - ledy[i] < 1 && slope *ledx[i] + yint - ledy[i] > -1) leds[i] = CHSV(hue, 255, 255);
		}
	}
	if (eq == 2) {  // 2 = rotated
		for (int i = 0; i < NUM_LEDS; i++) {
			if (slope * ledy[i] + yint - ledx[i] < 1 && slope *ledy[i] + yint - ledx[i] > -1) leds[i] = CHSV(hue, 255, 255);
		}
	}

	leds[0] = leds[1];
	LEDS.show();
}

void coordinatesToBeatExperimental() {
	fadeAll(254);

	coordModeTimer++;
	if (coordModeTimer > 1000) {
		coordModeMain = random8(1, 6);
		coordModeTimer = 0;
	}

	if (beatDetected[2] == 2) {
		pingPong = !pingPong;
		switch (coordModeMain) {
		default:
			coordMode = random8(1, 9);
			break;
		case 1:
			if (pingPong) coordMode = 1;
			else coordMode = 2;
			break;
		case 2:
			if (pingPong) coordMode = 3;
			else coordMode = 4;
			break;
		case 3:
			if (pingPong) coordMode = 5;
			else coordMode = 6;
			break;
		case 4:
			if (pingPong) coordMode = 7;
			else coordMode = 8;
			break;
		case 5:
			coordMode = random8(1, 9);
			break;
		}
		//coordMode = random8(1, 9);
		speed -= .85*speed;
		var = 0;
	}

	if (time > speed) {
		var++;
		time = 0;
	}

	switch (coordMode) {
	case 1:                ////// SWEEPING UP TO THE RIGHT
		slope = -1;
		yint = -35 + var;
		if (var > 70) sweepStatus = true;
		else sweepStatus = false;
		break;
	case 2:              ////// SWEEPING DOWN TO THE LEFT
		slope = -1;
		yint = 35 - var;
		if (var > 70) sweepStatus = true;
		else sweepStatus = false;
		break;
	case 3:              ////// SWEEPING UP FROM THE BOTTOM
		slope = 0;
		yint = -27 + var;
		delay(8);            /// Delay to make it take about the same time to sweep up/down as it takes to go diagonal
		if (var > 52) sweepStatus = true;
		else sweepStatus = false;
		break;
	case 4:              ////// SWEEPING DOWN FROM THE TOP
		slope = 0;
		yint = 27 - var;
		delay(8);
		if (var > 52) sweepStatus = true;
		else sweepStatus = false;
		break;
	case 5:              ////// SWEEPING LEFT TO RIGHT
		slope = 0;
		yint = -26 + var;
		delay(8);
		if (var > 52) sweepStatus = true;
		else sweepStatus = false;
		break;
	case 6:       ////// SWEEPING RIGHT TO LEFT
		slope = 0;
		yint = 26 - var;
		delay(8);
		if (var > 52) sweepStatus = true;
		else sweepStatus = false;
		break;
	case 7:     ////// SWEEPING UP TO THE LEFT
		slope = 1;
		yint = -52 + var;
		if (var > 104) sweepStatus = true;
		else sweepStatus = false;
		break;
	case 8:     ////// SWEEPING DOWN TO THE RIGHT
		slope = 1;
		yint = 52 - var;
		if (var > 104) sweepStatus = true;
		else sweepStatus = false;
		break;
	}

	hue++;

	if (coordMode == 3 || coordMode == 4) {  // 2 = rotated
		for (int i = 0; i < NUM_LEDS; i++) {
			if (slope * ledy[i] + yint - ledx[i] < 1 && slope *ledy[i] + yint - ledx[i] > -1) leds[i] = CHSV(hue, 255, 255);
		}
	}
	else {  // 1 = normal
		for (int i = 0; i < NUM_LEDS; i++) {
			//bool isLEDOnLine = slope * ledx[i] + yint - ledy[i] < 1 && slope *ledx[i] + yint - ledy[i] > -1
			if (slope * ledx[i] + yint - ledy[i] < 1 && slope *ledx[i] + yint - ledy[i] > -1) leds[i] = CHSV(hue, 255, 255);
		}
	}

	counter++;  // This exists to make the animation slower. hue only gets incremented every 3 goes arounds
	if (counter == 6) {
		hue++;
		counter = 0;
	}
	//if (var > 100 || var < -100) {
	if (sweepStatus) {
		for (int i = 0; i < 1; i++) {
			int pos = random16(NUM_LEDS);
			leds[pos] = CHSV(96, 255, random8(10, 100)); // green 96
		}
		for (int i = 0; i < spectrumValue[12] / 5; i++) {  // For as loud as the high frequency is, add that many sparkles to top right
			int pos = random16(NUM_LEDS);
			//brightness = map(spectrumValue[12], 0, 14, 0, 255);
			leds[pos] = CHSV(hue, random8(200, 255), random8(30, 130)); // blue 160
		}
	}

	speed = constrain(speed + .1 + .01*speed, 0, 200);
	leds[0] = leds[1];  // led[0] was not responding, not sure why
	LEDS.show();
}

void displayCorners() {
	leds[cor1] = CHSV(0, 255, 255);
	leds[cor2] = CHSV(0, 255, 255);
	leds[cor3] = CHSV(0, 255, 255);
	leds[cor4] = CHSV(0, 255, 255);
	leds[cor5] = CHSV(0, 255, 255);
	leds[cor6] = CHSV(0, 255, 255);
	leds[cor7] = CHSV(0, 255, 255);
	leds[cor8] = CHSV(0, 255, 255);
	leds[cor9] = CHSV(0, 255, 255);
	leds[cor10] = CHSV(0, 255, 255);
	leds[cor11] = CHSV(0, 255, 255);
	leds[cor12] = CHSV(0, 255, 255);
	leds[cor13] = CHSV(0, 255, 255);
	leds[cor14] = CHSV(0, 255, 255);
	leds[cor15] = CHSV(0, 255, 255);
	leds[cor16] = CHSV(0, 255, 255);
	leds[cor17] = CHSV(0, 255, 255);
	leds[cor18] = CHSV(0, 255, 255);
	leds[cor19] = CHSV(0, 255, 255);
	leds[cor20] = CHSV(0, 255, 255);
	leds[cor21] = CHSV(0, 255, 255);
	leds[cor22] = CHSV(0, 255, 255);
	leds[cor23] = CHSV(0, 255, 255);
	leds[cor24] = CHSV(0, 255, 255);
	leds[cor25] = CHSV(0, 255, 255);
	leds[cor26] = CHSV(0, 255, 255);
	leds[cor27] = CHSV(0, 255, 255);
	leds[cor28] = CHSV(0, 255, 255);

	LEDS.show();
}

void dynamicFading() {
	getSpectrum(spectrumValue, spectrumValueOld, spectrumValueOldOld);
	//val = map(analogRead(8), 0, 1023, 0, 100);
	//LEDS.setBrightness(val);

	if (beatDetect(8)) {
		lightSide(1, 160, 255, 255, 0, 0);
		lightSide(4, 160, 255, 255, 0, 0);
		lightSide(5, 160, 255, 255, 0, 0);
		lightSide(10, 160, 255, 255, 0, 0);
		lightSide(11, 160, 255, 255, 0, 0);
		lightSide(17, 160, 255, 255, 0, 0);
		lightSide(16, 160, 255, 255, 0, 0);
		lightSide(20, 160, 255, 255, 0, 0);
		Bcounter = Bcounter - 15;
	}

	if (beatDetect(2)) {
		lightSide(2, 0, 255, 255, 0, 0);
		lightSide(3, 0, 255, 255, 0, 0);
		lightSide(6, 0, 255, 255, 0, 0);
		lightSide(7, 0, 255, 255, 0, 0);
		lightSide(8, 0, 255, 255, 0, 0);
		lightSide(9, 0, 255, 255, 0, 0);
		Rcounter = Rcounter - 15;
	}

	//if (beatDetect(12)) {
	if (spectrumValueOld[12] > 1.7 * spectrumValueOldOld[12] && spectrumValueOld[12] > spectrumValue[12] * .8 && spectrumValueOld[12] > highThresh) {
		lightSide(14, 96, 255, 255, 0, 0);
		lightSide(18, 96, 255, 255, 0, 0);
		lightSide(12, 96, 255, 255, 0, 0);
		lightSide(13, 96, 255, 255, 0, 0);
		lightSide(14, 96, 255, 255, 0, 0);
		lightSide(15, 96, 255, 255, 0, 0);
		Gcounter = Gcounter - 15;
	}

	Rcounter = constrain(Rcounter, 0, 100);
	Gcounter = constrain(Gcounter, 0, 100);
	Bcounter = constrain(Bcounter, 0, 100);
	redFadeVar = map(constrain(Rcounter, 0, 100), 1, 100, 180, 253);
	greenFadeVar = map(constrain(Gcounter, 0, 100), 1, 100, 180, 253);
	blueFadeVar = map(constrain(Bcounter, 0, 100), 1, 100, 180, 253);
	if (Bcounter < 100) Bcounter++;
	if (Rcounter < 100) Rcounter++;
	if (Gcounter < 100) Gcounter++;
	fadeRed(redFadeVar);
	fadeBlue(blueFadeVar);
	fadeGreen(greenFadeVar);

	LEDS.show();
}

void travelingDotsToLowBeats() {
	if (beatDetected[2] == 2) {
		bottomDotSpeed -= .85*bottomDotSpeed;
		topDotSpeed -= .85*topDotSpeed;
		travelingDotBigSquareDesigns(1, 0);
	}

	travelingBottomDot(bottomDotSpeed);
	travelingTopDot(topDotSpeed);

	bottomDotSpeed = constrain(bottomDotSpeed + .2 + .01*bottomDotSpeed, 0, 200);
	topDotSpeed = constrain(topDotSpeed + .2 + .01*topDotSpeed, 0, 200);

	fadeAll(250);
	FastLED.show();
}

void travelingDotsToHighAndLow() {
	if (beatDetected[2] == 2) {
		bottomDotSpeed -= .85*bottomDotSpeed;
		travelingDotBigSquareDesigns(3, 0);
	}
	if (beatDetected[12] == 2) {
		topDotSpeed -= .85*topDotSpeed;
		travelingDotBigSquareDesigns(3, 1);
	}
	travelingBottomDot(bottomDotSpeed);
	travelingTopDot(topDotSpeed);

	bottomDotSpeed = constrain(bottomDotSpeed + .2 + .01*bottomDotSpeed, 0, 200);
	topDotSpeed = constrain(topDotSpeed + .2 + .01*topDotSpeed, 0, 200);

	fadeAll(250);
	FastLED.show();
}

void travelingDots(int mode) {

	switch (mode) {
	default:
		mode = 1;
		break;
	case 1:
		if (beatDetected[2] == 2) {
			bottomDotSpeed -= .85*bottomDotSpeed;
			topDotSpeed -= .85*topDotSpeed;
			travelingDotBigSquareDesigns(1, 0);
		}
		break;
	case 2:
		if (beatDetected[2] == 2) {
			bottomDotSpeed -= .85*bottomDotSpeed;
			topDotSpeed -= .85*topDotSpeed;
			travelingDotBigSquareDesigns(2, 0);
		}
		break;
	case 3:
		if (beatDetected[2] == 2) {
			bottomDotSpeed -= .85*bottomDotSpeed;
			travelingDotBigSquareDesigns(3, 0);
		}
		if (beatDetected[12] == 2) {
			topDotSpeed -= .85*topDotSpeed;
			travelingDotBigSquareDesigns(3, 1);
		}
		break;
	}

	travelingBottomDot(bottomDotSpeed);
	travelingTopDot(topDotSpeed);

	bottomDotSpeed = constrain(bottomDotSpeed + .2 + .01*bottomDotSpeed, 0, 200);
	topDotSpeed = constrain(topDotSpeed + .2 + .01*topDotSpeed, 0, 200);

	fadeAll(250);
	FastLED.show();
}

void travelingDotBigSquareDesigns(int mode, int option) {
	switch (mode) {
	case 1:
		for (int i = 0; i < 25; i++) {                    // Creates white lights on corners of the big square
			leds[cor0 + i] = CHSV(0, 0, 255 - (i * 10));
			leds[cor14 + i] = CHSV(0, 0, 255 - i * 10);
			leds[cor28 - i] = CHSV(0, 0, 255 - i * 10);
			leds[cor14 - i] = CHSV(0, 0, 255 - i * 10);
			lightSide(2, 0, 0, 255, 0, 0);
			lightSide(3, 0, 0, 255, 0, 0);
			lightSide(22, 0, 0, 255, 0, 0);
			lightSide(23, 0, 0, 255, 0, 0);
		}
		break;
	case 2:
		for (int i = 0; i < 16; i++) {                    // White lights at ceter of side of big square
			leds[cor28 - 18 + i] = CHSV(0, 0, 255 - i * 13);
			leds[cor0 + 18 - i] = CHSV(0, 0, 255 - i * 13);
			leds[cor14 - 18 + i] = CHSV(0, 0, 255 - i * 13);
			leds[cor14 + 18 - i] = CHSV(0, 0, 255 - i * 13);
			if (i < 8) {                    // If ..., then extend white lights towards small squares, but don't go into them
				leds[cor0 + 18 + i] = CHSV(0, 0, 255 - i * 28);
				leds[cor28 - 18 - i] = CHSV(0, 0, 255 - i * 28);
				leds[cor14 - 18 - i] = CHSV(0, 0, 255 - i * 28);
				leds[cor14 + 18 + i] = CHSV(0, 0, 255 - i * 28);
			}
		}
		break;
	case 3:
		for (int i = 0; i < 25; i++) {
			if (option == 0) {
				leds[cor1 - 1 - i] = CHSV(0, 0, 255 - (i * 10));
				leds[cor13 + i] = CHSV(0, 0, 255 - (i * 10));
				lightSide(2, 0, 0, 255, 0, 0);
				lightSide(3, 0, 0, 255, 0, 0);
			}
			if (option == 1) {
				leds[cor15 - i] = CHSV(0, 0, 255 - (i * 10));
				leds[cor27 + i] = CHSV(0, 0, 255 - (i * 10));
				lightSide(22, 0, 0, 255, 0, 0);
				lightSide(23, 0, 0, 255, 0, 0);
			}
		}
		break;
	}
}

//void travelingTopDot(double &topDotSpeed) {
//  if (topDotTime > topDotSpeed) {
//    for (int i = 0; i < 2; i++) {
//      if (topDotZones[i] == 1) {
//        topDots[i]++;
//        if (topDots[i] >= cor17 - 0) {
//          topDots[i] = cor21;
//          topDotZones[i] = 2;
//        }
//      }
//      if (topDotZones[i] == 2) {
//        topDots[i]--;
//        if (topDots[i] <= cor19 - 1) {
//          topDots[i] = cor13;
//          topDotZones[i] = 1;
//        }
//      }
//      leds[topDots[i]] = CRGB::White;
//      //topDotTime = 0;
//    }
//  }
//}


void travelingTopDot(double &topDotSpeed) {
	if (topDotTime > topDotSpeed) {
		for (int i = 0; i < 2; i++) {
			if (topDotZones[i] == 1) {
				topDots[i]++;
				if (topDots[i] >= cor19 && topDots[i] <= cor21) {
					topDots[i] = cor23;
					topDotZones[i] = 2;
				}
			}
			if (topDotZones[i] == 2) {
				topDots[i]++;
				if (topDots[i] >= cor25) {
					topDots[i] = cor15;
					topDotZones[i] = 1;
				}
			}
			leds[topDots[i]] = CRGB::White;
			topDotTime = 0;
		}
	}
}

void travelingBottomDot(double &bottomDotSpeed) {
	if (bottomDotTime > bottomDotSpeed) {
		for (int i = 0; i < 2; i++) {
			bottomDots[i]++;
			if (bottomDots[i] == cor13) bottomDots[i] = cor7;
			leds[bottomDots[i]] = CRGB::White;
			bottomDotTime = 0;
		}
	}
}

void pingPongSquares() {
	fadeAll(253);

	if (beatDetected[2] == 2) {
		//lowFreqSquare != lowFreqSquare;
		color = random(214, 300);
		if (lowFreqSquare == false) { // bottom square
			lowFreqSquare = true;
			lightSide(10, color, 255, 255, 0, 0);
			lightSide(11, color, 255, 255, 0, 0);
			lightSide(12, color, 255, 255, 0, 0);
			lightSide(13, color, 255, 255, 0, 0);
			lightSide(8, color, 255, 255, 0, 0);
			lightSide(9, color, 255, 255, 0, 0);
		}
		else {                   // top square
			lowFreqSquare = false;
			lightSide(16, color, 255, 255, 0, 0);
			lightSide(17, color, 255, 255, 0, 0);
			lightSide(18, color, 255, 255, 0, 0);
			lightSide(19, color, 255, 255, 0, 0);
			lightSide(24, color, 255, 255, 0, 0);
			lightSide(25, color, 255, 255, 0, 0);
		}

	}
	if (beatDetected[12] == 2) {
		color = random(90, 180);
		if (highFreqSquare == true) { // bottom small square, inside and out
			highFreqSquare = false;
			lightSide(14, color, 255, 255, 0, 0);
			lightSide(15, color, 255, 255, 0, 0);
		}
		/*for (int i = 2; i < 5; i++) {
		lightSide(i, color, 255, 255, 0, 0);
		}*/
		else {
			highFreqSquare = true;
			lightSide(1, color, 255, 255, 0, 0);
			lightSide(28, color, 255, 255, 0, 0);
			/*for (int i = 24; i < 27; i++) {
			lightSide(i, color, 255, 255, 0, 0);
			}*/
		}
	}

	LEDS.show();
}

void randomSquares() {
	fadeAll(253);

	if (beatDetected[2] == 2) {
		box = random(1, 6);
		color = random(214, 300);
		switch (box) {
		case 1:  // Big square
			lightSide(1, color, 255, 255, 0, 0);
			lightSide(6, color, 255, 255, 0, 0);
			lightSide(7, color, 255, 255, 0, 0);
			lightSide(14, color, 255, 255, 0, 0);
			lightSide(15, color, 255, 255, 0, 0);
			lightSide(26, color, 255, 255, 0, 0);
			lightSide(27, color, 255, 255, 0, 0);
			lightSide(28, color, 255, 255, 0, 0);
			break;
		case 2: // bottom square
			lightSide(10, color, 255, 255, 0, 0);
			lightSide(11, color, 255, 255, 0, 0);
			lightSide(12, color, 255, 255, 0, 0);
			lightSide(13, color, 255, 255, 0, 0);
			lightSide(8, color, 255, 255, 0, 0);
			lightSide(9, color, 255, 255, 0, 0);
			break;
		case 3: // top square
			lightSide(16, color, 255, 255, 0, 0);
			lightSide(17, color, 255, 255, 0, 0);
			lightSide(18, color, 255, 255, 0, 0);
			lightSide(19, color, 255, 255, 0, 0);
			lightSide(24, color, 255, 255, 0, 0);
			lightSide(25, color, 255, 255, 0, 0);
			break;
		case 4: // inside of bottom smallest square
			lightSide(6, color, 255, 255, 0, 0);
			lightSide(7, color, 255, 255, 0, 0);
			lightSide(8, color, 255, 255, 0, 0);
			lightSide(9, color, 255, 255, 0, 0);
			break;
		case 5: // outside of bottom smallest square
			lightSide(2, color, 255, 255, 0, 0);
			lightSide(3, color, 255, 255, 0, 0);
			lightSide(4, color, 255, 255, 0, 0);
			lightSide(5, color, 255, 255, 0, 0);
			break;
		case 6: // inside of top smallest square
			lightSide(24, color, 255, 255, 0, 0);
			lightSide(25, color, 255, 255, 0, 0);
			lightSide(26, color, 255, 255, 0, 0);
			lightSide(27, color, 255, 255, 0, 0);
			break;
		case 7: // outside of top smallest square
			lightSide(20, color, 255, 255, 0, 0);
			lightSide(21, color, 255, 255, 0, 0);
			lightSide(22, color, 255, 255, 0, 0);
			lightSide(23, color, 255, 255, 0, 0);
			break;
		}
	}

	if (beatDetected[12] == 2) {
		box = random(1, 6);
		color = random(90, 180);
		switch (box) {
		case 1:  // Big square
			lightSide(1, color, 255, 255, 0, 0);
			lightSide(6, color, 255, 255, 0, 0);
			lightSide(7, color, 255, 255, 0, 0);
			lightSide(14, color, 255, 255, 0, 0);
			lightSide(15, color, 255, 255, 0, 0);
			lightSide(26, color, 255, 255, 0, 0);
			lightSide(27, color, 255, 255, 0, 0);
			lightSide(28, color, 255, 255, 0, 0);
			break;
		case 2: // bottom square
			lightSide(10, color, 255, 255, 0, 0);
			lightSide(11, color, 255, 255, 0, 0);
			lightSide(12, color, 255, 255, 0, 0);
			lightSide(13, color, 255, 255, 0, 0);
			lightSide(8, color, 255, 255, 0, 0);
			lightSide(9, color, 255, 255, 0, 0);
			break;
		case 3: // top square
			lightSide(16, color, 255, 255, 0, 0);
			lightSide(17, color, 255, 255, 0, 0);
			lightSide(18, color, 255, 255, 0, 0);
			lightSide(19, color, 255, 255, 0, 0);
			lightSide(24, color, 255, 255, 0, 0);
			lightSide(25, color, 255, 255, 0, 0);
			break;
		case 4: // inside of bottom smallest square
			lightSide(6, color, 255, 255, 0, 0);
			lightSide(7, color, 255, 255, 0, 0);
			lightSide(8, color, 255, 255, 0, 0);
			lightSide(9, color, 255, 255, 0, 0);
			break;
		case 5: // outside of bottom smallest square
			lightSide(2, color, 255, 255, 0, 0);
			lightSide(3, color, 255, 255, 0, 0);
			lightSide(4, color, 255, 255, 0, 0);
			lightSide(5, color, 255, 255, 0, 0);
			break;
		case 6: // inside of top smallest square
			lightSide(24, color, 255, 255, 0, 0);
			lightSide(25, color, 255, 255, 0, 0);
			lightSide(26, color, 255, 255, 0, 0);
			lightSide(27, color, 255, 255, 0, 0);
			break;
		case 7: // outside of top smallest square
			lightSide(20, color, 255, 255, 0, 0);
			lightSide(21, color, 255, 255, 0, 0);
			lightSide(22, color, 255, 255, 0, 0);
			lightSide(23, color, 255, 255, 0, 0);
			break;
		}
	}

	FastLED.show();
}

void forTesting() {
	//input testing
	if (button1.released()) {
		Serial.println("released");
	}
	//Serial.print(button1.released());
	/*Serial.print("\t");
	Serial.print(button2.released());
	Serial.print("\t");
	Serial.print(digitalRead(inputSwitch));
	Serial.print("\t");
	Serial.print(digitalRead(reactiveSwitch));
	Serial.print("\t");
	Serial.println(analogRead(brightnessPot));*/

	// Sidse 7 and 8 have weird corrections in the code
	//lightSide(7, 0, 255, 255, 0, 0);
	//lightSide(8, 0, 255, 255, 0, 0);
	//LEDS.show();
}

void confetti() {
	int max_num_of_sparkles = 25;  // old 14
								   //int brightnessMax = 10;
	brightness = 255;

	//for (int i = 0; i < 343; i++) {
	//  if (leds[i].h = colorOne) break;
	//}
	//colorTwo = random8(255);
	//if (yintLow == -8) colorOne = random8(255);

	EVERY_N_SECONDS(10) {                // Every 10 seconds, change the color of the low freq color (1) and high freq color (2)
		colorOne = random8(0, 64);
		colorTwo = random8(128, 192);
	}

	spectrumValue[2] = map(constrain(spectrumValue[2] - 50, 0, 500), 0, 500, 0, max_num_of_sparkles); // " - 50" exists to get rid of low volume noise
	spectrumValue[8] = map(constrain(spectrumValue[8] - 50, 0, 500), 0, 500, 0, max_num_of_sparkles - 4); // "- 4" exists to make less sparkles show
	spectrumValue[12] = map(constrain(spectrumValue[12] - 50, 0, 450), 0, 500, 0, max_num_of_sparkles);

	for (int i = 0; i < spectrumValue[2] * 1.9; i++) {  // For as loud as the low frequency is, add that many sparkles to bottom left
		int pos = random16(cor0, cor14);
		//brightness = map(spectrumValue[2], 0, brightnessMax, 0, 255);
		leds[pos] = CHSV(colorOne, random8(200, 255), brightness);
	}

	for (int i = 0; i < spectrumValue[12] * 1.9; i++) {  // For as loud as the high frequency is, add that many sparkles to top right
		int pos = random16(cor14, cor28);
		//brightness = map(spectrumValue[12], 0, 10, 0, 255);
		leds[pos] = CHSV(colorTwo, random8(200, 255), brightness);
	}

	for (int i = 0; i < spectrumValue[8]; i++) {  // For as loud as the middle frequency is, minus some, add that many green sparkles throughout entire strip
		int pos = random16(NUM_LEDS);
		//brightness = map(spectrumValue[12], 0, 14, 0, 255);
		leds[pos] = CHSV(96, 0, brightness); // white
	}

	for (int i = 0; i < 1; i++) {
		int pos = random16(NUM_LEDS);
		leds[pos] = CHSV(96, 255, random8(70, 120)); // green
	}

	fadeToBlackBy(leds, NUM_LEDS, 1);
	LEDS.show();
}

void sixFrequencyGlitter() {
	brightness = 255;
	//int max_num_of_sparkles = 25;
	//2 4 7 10 12
	spectrumValue[2] = map(constrain(spectrumValue[2] - 50, 0, 500), 0, 500, 0, 25); // " - 50" exists to get rid of low volume noise
	spectrumValue[4] = map(constrain(spectrumValue[4] - 50, 0, 500), 0, 500, 0, 25);
	spectrumValue[7] = map(constrain(spectrumValue[7] - 50, 0, 500), 0, 500, 0, 25);
	spectrumValue[10] = map(constrain(spectrumValue[10] - 50, 0, 450), 0, 500, 0, 25);
	spectrumValue[12] = map(constrain(spectrumValue[12] - 50, 0, 450), 0, 500, 0, 25);

	// 1.9 is an arbitrary number that dictates how mnuch glitter shows up for how loud the frequency is
	for (int i = 0; i < spectrumValue[2] * 1.9; i++) {
		int pos = random16(cor9, cor13);
		//brightness = map(spectrumValue[2], 0, 14, 0, 255);
		leds[pos] = CHSV(0, random8(200, 255), brightness);
	}

	for (int i = 0; i < spectrumValue[4] * 1.9; i++) {
		int pos = random16(cor1, cor9);
		//brightness = map(spectrumValue[4], 0, 14, 0, 255);
		leds[pos] = CHSV(32, random8(200, 255), brightness);
	}

	for (int i = 0; i < spectrumValue[7] * 1.9; i++) {
		int pos = random16(cor13, cor15);
		//brightness = map(spectrumValue[7], 0, 14, 0, 255);
		leds[pos] = CHSV(96, random8(200, 255), brightness);
	}

	for (int i = 0; i < spectrumValue[7] * 1.9; i++) {
		int pos = random16(cor27, cor28);
		//brightness = map(spectrumValue[7], 0, 14, 0, 255);
		leds[pos] = CHSV(64, random8(200, 255), brightness);
	}

	for (int i = 0; i < spectrumValue[7] * 1.9; i++) {
		int pos = random16(0, cor1);
		//brightness = map(spectrumValue[7], 0, 14, 0, 255);
		leds[pos] = CHSV(64, random8(200, 255), brightness);
	}

	for (int i = 0; i < spectrumValue[10] * 1.9; i++) {
		int pos = random16(cor19, cor27);
		//brightness = map(spectrumValue[10], 0, 14, 0, 255);
		leds[pos] = CHSV(160, random8(200, 255), brightness);
	}

	for (int i = 0; i < spectrumValue[12] * 1.9; i++) {
		int pos = random16(cor15, cor19);
		//brightness = map(spectrumValue[12], 0, 14, 0, 255);
		leds[pos] = CHSV(209, random8(200, 255), brightness);
	}

	fadeAll(252);
	//fadeToBlackBy(leds, NUM_LEDS, 1);
	LEDS.show();
}

// Couldn't get this function to work, currently not used
void sparkle(int frequency, int start, int stop, int hue, int sat, int vol, int max) {
	for (int i = 0; i < frequency; i++) {  // For as loud as the low frequency is, 
		int pos = random8(start, stop);
		if (vol == 0) vol = map(frequency, 0, max, 0, 255);
		leds[pos] = CHSV(hue, sat, vol);
	}
}

void ambient_rainbowFade() {
	speed = map(analogRead(A1), 0, 1023, 0, 200);
	if (time > speed) {
		hue++;
		time = 0;
	}

	for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(hue, 255, 255);

	LEDS.show();
}

void ambient_solidColor() {
	hue = map(analogRead(A1), 0, 1023, 0, 255);

	for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(hue, 255, 255);
	LEDS.show();
}

void ambient_confetti() {
	int intensity = map(analogRead(A1), 0, 1023, 0, 50);

	for (int i = 0; i < intensity; i++) {
		int pos = random16(NUM_LEDS);
		hue = random8(255);        // If switch is in position 1, each sparkle will be a random color
		leds[pos] = CHSV(hue, 255, random8(150, 255));
	}

	fadeAll(253);
	LEDS.show();
}

//int pennyPress(int penny) {
//  int releasedThreshold = 2200;
//  int touchedThreshold = 2500;
//  double pennyDelayTime = 400;
//
//  pennyNew = (penny == 1) ? penny1.capacitiveSensor(50) : penny2.capacitiveSensor(50);
//
//  bool isGreaterThanDebounce = pennyDelay > pennyDelayTime;       // True if time passed is greater than the debounce time
//  bool thresholdReached = pennyOld < releasedThreshold;     // True if last reading is below threshold (not touched)
//  bool touched = pennyNew > touchedThreshold;           // True if new reading is greater than threshold (touched)
//
//  if (isGreaterThanDebounce && thresholdReached && touched) {      // If there is a jump in value greater than 1000
//    pennyCounter++;            // Increment touch counter
//    pennyDelay = 0;                    // Initialize debounce timer at 0
//    return 1;
//  }
//  else {
//    return 0;
//  }
//  // DEBUGGING INFO
//  //Serial.print(pennyNew);
//  //Serial.print("\t");
//  //Serial.println(pennyCounter);
//
//  pennyOld = pennyNew;
//}

void SolidColorMode() {

	if (beatDetected[2] == 2) r = r + 70;
	if (beatDetected[8] == 2) b = b + 70;
	if (beatDetected[12] == 2) g = g + 70;

	r = constrain(r - 1, 0, 255);
	b = constrain(b - 1, 0, 255);
	g = constrain(g - 1, 0, 255);

	for (int i = 0; i < NUM_LEDS; i++) {
		leds[i] = CRGB(r, g, b);
	}

	fadeAll(250);
	LEDS.show();
}

void ambient_rainbow() {
	delay(analogRead(A1));

	for (int i = 0; i < NUM_LEDS; i++) {
		int temp = -1 * ledx[i] + ledy[i];
		leds[i] = CHSV(temp + hue, 255, 255);
	}
	hue = hue + 2;

	LEDS.show();
}

void beatMeter() {
	slope = 1;

	if (beatDetected[2] == 2) {
		yintLow = -52;
		lowSpeed -= .85*lowSpeed;
		for (int i = 0; i < cor14; i++) leds[i] = CHSV(colorOne, 255, 180);
	}
	if (beatDetected[12] == 2) {
		yintHigh = 52;
		highSpeed -= .85*highSpeed;
		for (int i = cor14; i < cor28; i++) leds[i] = CHSV(colorTwo, 255, 180);
	}

	if (time > highSpeed) {
		if (yintHigh > 6) yintHigh--;
		time = 0;
	}
	if (time1 > lowSpeed) {
		if (yintLow < -6) yintLow++;
		time1 = 0;
	}
	if (yintHigh == 8) colorTwo = random8(255);
	if (yintLow == -8) colorOne = random8(255);

	for (int i = cor0; i < cor14 - 6; i++) {
		if (slope * ledx[i] + yintLow - ledy[i] < 1 && slope *ledx[i] + yintLow - ledy[i] > -1) leds[i] = CRGB::Black;
	}
	for (int i = cor14 + 5; i < cor28; i++) {
		if (slope * ledx[i] + yintHigh - ledy[i] < 1 && slope *ledx[i] + yintHigh - ledy[i] > -1) leds[i] = CRGB::Black;
	}

	if (yintLow > -7 || !leds[cor0 + 7]) {
		for (int i = 0; i < spectrumValue[2] - 10; i++) {  // For as loud as the low frequency is, add that many sparkles to bottom left
			int pos = random16(cor0 + 7, cor14);
			//brightness = map(spectrumValue[2], 0, 14, 0, 255);
			leds[pos] = CHSV(colorOne, random8(200, 255), random(30, 130));
		}
	}
	if (yintHigh < 7 || !leds[cor28 - 7]) {
		for (int i = 0; i < spectrumValue[12] - 10; i++) {  // For as loud as the high frequency is, add that many sparkles to top right
			int pos = random16(cor14, cor28 - 7);
			//brightness = map(spectrumValue[12], 0, 14, 0, 255);
			leds[pos] = CHSV(colorTwo, random8(200, 255), random8(30, 130));
		}
	}

	for (int i = 0; i < 6; i++) {   // Creates white corners on top right and bottom left
		leds[i] = CHSV(0, 0, 255);
		leds[cor14 + i - 1] = CHSV(0, 0, 255);
		leds[cor14 - i - 1] = CHSV(0, 0, 255);
		leds[cor28 - i - 1] = CHSV(0, 0, 255);
	}
	highSpeed = constrain(highSpeed + .2 + .01*highSpeed, 0, 200);
	lowSpeed = constrain(lowSpeed + .2 + .01*lowSpeed, 0, 200);
	fadeAll(253);
	LEDS.show();
}

void circle_midOut_One_Experimental() {   // This mode shows a circle expanding from the middle
	if (beatDetected[2] == 2) {
		radius = 10;
		time1 = 0;
		speed -= .85*speed;  // old is .85
	}

	//brightness = 255 - (radius - 10) * 6;   // Makes brightness decrease as radius goes up (radius goes up)
	brightness = 255;
	// Old way of doing the speed of the radius increments
	/*if (time1 > radius - 10) {
	radius = radius + .5;
	time1 = 0;
	}*/
	if (radius > 38) sweepStatus = true;  // When the circle is done growing, start the confetti
	else sweepStatus = false;

	//if (time1 > radius - 10) {
	if (time1 > speed) {
		radius = radius + .5;
		time1 = 0;
	}

	if (radius < 36) {  // Only increment 'hue' when the circle is still growing
		hue = hue + 2;
	}

	switch (side) {
	case 0:
		for (int i = cor0; i < cor14; i++) {
			if (pow(ledx[i], 2) + pow(ledy[i], 2) - pow(radius, 2) > -20 && pow(ledx[i], 2) + pow(ledy[i], 2) - pow(radius, 2) < 20) leds[i] = CHSV(hue, 255, brightness);
		}
		side = 1;
		break;
	case 1:
		for (int i = cor14; i < cor28; i++) {
			if (pow(ledx[i], 2) + pow(ledy[i], 2) - pow(radius, 2) > -20 && pow(ledx[i], 2) + pow(ledy[i], 2) - pow(radius, 2) < 20) leds[i] = CHSV(hue, 255, brightness);
		}
		side = 0;
		break;
	}

	counter++;  // This exists to make the animation slower. hue only gets incremented every 3 goes arounds
	if (counter == 6) {
		hue++;
		counter = 0;
	}

	sweepStatus = true; // makes the sparkles stay on forever
	if (sweepStatus) {
		for (int i = 0; i < 1; i++) {
			int pos = random16(NUM_LEDS);
			leds[pos] = CHSV(96, 255, random8(10, 100)); // green 96
		}
		for (int i = 0; i < spectrumValue[12] / 5; i++) {  // For as loud as the high frequency is, add that many sparkles to top right
			int pos = random16(NUM_LEDS);
			//brightness = map(spectrumValue[12], 0, 14, 0, 255);
			leds[pos] = CHSV(hue, random8(200, 255), random8(30, 130)); // blue 160
		}
	}

	speed = constrain(speed + .15 + .02*speed, 0, 180);  // old is .1
	fadeAll(250);
	LEDS.show();
}