const int ledPin1 = 13;
const int ledPin2 = 11;
const int fanPin = 12;
const int acPin = 10;

int warm_up;

bool manualMode = false;
bool ledOn = false;
unsigned long previousMillis = 0;
unsigned long lastCommandMillis = 0;
const long interval = 5000;
const long manualTimeout = 30000; // 10 minutes in milliseconds

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
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    digitalWrite(ledPin1, !digitalRead(ledPin1));
    digitalWrite(ledPin2, !digitalRead(ledPin2));
    digitalWrite(acPin, !digitalRead(ledPin1));
    digitalWrite(fanPin, !digitalRead(ledPin1));// toggle all switches
    Serial.println("Toggling Switches");
  }
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
    }
  } else {
    lastCommandMillis = currentMillis; // reset timer when no command received
  }
}
