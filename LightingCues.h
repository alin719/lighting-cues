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
      FP const commandTable[44] = {
        &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
        &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
        &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
        &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
        &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
        &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
        &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
        &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
        &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
        &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE,
        &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE, &LightingCues::NOCUE
      };
      
    void setLightColor(int color); //hex 
    void setRainbow();
    //void confetti();
    void speedUp();
    void slowDown();
    void toggleLight();
    void brightnessUp();
    void brightnessDown();
    void NOCUE();

    // void addGlitter(fract8 chanceOfGlitter);
    void sinelon();
    void larson();
    void bpm();
    void strobeColor();
    void juggle();
    void solidColor();
    void rainbowCycle();
};

extern LightingCues LC;

#endif