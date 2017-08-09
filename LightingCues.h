#ifndef LightingCues_h
#define LightingCues_h

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
    void setBrightness(int change);
    int getBrightness();
    void setSpeed(int change);
    int getSpeed();

  private:
    typedef void (LightingCues::*FP)();
    //Command table that we index into with a cue to choose the function we are running.
    FP const commandTable[44] = {
      &LightingCues::brightnessUp, &LightingCues::brightnessDown, &LightingCues::NOCUE, &LightingCues::blackout,
      &LightingCues::testOffset, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::speedUp,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::slowDown,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::bpm,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::juggle,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::sinelon, &LightingCues::rainbowCycle
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
    void staggerRainbow();
    // State modifier buttons
    void speedUp();
    void slowDown();
    void pausePlay();
    void brightnessUp();
    void brightnessDown();
    void activate();
    void setLightColor(int color); //hex 
    void setRainbow();
    void testOffset();
    
    //Helpers
    bool shouldSetCue(int cue);
    void callCue(int cue);
    void NOCUE();

    void getPosition(int virtualAddr, int max, int min);
};

// extern LightingCues LC;

#endif