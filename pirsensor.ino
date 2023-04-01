const int pirPin = 2;  /* PIR sensor O/P pin */
const int ledPin1 = 13;
const int fanPin3 = 11;
const int acPin4 = 10;
int warm_up;
unsigned long previousMillis = 0;
const long interval = 30000;  // interval at which to read the sensor (in milliseconds)

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(fanPin3, OUTPUT);
  pinMode(acPin4, OUTPUT);
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
        digitalWrite(fanPin3, LOW);
        digitalWrite(acPin4, LOW);
      }
      else {
        Serial.print("Object detected\n\n");    
        digitalWrite(ledPin1, HIGH);
        digitalWrite(fanPin3, HIGH);
        digitalWrite(acPin4, HIGH);
      }
    }
}
