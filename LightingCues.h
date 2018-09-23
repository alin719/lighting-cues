#ifndef LightingCues_h
#define LightingCues_h
#include "stdint.h"

class LightingCues {
  public:
    LightingCues();
    ~LightingCues();

    void lightSetup();
    void lightingLoop();
    void setCue(int cue);

    // Sync Modifiers
    void setGHue(int change);
    int getGHue();
    void setTimeOffset(int set);
    int getTimeOffset();
    void setBaseOffset(int set);
    int getBaseOffset();
    void setBrightness(int change);
    int getBrightness();
    void setSpeed(int change);
    int getSpeed();
    void peakDet(int amp);
    void setMaster(bool master);

    void shiftTimeOffset();
    // Positional Setters
    void setPositionInfo(uint8_t setPosition, uint8_t setCount, int setSubPosition, int setSubCount);
    void assignInstrumentation(bool setIsSnare, bool setIsAxis);
    bool shouldSetCue(int cue);


  private:
    typedef void (LightingCues::*FP)();
    //Command table that we index into with a cue to choose the function we are running.
    FP const commandTable[44] = {
      &LightingCues::brightnessUp, &LightingCues::brightnessDown, &LightingCues::NOCUE, &LightingCues::blackout,
      &LightingCues::redPalette, &LightingCues::testOffset,  &LightingCues::NOCUE, &LightingCues::NOCUE,
      &LightingCues::stripePalette, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
      &LightingCues::rainbowPalette, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::rainbowCenterStagger,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::rainbowStagger,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::speedUp,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::slowDown,
      &LightingCues::axisOutLarson, &LightingCues::larson, &LightingCues::centerBpm, &LightingCues::bpm,
      &LightingCues::NOCUE, &LightingCues::centerSinelon, &LightingCues::centerSinelonOffset, &LightingCues::juggle,
      &LightingCues::strobeRainbow, &LightingCues::rainbowReact, &LightingCues::sinelon, &LightingCues::rainbowCycle
    };
    
    // Animations
    void blackout();
    void sinelon();
    void centerSinelon();
    void centerSinelonOffset();
    void larson();
    void bpm();
    void centerBpm();
    void strobeRainbow();
    void juggle();
    void solidColor();
    void rainbowCycle();
    // void rainbowCycleOffset();
    void rainbowReact();
    void solidReact();
    void rainbowStagger();
    void rainbowCenterStagger();
    void juggleStagger();
    void axisOutLarson();
    // void addGlitter( fract8 chanceOfGlitter);

    // State modifier buttons
    void speedUp();
    void slowDown();
    void pausePlay();
    void brightnessUp();
    void brightnessDown();
    void setLightColor(int color); //hex 
    void setRainbow();
    void testOffset();
    void redPalette();
    void stripePalette();
    void rainbowPalette();
    
    //Helpers
    void callCue(int cue);
    void NOCUE();

};

// extern LightingCues LC;

#endif