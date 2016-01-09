/*
* Pseudo randomly timed relay
* --
* Developed for Adafruit Trinket (ATTiny85) so it should work 
* also with bigger boards
* 
* Author: Eduard Roccatello (eduard <at> xfce <dot>org)
*
*/
#define MILLIS_TO_MINS 60 * 1000;

// Edit PINS
const int BUTTON = 0;
const int LED = 1;
const int RELAY = 2;

// Edit MINUTES 
int minOnTime = 20;
int maxOnTime = 60;
int minOffTime = 5;
int maxOffTime = 15;
// -- Stop editing here

bool active = false;
bool relayIsOn = false;
unsigned long previousTime = 0;
unsigned long timeToWait = 0;

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setRelay(bool status) {
  if (status == true) {
    digitalWrite(RELAY, LOW);
  } else {
    digitalWrite(RELAY, HIGH); 
  }
  relayIsOn = status;
}

void setup() {
  setRelay(false);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(RELAY, OUTPUT);
  
  previousTime = millis();
}
 
void loop() {
  bool hasToStart = false;
  unsigned long curTime = millis();
  
  // read input
  int reading = digitalRead(BUTTON);
  if (reading != lastButtonState) {
    lastDebounceTime = curTime;
  }
  
  if ((curTime - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      
      if (buttonState == LOW) {
        active = !active;
        if (active) {
          hasToStart = true;
        }
      }
    }
  }
  
  if (active) {
    if (hasToStart) {
      previousTime = curTime;
      timeToWait = random(minOnTime, maxOnTime) * MILLIS_TO_MINS;
      setRelay(true);
    }

    if (curTime - previousTime > timeToWait) {
      if (relayIsOn) {
        timeToWait = random(minOffTime, maxOffTime) * MILLIS_TO_MINS;
        setRelay(false);          
      } else {
        timeToWait = random(minOnTime, maxOnTime) * MILLIS_TO_MINS;
        setRelay(true);
      }
      previousTime = curTime;
    }
  } else {
    if (relayIsOn) {
      setRelay(false);
    }
  }
  
  lastButtonState = reading;
}
