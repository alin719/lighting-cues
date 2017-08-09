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
	int timeOffSet = 0;

	CRGBPalette16 currentPalette;
	TBlendType currentBlending;

	LightingCues::LightingCues(){
		
	}
	LightingCues::~LightingCues(){

	}
	void LightingCues::lightSetup(){
		delay(3000);
  		FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
		FastLED.setBrightness(brightness);

		currentPalette = RainbowColors_p;
    	currentBlending = LINEARBLEND;
	}
	void LightingCues::lightingLoop(){

	    //Call proper function
    	if (isActive && curCue < 44){
    		callCue(curCue);
   			FastLED.show(); 
    	}  
	    
		if(micros() - lastGHue > (11 - lightSpeed) * 10000){
		lastGHue = micros();
		gHue++;
		}
	}
	void LightingCues::getPosition(int virtualAddr, int max, int min){
		
	}
	void LightingCues::setLightColor(int color){
		rainbowCue = false;
		currColor = color;
	}
	void LightingCues::setRainbow(){
		rainbowCue = true;
	}
	void LightingCues::speedUp(){
		if(lightSpeed < 50){
			lightSpeed++;	
		}
	}
	void LightingCues::activate(){
		isActive = !isActive;
	}

	void LightingCues::slowDown(){
		if(lightSpeed > 0){
			lightSpeed--;
		}
	}
	void LightingCues::pausePlay(){
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
    	if (shouldSetCue(cue)) curCue = cue;
	}

	void LightingCues::blackout() {
		fadeToBlackBy(leds, NUM_LEDS, 255);
		FastLED.show();  
		pausePlay();
	}

	void LightingCues::NOCUE() {
	}
	void LightingCues::setTimeOffset(int set){
		timeOffSet = set;
	}
	int LightingCues::getTimeOffset(){
		return timeOffSet;
	}
	void LightingCues::testOffset(){
		if(timeOffSet == 1000){
			timeOffSet = 0;
		}else{
			timeOffSet = 1000;
		}
	}
	int LightingCues::getBrightness(){
		return brightness;
	}
	void LightingCues::setBrightness(int change){
		brightness = change;
	}
	int LightingCues::getSpeed(){
		return lightSpeed;
	}
	void LightingCues::setSpeed(int change){
		lightSpeed = change;
	}
	int LightingCues::getGHue(){
		return gHue;
	}
	void LightingCues::setGHue(int change){
		gHue = change;
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
	  int pos = beatsin16(lightSpeed * 6,0,NUM_LEDS,timeOffSet,0);
	  leds[pos] += ColorFromPalette(currentPalette, gHue, brightness, currentBlending);
	}
	void LightingCues::larson(){

	  
	}
	void LightingCues::bpm()
	{
	  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
	  uint8_t BeatsPerMinute = 62;
	  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
	  for( int i = 0; i < NUM_LEDS; i++) { //9948
	    leds[i] = ColorFromPalette(currentPalette, gHue+(i*2), beat-gHue+(i*10));
	  }
	}
	void LightingCues::strobeColor(){
	  
	}

	void LightingCues::juggle() {
	  // eight colored dots, weaving in and out of sync with each other
	  fadeToBlackBy( leds, NUM_LEDS, 11 - lightSpeed);
	  byte dothue = 0;
	  for( int i = 0; i < lightSpeed + 5; i++) {
	    leds[beatsin16(i + 7,0,NUM_LEDS,timeOffSet,0)] |= CHSV(dothue, 200, brightness);
	    dothue += 256 / 8;
	  }
	}
	void LightingCues::solidColor(){
	  for(int i=0; i < NUM_LEDS; i++){
	    leds[i] = currColor;
	  }
	}

  void LightingCues::callCue(int cue) {
    ((LightingCues*)this->*(commandTable[cue]))();
  }

  bool LightingCues::shouldSetCue(int cue) {
    if (cue <= 4 || cue == 27 || cue == 31){
      callCue(cue);
      return false;
    }
    if (commandTable[cue] == &LightingCues::NOCUE) return false;
    return true;
  }

	void LightingCues::rainbowCycle() {
	    for(int i=0; i< NUM_LEDS; i++) {
	      leds[i] = ColorFromPalette(currentPalette,gHue,brightness,currentBlending);
	    }
	    gHue += lightSpeed / 3;
	}


