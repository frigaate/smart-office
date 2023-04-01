#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>
#include <RTClib.h>

// Replace with your network credentials
const char* ssid = "******";
const char* password = "******";

// Replace with your Telegram Bot token (given to you by BotFather)
const char* token = "Your Bot Token here";

// Initialize WiFi client object
WiFiClientSecure client;

// Initialize Telegram bot object
UniversalTelegramBot bot(token, client);
RTC_DS1307 rtc;

// Define LED pins
const int LED_PIN_1 = 5;
const int LED_PIN_2 = 4;
const int LED_PIN_3 = 0;
const int LED_PIN_4 = 2;
const int pinPIR = 14;
int pirState = 0; 
int mode = 1;
bool led1_state = LOW;
bool led2_state = LOW;
bool led3_state = LOW;
bool led4_state = LOW;

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi!");

  // Set up secure client connection
  client.setInsecure();
  client.setTimeout(5000);

  // Set LED pins as outputs
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);
}

void telegram() {
  // Check for incoming messages
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

 

    while (numNewMessages) {
      Serial.println("New message received:");

      // Print the message text
      for (int i=0; i<numNewMessages; i++) {
        String chat_id = String(bot.messages[i].chat_id);
        String text = bot.messages[i].text;
        Serial.println("Chat ID: " + chat_id);
        Serial.println("Message text: " + text);

      if (text == "/mode0") {
          mode = 0;
          bot.sendMessage(chat_id, "Mode is now Manual");
      }
      else if (text == "/mode1") {
          mode = 1;
          bot.sendMessage(chat_id, "Mode is now Automatic");
          break;
      }

        // Control LEDs based on message text
     while(mode ==0){
        if (text == "/led1_on") {
          if(led1_state != HIGH){
            digitalWrite(LED_PIN_1, HIGH);
            led1_state = HIGH;
            bot.sendMessage(chat_id, "LED 1 is now on");
            Serial.println("lED 1 is now on");
          }
        } else if (text == "/led1_off") {
          if(led1_state != LOW){
            digitalWrite(LED_PIN_1, LOW);
            led1_state = LOW;
            bot.sendMessage(chat_id, "LED 1 is now off");
            Serial.println("lED 1 is now off");
          }
        } else if (text == "/led2_on") {
          if(led2_state != HIGH){
            digitalWrite(LED_PIN_2, HIGH);
            led2_state = HIGH;
            bot.sendMessage(chat_id, "LED 2 is now on");
          }
        } else if (text == "/led2_off") {
          if(led2_state != LOW){
            digitalWrite(LED_PIN_2, LOW);
            led2_state = LOW;
            bot.sendMessage(chat_id, "LED 2 is now off");
          }
        } else if (text == "/led3_on") {
          if(led3_state != HIGH){
            digitalWrite(LED_PIN_3, HIGH);
            led3_state = HIGH;
            bot.sendMessage(chat_id, "LED 3 is now on");
          }
        } else if (text == "/led3_off") {
          if(led3_state != LOW){
            digitalWrite(LED_PIN_3, LOW);
            led3_state = LOW;
            bot.sendMessage(chat_id, "LED 3 is now off");
          }
        } else if (text == "/led4_on") {
          if(led4_state != HIGH){
            digitalWrite(LED_PIN_4, HIGH);
            led4_state = HIGH;
            bot.sendMessage(chat_id, "LED 4 is now on");
          }
        } else if (text == "/led4_off") {
          if(led4_state != LOW){
            digitalWrite(LED_PIN_4, LOW);
            led4_state = LOW;
            bot.sendMessage(chat_id, "LED 4 is now off");
          }
        }
     }
      }
    }
  }

 void loop() {
    

  if(mode == 0){

     telegram();
    
  }
  // automatic(default) mode
  else if (mode == 1) {
  // get current time
  DateTime now = rtc.now();
  //Check if it's weekday
  int dayOfWeek = now.dayOfTheWeek();


  if (dayOfWeek >= 1 && dayOfWeek <= 5) {
    // turn on LED at 8 am and turn off at 4 pm
      if (now.hour() >= 8 && now.hour() < 16) {
      digitalWrite(LED_PIN_1, HIGH);
      Serial.println("LED1 ON");
      digitalWrite(LED_PIN_2, LOW);
      Serial.println("LED2 OFF");
      digitalWrite(LED_PIN_3, HIGH);
      Serial.println("LED3 ON");
      digitalWrite(LED_PIN_4, HIGH);
      Serial.println("LED4 ON");
    }
    else {
      digitalWrite(LED_PIN_1, LOW);
      Serial.println("LED1 OFF");
      digitalWrite(LED_PIN_2, HIGH);
      Serial.println("LED2 ON");
      digitalWrite(LED_PIN_3, LOW);
      Serial.println("LED3 OFF");
      digitalWrite(LED_PIN_4, LOW);
      Serial.println("LED4 OFF");
    }
  
  }
  


  // PIR sensor
  pirState = digitalRead(pinPIR);
  int counter = 0;
// If there is movement, reset the counter and turn on the LEDs
  if (pirState == HIGH) {
  counter = 0;
    digitalWrite(LED_PIN_1, HIGH);
    Serial.println("LED1 ON");
    //digitalWrite(pinLED2, HIGH);
    //Serial.println("LED2 ON");
    digitalWrite(LED_PIN_3, HIGH);
    Serial.println("LED3 ON");
    digitalWrite(LED_PIN_4, HIGH);
    Serial.println("LED4 ON");
  }


  // If there is no movement, increment the counter
  else {
  counter++;
  }


  // If there has been no movement for 4 consecutive checks (4 * 5 minutes), turn off the LEDs
  if (counter >= 4) {
    digitalWrite(LED_PIN_1, LOW);
    Serial.println("LED1 OFF");
    //digitalWrite(pinLED2, LOW);
    //Serial.println("LED2 OFF");
    digitalWrite(LED_PIN_3, LOW);
    Serial.println("LED3 OFF");
    digitalWrite(LED_PIN_4, LOW);
    Serial.println("LED4 OFF");
  }


  // Wait 5 minutes before checking again
  delay(300000);
  }
}
