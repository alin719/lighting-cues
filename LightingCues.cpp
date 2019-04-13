#include "LightingCues.h"
// https://github.com/FastLED/FastLED
#include "FastLED.h"
//FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    2
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define DEFAULT_NUM_LEDS    67 //Axis has 135, rest have 67
#define DEFAULT_AXIS_NUM_LEDS	135
#define MICROS_PER_UPDATE  1000
#define MICROS_PER_GHUE 80000
#define REACT_FADE_INTERVAL 30 //0-255

#define MAX_LIGHT_SPEED 	20
#define MIN_SPEED 			1
#define MIN_BRIGHTNESS 	20
#define MAX_BRIGHTNESS 255
#define NUM_PALETTES 3

static CRGB* leds;
static int NUM_LEDS = DEFAULT_NUM_LEDS;
static uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
static uint8_t gHue = 0; // rotating "base color" used by many of the patterns
static int lastUpdate = 0;
static int lastGHue = 0;
static int currColor = 0;
static int curCue = 0;
static bool rainbowCue = true;
static int lightSpeed = 10; // 0 - 10
static int brightness = 255;
static bool isActive = true;
static int baseOffSet = 0;
static int timeOffSet = 0;
static int peakAmp = 0;

static bool isOff = false;
static int lastCue = 0;

// For center sinelon offset
static int lastPos = 0;

static uint8_t position = 0;
static uint8_t count = 1;
static uint8_t subCount = 1;
static uint8_t subPosition = 0;

static bool isMaster = false;
static bool isSnare = false;
static bool isAxis = false;

// position 	Snare->Axis->Toms
// count		|drums|
// subPosition 	0 indexed at Axis, outward
// subCount		|like drums| 

CRGBPalette16 currentPalette;

// default to Rainbow palette
static int paletteCode = 0;
// palette codes are:
// 0: Rainbow
// 1: Red (red_gp)
// 2: Red and White (cardinalStripe_gp)
DEFINE_GRADIENT_PALETTE( cardinalStripe_gp ) {
0,   255,  0,  0,   //red
63,   255,  0,  0,   //to red
64,   255,255,255, //to white
127,   255,255,255, //to white
128,   255,  0,  0,   //to red
191,   255,  0,  0,   //to red
192,   255,255,255, //to white
255,   255,255,255 //to white
}; //full white
DEFINE_GRADIENT_PALETTE( red_gp) {
	0, 255, 0, 0, //it's just red
	255, 255, 0, 0 //it's still just red
};
DEFINE_GRADIENT_PALETTE( white_gp) {
	0, 255, 255, 255, // it's literally white
	255, 255, 255, 255 // lmao white
};


TBlendType currentBlending;

LightingCues::LightingCues() {

}
LightingCues::~LightingCues() {

}
void LightingCues::lightSetup() {
	delay(2000);

	if (isAxis) NUM_LEDS = DEFAULT_AXIS_NUM_LEDS;
	leds = new CRGB[NUM_LEDS];
	
	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
	FastLED.setBrightness(brightness);

	//TODO: return to RainbowColors_p;
	//currentPalette = heatmap_gp;
	currentPalette = RainbowColors_p;
	paletteCode = 0;

	currentBlending = LINEARBLEND;
}

void LightingCues::lightingLoop() {

	//Call proper function
	if (curCue < 44) { //} && !isOff){
		callCue(curCue);
		if (isMaster) {
			leds[0] = CRGB::Red;
		}
		FastLED.show();
	}

	if (micros() - lastGHue > (11 - lightSpeed) * 10000) {
		lastGHue = micros();
		gHue++;
	}
}
void LightingCues::setMaster(bool master) {
	isMaster = master;
}

void LightingCues::assignInstrumentation(bool setIsSnare, bool setIsAxis) {
	isSnare = setIsSnare;
	isAxis = setIsAxis;
}

void LightingCues::setPositionInfo(uint8_t setPosition, uint8_t setCount, int setSubPosition, int setSubCount) {
	position = setPosition;
	count = setCount;
	subPosition = setSubPosition;
	subCount = setSubCount;
	// shiftTimeOffset();
}

void LightingCues::setLightColor(int color) {
	rainbowCue = false;
	currColor = color;
}
void LightingCues::setRainbow() {
	rainbowCue = true;
}
void LightingCues::speedUp() {
	if (lightSpeed < MAX_LIGHT_SPEED) {
		lightSpeed++;
	}
}
void LightingCues::slowDown() {
	if (lightSpeed > MIN_SPEED + 1) {
		lightSpeed--;
	}
}
void LightingCues::pausePlay() {
	isActive = !isActive;
}
void LightingCues::brightnessUp() {
	if (brightness <= MAX_BRIGHTNESS - 10) {
		brightness += 10;
	}
}
void LightingCues::brightnessDown() {
	if (brightness >= MIN_BRIGHTNESS + 10) {
		brightness -= 10;
	}
}
void LightingCues::setCue(int cue) {
	if (shouldSetCue(cue)) {
		if (lastCue != curCue) lastCue = curCue;
		curCue = cue;
	}
}
void LightingCues::peakDet(int amp) {
	peakAmp = amp;
}
void LightingCues::blackout() {
	// Short fix since Axis is responding poorly to blackout
	if(!isAxis){
		fadeToBlackBy(leds, NUM_LEDS, 255);
		FastLED.show();
	}
}

void LightingCues::flashlight() {
	CRGBPalette16 white_palette = white_gp;
	for (int i = 0; i < NUM_LEDS; i++) {
		leds[i] = ColorFromPalette(white_palette, gHue, brightness);
	}
	gHue += lightSpeed / 3;
}

void LightingCues::NOCUE() {
}

void LightingCues::shiftTimeOffset() {
	int positionOffset = 0;
	// if (curCue == 38 or curCue == 37) { // Currently just set for rainbowCycleOffset
	// 	int TOTAL_OFFSET = MICROS_PER_UPDATE*255/lightSpeed*3;
	// 	positionOffset = TOTAL_OFFSET*position/count;
	// }
	timeOffSet = baseOffSet + positionOffset;
}

void LightingCues::setBaseOffset(int set) {
	baseOffSet = set;
	timeOffSet = set;
	// shiftTimeOffset();
}

int LightingCues::getBaseOffset() {
	return baseOffSet;
}

void LightingCues::setTimeOffset(int set) {
	timeOffSet = set;
}

int LightingCues::getTimeOffset() {
	return timeOffSet;
}

void LightingCues::testOffset() {
	if (timeOffSet == 1000) {
		timeOffSet = 0;
	} else {
		timeOffSet = 1000;
	}
}
int LightingCues::getBrightness() {
	return brightness;
}
void LightingCues::setBrightness(int change) {
	if (change >= MIN_BRIGHTNESS && change <= MAX_BRIGHTNESS) {
		brightness = change;
	}
}
int LightingCues::getSpeed() {
	return lightSpeed;
}
void LightingCues::setSpeed(int change) {
	if (change >= MIN_SPEED && change <= MAX_LIGHT_SPEED) {
		lightSpeed = change;
	} 
}
int LightingCues::getGHue() {
	return gHue;
}
void LightingCues::setGHue(int change) {
	gHue = change;
}

int LightingCues::getPaletteCode() {
	return paletteCode;
}
void LightingCues::setPaletteCode(int code) {
	if(code < NUM_PALETTES){
		paletteCode = code;
		switch (paletteCode) {
			case 0: {
				currentPalette = RainbowColors_p;
				break;
			}
			case 1: {
				currentPalette = red_gp;
				break;
			}
			case 2: {
				currentPalette = cardinalStripe_gp;
				break;
			}
			default: {
				// do nothing
			}
		}
	}
}
void LightingCues::redPalette() {	
	setPaletteCode(1);
}
void LightingCues::stripePalette() {	
	setPaletteCode(2);	
}
void LightingCues::rainbowPalette() {
	setPaletteCode(0);
}
// void LightingCues::addGlitter(fract8 chanceOfGlitter)
// {
//   if( random8() < chanceOfGlitter) {
//     leds[ random16(NUM_LEDS) ] += CRGB::White;
//   }
// }

void LightingCues::sinelon()
{
	// a colored dot sweeping back and forth, with fading trails
	fadeToBlackBy( leds, NUM_LEDS, 2);
	int pos = beatsin16(lightSpeed * 6, 0, NUM_LEDS, timeOffSet, 0);
	leds[pos] += ColorFromPalette(currentPalette, gHue, brightness, currentBlending);
}


void LightingCues::centerSinelon() {
	// a colored dot sweeping back and forth, with fading trails
	fadeToBlackBy( leds, NUM_LEDS, 2);
	int pos = beatsin16(lightSpeed * 6, 0, NUM_LEDS, timeOffSet, 0);
	if (isSnare) pos = NUM_LEDS - pos - 1;
	leds[pos] += ColorFromPalette(currentPalette, gHue, brightness, currentBlending);	
}

void LightingCues::centerSinelonOffset() {
	// a colored dot sweeping back and forth, with fading trails
	fadeToBlackBy( leds, NUM_LEDS, 2);
	int pos = beatsin16(lightSpeed * 6, 0, NUM_LEDS, timeOffSet, 0);
	if (isSnare) pos = NUM_LEDS - pos - 1;
	int multiplier = 1;
	if (pos < lastPos) { //coming backwards
		multiplier = -1;
	}
	pos += abs(multiplier*subPosition*NUM_LEDS/count);
	pos = subPosition % NUM_LEDS;
	leds[pos] += ColorFromPalette(currentPalette, gHue, brightness, currentBlending);	
}

void LightingCues::larson() {
	// Scans down the line
	uint8_t BeatsPerMinute = 62;
	uint8_t beat = beatsin8(BeatsPerMinute, 0, 255, timeOffSet, 0);
	int power = (beat*count)/255;
	int staggerPerdecage = 8;
	int offset = 255*position/count*staggerPerdecage/10;
	for (int i = 0; i < NUM_LEDS; i++) {
		leds[i] = ColorFromPalette(currentPalette, gHue + offset, brightness/(1<<(2*abs(position - power))), currentBlending);
	}
	gHue += lightSpeed / 3;
}
void LightingCues::axisOutLarson() {
	// Scans down the line
	// Scans down the line
	uint8_t BeatsPerMinute = 62;
	uint8_t beat = beatsin8(BeatsPerMinute, 0, 255, timeOffSet, 0);
	int power = (beat*subCount)/255;
	int staggerPerdecage = 8;
	int offset = 255*position/count*staggerPerdecage/10;
	for (int i = 0; i < NUM_LEDS; i++) {
		leds[i] = ColorFromPalette(currentPalette, gHue + offset, brightness/(1<<(2*abs(subPosition - power))), currentBlending);
	}
	gHue += lightSpeed / 3;
}
void LightingCues::centerBpm()
{
	// colored stripes pulsing at a defined Beats-Per-Minute (BPM)
	uint8_t BeatsPerMinute = 62;
	uint8_t beat = beatsin8(BeatsPerMinute, 64, 255, timeOffSet, 0);
	for ( int i = 0; i < NUM_LEDS; i++) { //9948
		int pos = i;
		if (isSnare) pos = NUM_LEDS - pos - 1;
		leds[pos] = ColorFromPalette(currentPalette, gHue + (i * 2), beat - gHue + (i * 10));
	}
}

void LightingCues::bpm()
{
	// colored stripes pulsing at a defined Beats-Per-Minute (BPM)
	uint8_t BeatsPerMinute = lightSpeed * 6;
	uint8_t beat = beatsin8(BeatsPerMinute, 64, 255, timeOffSet, 0);
	for ( int i = 0; i < NUM_LEDS; i++) { //9948
		leds[i] = ColorFromPalette(currentPalette, gHue + (i * 2), beat - gHue + (i * 10));
	}
}

void LightingCues::spin()
{
	// spin little bright lights
	for ( int i = 0; i < NUM_LEDS; i++) { //9948
		leds[i] = ColorFromPalette(currentPalette, gHue + (i * 2), (i * 10) - gHue);
	}
	gHue += lightSpeed/3;
}

void LightingCues::centerSpin()
{
	// spin little bright lights
	for ( int i = 0; i < NUM_LEDS; i++) { //9948
		int pos = i;
		if (isSnare) pos = NUM_LEDS - pos - 1;
		leds[pos] = ColorFromPalette(currentPalette, gHue + (i * 2), (i * 10) - gHue);
	}
	gHue += lightSpeed/3;
}

void LightingCues::strobeRainbow() {
	//Like BPM but with binary brightness
	uint8_t BeatsPerMinute = 20*(lightSpeed + 1);
	uint8_t beat = beatsin8(BeatsPerMinute, 64, 255, timeOffSet, 0);
	int on = beat/200;
	for(int i = 0; i < NUM_LEDS; i++) {
		leds[i] = ColorFromPalette(currentPalette, gHue + (i * 2), on*255);
	}
}

void LightingCues::juggle() {
	// eight colored dots, weaving in and out of sync with each other
	fadeToBlackBy( leds, NUM_LEDS, 11 - lightSpeed);
	byte dothue = 0;
	for ( int i = 0; i < lightSpeed + 5; i++) {
		leds[beatsin16(i + 7, 0, NUM_LEDS, timeOffSet, 0)] |= CHSV(dothue, 200, brightness);
		dothue += 256 / 8;
	}
}
void LightingCues::juggleStagger() {
	// eight colored dots, weaving in and out of sync with each other
	fadeToBlackBy( leds, NUM_LEDS, 11 - lightSpeed);
	byte dothue = 0;
	for ( int i = 0; i < lightSpeed + 5; i++) {
		leds[beatsin16(i + 7, 0, NUM_LEDS, timeOffSet, 0)] |= CHSV(dothue, 200, brightness);
		dothue += 256 / 8;
	}
}
void LightingCues::rainbowReact() {
	if (peakAmp) {
		for (int i = 0; i < NUM_LEDS; i++) {
			leds[i] = ColorFromPalette(currentPalette, gHue, peakAmp, currentBlending);
			gHue += lightSpeed / 15;
		}
		peakAmp = 0;
	}
	else {
		fadeToBlackBy(leds, NUM_LEDS, REACT_FADE_INTERVAL);
	}
}
void LightingCues::rainbowStagger() {
	// int staggerPerdecage = 10;
	int offset = 255*position/count;//*staggerPerdecage/10;	
	for (int i = 0; i < NUM_LEDS; i++) {
		leds[i] = ColorFromPalette(currentPalette, gHue + offset, brightness, currentBlending);
	}
	gHue += lightSpeed / 3;
}

void LightingCues::rainbowCenterStagger() {
	int staggerPerdecage = 8;
	uint8_t functionalPosition = subPosition;
	if (!isAxis) functionalPosition++;
	int offset = 255*functionalPosition/count*staggerPerdecage/10;	
	for (int i = 0; i < NUM_LEDS; i++) {
		leds[i] = ColorFromPalette(currentPalette, gHue + offset, brightness, currentBlending);
	}
	gHue += lightSpeed / 3;
}

void LightingCues::solidReact() {

}

void LightingCues::callCue(int cue) {
	((LightingCues*)this->*(commandTable[cue]))();
}

bool LightingCues::shouldSetCue(int cue) {
	if (cue <= 2 || cue == 27 || cue == 31 || cue == 4 || cue == 8 | cue == 12) {
		lastCue = curCue;
		callCue(cue);
		return false;
	}
	if (commandTable[cue] == &LightingCues::NOCUE) return false;
	return true;
}

void LightingCues::rainbowCycle() {
	for (int i = 0; i < NUM_LEDS; i++) {
		leds[i] = ColorFromPalette(currentPalette, gHue, brightness, currentBlending);
	}
	gHue += lightSpeed / 3;
}




