const int pirPin = 2;  /* PIR sensor O/P pin */
const int ledPin1 = 13;
const int ledPin2 = 12;
const int ledPin3 = 11;
const int ledPin4 = 10;
int warm_up;
unsigned long previousMillis = 0;
const long interval = 30000;  // interval at which to read the sensor (in milliseconds)

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  Serial.begin(9600); /* Define baud rate for serial communication */
}

void loop() {
  unsigned long currentMillis = millis();  // get the current time

  if (currentMillis - previousMillis >= interval) {  // if it's time to read the sensor
      previousMillis = currentMillis;  // save the current time
      int pirState = digitalRead(pirPin);  // read the state of the PIR sensor
  
      if (pirState == LOW) {
        Serial.print("Warming Up\n\n");
        warm_up = 0;
        digitalWrite(ledPin1, LOW);
        digitalWrite(ledPin2, LOW);
        digitalWrite(ledPin3, LOW);
        digitalWrite(ledPin4, LOW);
      }
      else {
        Serial.print("Object detected\n\n");    
        digitalWrite(ledPin1, HIGH);
        digitalWrite(ledPin2, HIGH);
        digitalWrite(ledPin3, HIGH);
        digitalWrite(ledPin4, HIGH);
      }
    }
}
