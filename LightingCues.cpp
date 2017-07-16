#include "LightingCues.h"
#include "FastLED.h"
//FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    2
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    67
#define MICROS_PER_UPDATE  1000
#define MICROS_PER_GHUE 80000

// LightingCues lc;

    // typedef void (LightingCues::*FP)();
    //   FP const commandTable[44] = {
    //     &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
    //     &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
    //     &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
    //     &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
    //     &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
    //     &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
    //     &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
    //     &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
    //     &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
    //     &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
    //     &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE
    //   };
	// typedef void (LightingCues::*FP)();
	// FP const commandTable[44] = {
	//   &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
	//   &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
	//   &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
	//   &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
	//   &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
	//   &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
	//   &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
	//   &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
	//   &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
	//   &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
	//   &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE
	// };

	CRGB leds[NUM_LEDS];
	uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
	uint8_t gHue = 0; // rotating "base color" used by many of the patterns
	int lastUpdate = 0;
	int lastGHue = 0;
	int currColor = 0;
	int curCue = 0;
	bool rainbowCue = true;
	int lightSpeed = 10; // 0 - 10
	int brightness = 255;
	bool isActive = true; 

	LightingCues::LightingCues(){
		// FP initTable[] = 
		// commandTable = initTable;

	}
	LightingCues::~LightingCues(){

	}
	void LightingCues::lightSetup(){
		delay(3000);
  		FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
		FastLED.setBrightness(brightness);
	}
	void LightingCues::lightingLoop(){

	  int curTime = micros();
	  if(curTime - lastUpdate > MICROS_PER_UPDATE){
	    lastUpdate = curTime;
	    //Call proper function
		// ((LightingCues*)this)->LightingCues::commandTable[curCue]()
		// lc.*(lc)
	    setRainbow();
	    // confetti();
	    // send the 'leds' array out to the actual LED strip
	    if(isActive){
	   		FastLED.show();  
	    }
	  }
	  if(curTime - lastGHue > (11 - lightSpeed) * 10000){
	    lastGHue = curTime;
	    gHue++;
    	}
	}
	void LightingCues::setLightColor(int color){
		rainbowCue = false;
		currColor = color;
	}
	void LightingCues::setRainbow(){
		rainbowCue = true;
	}
	void LightingCues::speedUp(){
		if(lightSpeed < 10){
			lightSpeed++;	
		}
	}
	void LightingCues::slowDown(){
		if(lightSpeed > 0){
			lightSpeed--;
		}
	}
	void LightingCues::toggleLight(){
		isActive = !isActive;
	}
	void LightingCues::brightnessUp(){
		if(brightness <= 245){
			brightness += 10;
		}
	}
	void LightingCues::brightnessDown(){
		if(brightness >= 10){
			brightness -= 10;
		}
	}
	void LightingCues::setCue(int cue){
		curCue = cue;
	}

	void LightingCues::NOCUE() {
	}

	// void LightingCues::addGlitter( fract8 chanceOfGlitter) 
	// {
	//   if( random8() < chanceOfGlitter) {
	//     leds[ random16(NUM_LEDS) ] += CRGB::White;
	//   }
	// }

	void LightingCues::sinelon()
	{
	  // a colored dot sweeping back and forth, with fading trails
	  fadeToBlackBy( leds, NUM_LEDS, 2);
	  int pos = beatsin16(lightSpeed * 6,0,NUM_LEDS);
	  leds[pos] += CHSV( gHue, 255, 192);
	}
	void LightingCues::larson(){

	  
	}
	void LightingCues::bpm()
	{
	  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
	  uint8_t BeatsPerMinute = 62;
	  CRGBPalette16 palette = PartyColors_p;
	  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
	  for( int i = 0; i < NUM_LEDS; i++) { //9948
	    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
	  }
	}
	void LightingCues::strobeColor(){
	  
	}

	void LightingCues::juggle() {
	  // eight colored dots, weaving in and out of sync with each other
	  fadeToBlackBy( leds, NUM_LEDS, 11 - lightSpeed);
	  byte dothue = 0;
	  for( int i = 0; i < lightSpeed + 5; i++) {
	    leds[beatsin16(i + 7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
	    dothue += 256 / 8;
	  }
	}
	void LightingCues::solidColor(){
	  for(int i=0; i < NUM_LEDS; i++){
	    leds[i] = currColor;
	  }
	}
	void LightingCues::rainbowCycle() {
	    for(int i=0; i< NUM_LEDS; i++) {
	      leds[i] = CHSV(gHue,200,255);
	    }
	    gHue += lightSpeed / 3;
	}