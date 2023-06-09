#include <RTClib.h>

const int ledPin1 = 11;
const int ledPin2 = 12;
const int fanPin = 13;
const int acPin = 10;

int warm_up;

bool manualMode = false;
bool ledOn = false;
unsigned long previousMillis = 0;
unsigned long lastCommandMillis = 0;
const long interval = 30000;
const long manualTimeout = 30000; // 30 seconds in milliseconds

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(acPin, OUTPUT);
  manualMode = false; // set to automatic on startup

}

void loop() {
  if (manualMode) {
    manualControl();
  } else {
    automaticControl();
  }
}

void manualControl() {
  unsigned long currentMillis = millis();
  if (Serial.available() > 0) {
    lastCommandMillis = currentMillis; // update timer when command received
    char receivedChar = Serial.read();
    Serial.println(receivedChar);
    switch (receivedChar) {
      case 'A':
        digitalWrite(ledPin1, !digitalRead(ledPin1));
        Serial.println("Toggling LED 1");
        break;
      case 'B':
        digitalWrite(ledPin2, !digitalRead(ledPin2));
        Serial.println("Toggling LED 2");
        break;
      case 'C':
        digitalWrite(fanPin, !digitalRead(fanPin));
        Serial.println("Toggling Fan");
        break;
      case 'D':
        digitalWrite(acPin, !digitalRead(acPin));
        Serial.println("Toggling AC");
        break;
      case 'M':
        manualMode = false;
        Serial.println("Switching to Automatic Mode");
        break;
      default:
        break;
    }
  } else {
    // check if timeout has been reached
    if (currentMillis - lastCommandMillis >= manualTimeout) {
      manualMode = false;
      Serial.println("Automatic Mode (timeout)");
    }
  }
}

void automaticControl() {

  unsigned long currentMillis = millis();

  // check if interval has passed since last switch toggle
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (rtcModule() == 1) {
      digitalWrite(ledPin1, !digitalRead(ledPin1));
      digitalWrite(acPin, digitalRead(ledPin1));
      digitalWrite(fanPin, digitalRead(ledPin1));
      digitalWrite(ledPin2, !digitalRead(ledPin1));// toggle all switches
      Serial.println("Toggling Switches");
    }
  }

  // check for manual mode switch command
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();
    Serial.println(receivedChar);
    if (receivedChar == 'M') {
      manualMode = true;
      Serial.println("Switching to Manual Mode");
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(acPin, LOW);
      digitalWrite(fanPin, LOW);
      lastCommandMillis = currentMillis;
    }
  } else {
    lastCommandMillis = currentMillis; // reset timer when no command received
  }
}






int rtcModule() {
  RTC_DS1307 rtc;
  
  rtc.begin();
  // Set the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  DateTime now = rtc.now();
  int seconds = now.second();
  int value;
  if (seconds % 10 == 0) {
    value = 1; // read the value of the RTC module
  }
  return value;
  delay(1000); // wait for 1 second before checking again
}
