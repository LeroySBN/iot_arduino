#include <SevSeg.h>

SevSeg sevseg;

const int INITIAL_COUNTDOWN = 1500;
//const int INITIAL_COUNTDOWN = 3600;
int COUNTDOWN = INITIAL_COUNTDOWN;
unsigned long lastUpdate = 0;  // To store the last time the countdown was updated
const int countdownInterval = 1000;  // 1 second interval for the countdown

const int  buttonResetPin = A0;
const int  buttonStopPin = A1;
int resetNow, stopNow;

void setup() {
  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {9, 2, 3, 5, 6, 8, 7, 4};
  bool resistorsOnSegments = true;
  byte hardwareConfig = COMMON_CATHODE;

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);

  Serial.begin(9600);
  pinMode(buttonResetPin, INPUT_PULLUP);
  pinMode(buttonStopPin, INPUT_PULLUP);
}

void loop() {
  resetNow = analogRead(buttonResetPin);
  stopNow = analogRead(buttonStopPin);

  if (resetNow < 500) {
    COUNTDOWN = INITIAL_COUNTDOWN;
  }

  if (stopNow < 500) {
    COUNTDOWN = 0;
  }

  String StringMinutes = String(COUNTDOWN / 60);
  int seconds = COUNTDOWN % 60;
  String StringSeconds = (seconds < 10) ? "0" + String(seconds) : String(seconds);
  String result = StringMinutes + StringSeconds;
  int now = result.toInt();
  
  // Refresh the display continuously
  sevseg.setNumber(now, 2);
  sevseg.refreshDisplay();

  // Check if 1 second has passed since the last update
  if (millis() - lastUpdate >= countdownInterval) {
    lastUpdate = millis();  // Update the timestamp
    
    // Decrease the countdown and print it to Serial Monitor
    if (COUNTDOWN > 0) {
      COUNTDOWN--;
      Serial.print("Total Seconds Remaining -> ");
      Serial.println(COUNTDOWN);
    } else {
      exit; 
    }
  }
}
