 #include "receiver.h"
#include "SevSeg.h"
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define SPEAKER 3
const int CHANNEL = 0;
char msg[4];
long num;
RF24 radio(7,8);
const uint64_t pipe_num = 0xE8E8F0F0E1LL;

SevSeg sevseg;

void setup() {
  //radio setup
   radio.begin();
   radio.setChannel(2);
   radio.setPayloadSize(7);
   radio.setDataRate(RF24_250KBPS);
   radio.openReadingPipe(1,pipe_num);
   radio.startListening();
  
  //7-segment display setup
  byte numDigits = 4;
  byte digitPins[] = {15, 16, 17, 18};
  byte segmentPins[] = {14, 2, 19, 5, 6, 10, 9, 4};
  bool resistorsOnSegments = true; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_CATHODE; // See README.md for options
  bool updateWithDelays = false; // Default 'false' is Recommended
  bool leadingZeros = true; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected. Then, you only need to specify 7 segmentPins[]

  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments,
  updateWithDelays, leadingZeros, disableDecPoint);

  //Speaker setup
  pinMode(SPEAKER, OUTPUT);
  Serial.begin(115200);
}

void loop() {
    if (radio.available()){  
       radio.read(msg, 4); 
       Serial.println(msg);
       if (msg[0] == 'O'){
          tone(SPEAKER, 440);
          Serial.println(msg[0]);
       }
       else if (msg[0] == 'F'){
          noTone(SPEAKER);
       }
       else {
         num = 1000*msg[0] + 100*msg[1] + 10*msg[2] + msg[3];
         Serial.println(num);
         delay(10);
         sevseg.setNumber(num,2);
         sevseg.refreshDisplay();
       }
    }
   else{
    Serial.println("No radio available");
    sevseg.refreshDisplay();
   }

  
}
