#ifndef LightingCues_h
#define LightingCues_h

class LightingCues {
  public:
    LightingCues();
    ~LightingCues();

    void lightSetup();
    void lightingLoop();
    void setCue(int cue);

  private:
    typedef void (LightingCues::*FP)();
    //Command table that we index into with a cue to choose the function we are running.
    FP const commandTable[44] = {
      &LightingCues::brightnessUp, &LightingCues::brightnessDown, &LightingCues::NOCUE, &LightingCues::toggleLight,
      &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
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
    void sinelon();
    void larson();
    void bpm();
    void strobeColor();
    void juggle();
    void solidColor();
    void rainbowCycle();

    // State modifiers
    void speedUp();
    void slowDown();
    void toggleLight();
    void brightnessUp();
    void brightnessDown();

    void setLightColor(int color); //hex 
    void setRainbow();
    void LightingCues::setTimeOffset(int set);
    int LightingCues::getTimeOffset();
    
    //Helpers
    bool shouldSetCue(int cue);
    void callCue(int cue);
    void NOCUE();
};

// extern LightingCues LC;

#endif