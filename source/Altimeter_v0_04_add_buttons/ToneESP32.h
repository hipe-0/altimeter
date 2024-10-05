
#include "iferror.h"
const int PIN = 17;

hw_timer_t* timer = NULL;
bool value = true;
int frequency = 20;  // 20 to 20000

void IRAM_ATTR onTimer() {
  value = !value;
  digitalWrite(PIN, value);
}

class Tone {

private:
  void setupTimer_() {
    // Use 1st timer of 4  - 1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up
    timer = timerBegin(0, 80, true);  //div 80
    timerAttachInterrupt(timer, &onTimer, true);
  }

  void setFrequency_(long frequencyHz) {
    timerAlarmDisable(timer);
    timerAlarmWrite(timer, 1000000l / frequencyHz, true);
    timerAlarmEnable(timer);
  }

  float computeFrequency_(int note){
    return 440.0 * pow(2.0, note / 12.0);
  }

public:
  void init() {
    pinMode(PIN, OUTPUT);  // sets the digital pin as output
    setupTimer_();
  }

  void playBeep(int note, long durationMs) {
    long frequencyHz = computeFrequency_(note);
    DEBUGVAL("frequencyHz",frequencyHz)
    setFrequency_(frequencyHz);
    timerStart(timer);
    delay(durationMs);
    timerStop(timer);
    value = false;
    digitalWrite(PIN, value);

    DEBUGTXT("playBeep stopped")
  }



};