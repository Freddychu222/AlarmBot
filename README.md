# AlarmBot - the IoT wake up buddy
![](https://github.com/Freddychu222/AlarmBot/blob/main/alarmbot.PNG?raw=true)

### Fred Chu, Lucas Wolter

### [Website](https://freddychu222.github.io/AlarmBot/) | [Git Repo](https://github.com/Freddychu222/AlarmBot) | [DEMO](https://www.youtube.com/watch?v=BlgNk7RgsMY) | [Presentation](https://docs.google.com/presentation/d/1ObMXAebYAJUBGYW3-vbmgcVhR-EQM9kG7rVNcSX3umY/edit#slide=id.g4dfce81f19_0_45)

## Why AlarmBot
Waking up for classes in the middle of a pandemic can be a nightmare. With your bed so close to your zoom class, its harder now than ever to get up for that 8am lecture.
Worry no longer, your days of sleeping through class are over!
According to a recent study at Harvard, only 11% of college students sleep well. Sleep deprivation causes some pretty difficult mornings and dangerous potential to oversleep and miss morning classes. To remedy this problem, we developed Alarmbot, an IOT mobile alarm clock guaranteed to wake you up.

## Intro
AlarmBot, the IoT quick wake-up buddy.
AlarmBot is a practical wake up system that stops you from snoozing the morning away. AlarmBot features a stationary clock module, a bot module that rolls away from your bed when the alarm goes off, and a website to remotely set alarms and send messages. These features enable users to end that vicious snooze cycle and actually get out of bed, as well as allow parents or friends to set their forgetful buddy’s alarm remotely.

## Hardware Design
![](https://github.com/Freddychu222/AlarmBot/blob/main/img/8.PNG?raw=true)

From of the parts provided in the audiovisual box and base set, we utilized the esp32 microcontroller, OLED display, 7-segment display, audio transducer, and RGB LED in our design.

After fully utilizing the parts of the AV box, we used additional parts to make our vision become reality such as nRF radio modules, an Arduino nano, a speaker, 2 gear-motors, and an H-bridge.

The bot portion of our design is controlled by the ESP32 microcontroller and interfaces with an nRF radio using a modified SPI library in order to communicate with the clock module. The OLED display is connected via I2C, the H-bridge is used to control the motors, and the push button is paired with an RC debouncing circuit.

## Software Design:
![](https://github.com/Freddychu222/AlarmBot/blob/main/img/7.PNG?raw=true)

The code that runs on AlarmBot is under the respective Bot Module and Alarm Module folders in the Github repo. The Alarm module was connected to the Bot using two radio modules, implemented on the ESP32 with customized API libraries. By using the Firebase API, we connected the Bot module to a backend database,. This connection was done using the ESP-32 Wi-Fi connection API. The bot module was then connected to the database to a custom made website in HTML/CSS/js. The website is constantly updated by a live clock updated to the millisecond.

## Parts Used
### Bot Module:
* ESP32
* OLED Display
* Audio Piezo transducer
* RGB LED
* nRF24 Radio Module
* Gear-motor x2
* H-Bridge
* Tin Can

![](https://github.com/Freddychu222/AlarmBot/blob/main/img/9.PNG?raw=true)


### Clock Module:
* Arduino Nano
* Speaker
* nRF24 Radio Module
* 4 Digit 7-Segment Display
* Cardboard Box

![](https://github.com/Freddychu222/AlarmBot/blob/main/img/10.PNG?raw=true)

##Libraries Used
* [IOXhop_FirebaseESP32](https://github.com/ioxhop/IOXhop_FirebaseESP32)
* [MFRC522](https://github.com/miguelbalboa/rfid)
* [ArduinoJson](https://arduinojson.org/)
* Adafruit SSD1306 (for OLED)
* Adafruit GFX (for OLED)
* AdafruitBusIO
* Adafriut SSD1306
*ESP32 AnalogWrite
*[SevSeg] (https://www.arduino.cc/reference/en/libraries/sevseg/)
* pitches (labelled notes for ease of use with speaker)

![](https://github.com/Freddychu222/AlarmBot/blob/main/img/11.PNG?raw=true)

## Website
![](https://github.com/Freddychu222/AlarmBot/blob/main/img/5.PNG?raw=true)

Lastly, we made a simple webpage [simple website](https://freddychu222.github.io/AlarmBot/ using HTML, CSS, and Javascript to update the database remotely, as well as send messages to the OLED screen.  index.html calls the app0.js script to update the database, as well as the clock.js script to constantly refresh the clock.
