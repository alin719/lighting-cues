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


  private:
    typedef void (LightingCues::*FP)();
    //Command table that we index into with a cue to choose the function we are running.
    FP const commandTable[44] = {
      &LightingCues::brightnessUp, &LightingCues::brightnessDown, &LightingCues::NOCUE, &LightingCues::blackout,
      &LightingCues::NOCUE, &LightingCues::testOffset,  &LightingCues::NOCUE, &LightingCues::NOCUE,
      &LightingCues::redPalette, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
      &LightingCues::stripePalette, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
      &LightingCues::strobeColor, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::rainbowStagger,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::speedUp,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::slowDown,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::bpm,
      &LightingCues::NOCUE, &LightingCues::sinelon, &LightingCues::rainbowCycleOffset, &LightingCues::juggle,
      &LightingCues::NOCUE, &LightingCues::rainbowReact, &LightingCues::sinelon, &LightingCues::rainbowCycle
    };
    
    // Animations
    void blackout();
    void sinelon();
    void larson();
    void bpm();
    void strobeColor();
    void juggle();
    void solidColor();
    void rainbowCycle();
    void rainbowCycleOffset();
    void rainbowReact();
    void solidReact();
    void rainbowStagger();
    void juggleStagger();
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
    bool shouldSetCue(int cue);
    void callCue(int cue);
    void NOCUE();

};

// extern LightingCues LC;

#endif