//Wifi Communication Setup
#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#define FIREBASE_HOST "tincan-bc739-default-rtdb.firebaseio.com"  
#define FIREBASE_AUTH "xyGuR8T253ievEZQwvUDj2wqSRINxo6vq7t2IUUU"
#define WIFI_SSID "SpectrumSetup-50"               
#define WIFI_PASSWORD "stronglight381"
String timeUpdate = "0000";
String msgUpdate = "hello";       
String oldTime = "a";
String oldMsg = "a"

// Radio Communication Setup
#include  "SPI.h"
#include "nRF24L01.h"
#include "RF24.h"
RF24 radio(12, 14, 26, 25, 27);
const uint64_t pipe_num = 0xE8E8F0F0E1LL;
char buf2[5];

// Motor Pins
#define LEFT_MOTOR_EN 5
#define LEFT_MOTOR_1 18
#define LEFT_MOTOR_2 19
#define LEFT_MOTOR_CHANNEL 1

#define RIGHT_MOTOR_EN 35
#define RIGHT_MOTOR_1 32
#define RIGHT_MOTOR_2 33
#define RIGHT_MOTOR_CHANNEL 2

// Speaker Setup
#define SPEAKER 13
#define SPEAKER_CHANNEL 0
#define RESOLUTION 8

// LED Pins
#define RED_LED 2
#define GREEN_LED 4

// OLED Setup
#include "Wire.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_ADDR       0x3C
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button Pin
#define BUTTON 34

// Time Variables
// time starts from 7:55
int hour = 10;
int minute = 59;
int second = 55;
long last_time_millis = 0;

// Speaker Variables
long last_speaker_millis = 0;
bool speaker_on = false;

// Button Variables
bool last_button_pressed = false;

// Alarm Variables
int alarm_hour;
int alarm_minute;
int alarm_second;
bool alarm_on = false;

// update the current time and 7-seg display every loop cycle
void update_time(long curr_millis) {
  if(curr_millis - last_time_millis >= 1000) {
    last_time_millis = curr_millis;
    second++;
    if(second == 60) {
      second = 0;
      minute++;
    }
    if(minute == 60) {
      minute = 0;
      hour++;
    }
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.print(":");
    Serial.println(second);
    // update 7-seg display
    byte converted_time[4] = {hour/10, hour%10, minute/10, minute%10};
    radio.write(converted_time, 4);
  }
}

// turn on alarm if alarm time is reached
void turn_on_alarm() {
  char buf[5];
  timeUpdate.toCharArray(buf, 5);
  int alarmset = atoi(buf);
  int hourUpdate = alarmset / 100;
  Serial.println(hourUpdate);
  int minUpdate= alarmset % 100;
  if(hour == hourUpdate && minute == minUpdate && second == 0) {
    buf2[0] = 'O';
    buf2[1] = 'O';
    buf2[2] = 'O';
    buf2[3] = 'O';
    alarm_on = true;
    digitalWrite(GREEN_LED, LOW);

    display.clearDisplay();
    display.setCursor(40, 0);
    display.setTextSize(9);
    display.write(0x01);
    display.display();
    
    digitalWrite(LEFT_MOTOR_1, LOW);
    digitalWrite(LEFT_MOTOR_2, HIGH);
    digitalWrite(RIGHT_MOTOR_1, LOW);
    digitalWrite(RIGHT_MOTOR_2, HIGH);
    radio.write(buf2,4);
    // change OLED face
    // change RGB color
  }
}

// turn on/off speaker every half second if alarm is on
void update_speaker(long curr_millis) {
  if(curr_millis - last_speaker_millis >= 500) {
    last_speaker_millis = curr_millis;
    if(speaker_on) {
      ledcWriteTone(SPEAKER_CHANNEL, 0);
      digitalWrite(RED_LED, LOW);
      speaker_on = false;
    }
    else {
      ledcWriteTone(SPEAKER_CHANNEL, 440);
      digitalWrite(RED_LED, HIGH);
      speaker_on = true;
    }
  }
}

// turn off alarm when button is pressed
void turn_off_alarm(bool button_pressed) {
  if(!button_pressed && last_button_pressed) {
    alarm_on = false;
    ledcWriteTone(SPEAKER_CHANNEL, 0);
    buf2[0] = 'F';
    buf2[1] = 'F';
    buf2[2] = 'F';
    buf2[3] = 'F';
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);

    display.clearDisplay();
    display.setCursor(40, 0);
    display.setTextSize(9);
    display.write(0x02);
    display.display();

    digitalWrite(LEFT_MOTOR_1, LOW);
    digitalWrite(LEFT_MOTOR_2, LOW);
    digitalWrite(RIGHT_MOTOR_1, LOW);
    digitalWrite(RIGHT_MOTOR_2, LOW);
    
  }
  last_button_pressed = button_pressed;
}

void setup() {
  Serial.begin(115200);

  //Connect to Wifi
  delay(1000);
  pinMode(2, OUTPUT);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                  
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  //testing connection
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());  //print local IP address

  //Connecting to Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setString("LED_STATUS", "OFF");  
  // Radio Setup
  radio.begin();
  radio.setChannel(2);
  radio.setPayloadSize(7);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipe_num);

  // motor setup
  pinMode(LEFT_MOTOR_EN, OUTPUT);
  pinMode(LEFT_MOTOR_1, OUTPUT);
  pinMode(LEFT_MOTOR_2, OUTPUT);
  ledcSetup(LEFT_MOTOR_CHANNEL, 30000, RESOLUTION);
  ledcAttachPin(LEFT_MOTOR_EN, LEFT_MOTOR_CHANNEL);

  pinMode(RIGHT_MOTOR_EN, OUTPUT);
  pinMode(RIGHT_MOTOR_1, OUTPUT);
  pinMode(RIGHT_MOTOR_2, OUTPUT);
  ledcSetup(RIGHT_MOTOR_CHANNEL, 30000, RESOLUTION);
  ledcAttachPin(RIGHT_MOTOR_EN, RIGHT_MOTOR_CHANNEL);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUTTON, INPUT);

  // speaker setup
  ledcSetup(SPEAKER_CHANNEL, 2000, RESOLUTION);
  ledcAttachPin(SPEAKER, SPEAKER_CHANNEL);

  // OLED setup
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.setTextSize(9);
  display.setTextColor(WHITE);
  
  display.clearDisplay();
  display.setCursor(40, 0);
  display.write(0x02);
  display.display();

  digitalWrite(GREEN_LED, HIGH);
  
  // set alarm time
  alarm_hour = 11;
  alarm_minute = 0;
  alarm_second = 0;
}

void loop() {
  //Check website for new updates on the values
  timeUpdate = Firebase.getString("Time/Time");
  msgUpdate = Firebase.getString("Message/Message");
  Serial.println(msgUpdate);
  Serial.println(timeUpdate);
  
  update_time(millis());

  turn_on_alarm();

  if(alarm_on) {
    update_speaker(millis());

    int random_val = millis() % 10;
    if(random_val == 9) {
      digitalWrite(LEFT_MOTOR_1, LOW);
      digitalWrite(LEFT_MOTOR_2, HIGH);
      digitalWrite(RIGHT_MOTOR_1, HIGH);
      digitalWrite(RIGHT_MOTOR_2, LOW);
      ledcWrite(LEFT_MOTOR_CHANNEL, 127);
      ledcWrite(RIGHT_MOTOR_CHANNEL, 127);
    }else if(random_val == 8) {
      digitalWrite(LEFT_MOTOR_1, HIGH);
      digitalWrite(LEFT_MOTOR_2, LOW);
      digitalWrite(RIGHT_MOTOR_1, LOW);
      digitalWrite(RIGHT_MOTOR_2, HIGH);
      ledcWrite(LEFT_MOTOR_CHANNEL, 127);
      ledcWrite(RIGHT_MOTOR_CHANNEL, 127);
    }else {
      digitalWrite(LEFT_MOTOR_1, LOW);
      digitalWrite(LEFT_MOTOR_2, HIGH);
      digitalWrite(RIGHT_MOTOR_1, LOW);
      digitalWrite(RIGHT_MOTOR_2, HIGH);
      ledcWrite(LEFT_MOTOR_CHANNEL, 255);
      ledcWrite(RIGHT_MOTOR_CHANNEL, 255);
    }
    
    turn_off_alarm(digitalRead(BUTTON));
  }else {
    //check for messages to write to display
    if (msgUpdate != oldMsg){
      display.clearDisplay();
      display.setCursor(3, 0);
      display.setTextSize(2);
      display.println(msgUpdate);
      display.display();
      oldMsg = msgUpdate;
    }
  }
  Serial.println(buf2);
  radio.write(buf2, 4);
}
