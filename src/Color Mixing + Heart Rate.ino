#include <U8g2lib.h>
#include <avr/pgmspace.h>
// #include <AltSoftSerial.h>
#include <SoftPWM.h>
#include <SoftwareSerial.h>
// #include "RunningAverage.h"

/* The following example demonstrates setting channels for all pins
   on the ATmega328P or ATmega168 used on Arduino Uno, Pro Mini,
   Nano and other boards. */
SOFTPWM_DEFINE_CHANNEL(0, DDRD, PORTD, PORTD0);  //Arduino pin 0
SOFTPWM_DEFINE_CHANNEL(1, DDRD, PORTD, PORTD1);  //Arduino pin 1
SOFTPWM_DEFINE_CHANNEL(2, DDRD, PORTD, PORTD2);  //Arduino pin 2
SOFTPWM_DEFINE_CHANNEL(3, DDRD, PORTD, PORTD3);  //Arduino pin 3
SOFTPWM_DEFINE_CHANNEL(4, DDRD, PORTD, PORTD4);  //Arduino pin 4
SOFTPWM_DEFINE_CHANNEL(5, DDRD, PORTD, PORTD5);  //Arduino pin 5
SOFTPWM_DEFINE_CHANNEL(6, DDRD, PORTD, PORTD6);  //Arduino pin 6
SOFTPWM_DEFINE_CHANNEL(7, DDRD, PORTD, PORTD7);  //Arduino pin 7
// SOFTPWM_DEFINE_CHANNEL(8, DDRB, PORTB, PORTB0);  //Arduino pin 8
// SOFTPWM_DEFINE_CHANNEL(9, DDRB, PORTB, PORTB1);  //Arduino pin 9
SOFTPWM_DEFINE_CHANNEL(10, DDRB, PORTB, PORTB2);  //Arduino pin 10
SOFTPWM_DEFINE_CHANNEL(11, DDRB, PORTB, PORTB3);  //Arduino pin 11
SOFTPWM_DEFINE_CHANNEL(12, DDRB, PORTB, PORTB4);  //Arduino pin 12
SOFTPWM_DEFINE_CHANNEL(13, DDRB, PORTB, PORTB5);  //Arduino pin 13
SOFTPWM_DEFINE_CHANNEL(14, DDRC, PORTC, PORTC0);  //Arduino pin A0
SOFTPWM_DEFINE_CHANNEL(15, DDRC, PORTC, PORTC1);  //Arduino pin A1
SOFTPWM_DEFINE_CHANNEL(16, DDRC, PORTC, PORTC2);  //Arduino pin A2
SOFTPWM_DEFINE_CHANNEL(17, DDRC, PORTC, PORTC3);  //Arduino pin A3
SOFTPWM_DEFINE_CHANNEL(18, DDRC, PORTC, PORTC4);  //Arduino pin A4
SOFTPWM_DEFINE_CHANNEL(19, DDRC, PORTC, PORTC5); //Arduino pin A5


SoftwareSerial BlueBee(8,9); // permanently pins 8,9

/* Here you make an instance of desired channel counts you want
   with the default 256 PWM levels (0 ~ 255). */
SOFTPWM_DEFINE_OBJECT(20);

const int numReadings = 1;
// const long interval = 5000;            // timeout for dust sensor
// const long automatic_interval = 6000; // timeout for automatic mode

byte data[6];   // for incoming serial data
byte readbuffer[2];
int rb = 0;
int heartRate = 0;
int heartRateIndex[numReadings];          // the readings from the analog input
int readIndex = 0;                        // the index of the current reading
int total = 0;                            // the running total
int averageHeartRate = 0;                 // the average
unsigned long previousMillis = 0;         // will store last time LED was updated
unsigned long currentMillis = 0;
int i = 0, j = 0, k = 0;
int step = 50;

// pin declarations
int DeepBlue  =   2;
int Blue      =   3;
int Green     =   4;
int Lime      =   5;
int Yellow    =   6;
int Amber     =   7;
int Red       =   10;
int White1    =   11;
int White2    =   12;

// everything else
int colorcontrolMode = 1;

void setup() {
  BlueBee.begin(9600); // set baudrate according to BlueBee 4.0 baudrate
  BlueBee.println("BlueBee ready");
  Serial.begin(9600);

  Palatis::SoftPWM.begin(120);
  resetColors();
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    heartRateIndex[thisReading] = 0; // initialize all the readings to 0:
  }
}

void loop(){
  readBluetooth();
  ColorControl();
  // ReceiveHeartData
}

void ColorControl(){
  while (readbuffer[0] == 170) {
    Serial.print(readbuffer[0]);
    Serial.print(", ");
    Serial.println(readbuffer[1]);
    i = readbuffer[1];
    for (int j = 0; j < 2; j++) {
      readbuffer[j] = 0;
      delay(1);
    }
    setColor(DeepBlue, i);
    i = 0;
    break;
  }
  while (readbuffer[0] == 187) {
    Serial.print(readbuffer[0]);
    Serial.print(", ");
    Serial.println(readbuffer[1]);
    i = readbuffer[1];
    for (int j = 0; j < 2; j++) {
      readbuffer[j] = 0;
      delay(1);
    }
    setColor(Blue, i);
    i = 0;
    break;
  }
  while (readbuffer[0] == 204) {
    Serial.print(readbuffer[0]);
    Serial.print(", ");
    Serial.println(readbuffer[1]);
    i = readbuffer[1];
    for (int j = 0; j < 2; j++) {
      readbuffer[j] = 0;
      delay(1);
    }
    setColor(Green, i);
    i = 0;
    break;
  }
  while (readbuffer[0] == 221) {
    Serial.print(readbuffer[0]);
    Serial.print(", ");
    Serial.println(readbuffer[1]);
    i = readbuffer[1];
    for (int j = 0; j < 2; j++) {
      readbuffer[j] = 0;
      delay(1);
    }
    setColor(Lime, i);
    i = 0;
    break;
  }
  while (readbuffer[0] == 238) {
    Serial.print(readbuffer[0]);
    Serial.print(", ");
    Serial.println(readbuffer[1]);
    i = readbuffer[1];
    for (int j = 0; j < 2; j++) {
      readbuffer[j] = 0;
      delay(1);
    }
    setColor(Yellow, i);
    i = 0;
    break;
  }
  while (readbuffer[0] == 255) {
    Serial.print(readbuffer[0]);
    Serial.print(", ");
    Serial.println(readbuffer[1]);
    i = readbuffer[1];
    for (int j = 0; j < 2; j++) {
      readbuffer[j] = 0;
      delay(1);
    }
    setColor(Amber, i);
    i = 0;
    break;
  }
  while (readbuffer[0] == 160) {
    Serial.print(readbuffer[0]);
    Serial.print(", ");
    Serial.println(readbuffer[1]);
    i = readbuffer[1];
    for (int j = 0; j < 2; j++) {
      readbuffer[j] = 0;
      delay(1);
    }
    setColor(Red, i);
    i = 0;
    break;
  }
  while (readbuffer[0] == 176) {
    Serial.print(readbuffer[0]);
    Serial.print(", ");
    Serial.println(readbuffer[1]);
    i = readbuffer[1];
    for (int j = 0; j < 2; j++) {
      readbuffer[j] = 0;
      delay(1);
    }
    setColor(White1, i);
    i = 0;
    break;
  }
  while (readbuffer[0] == 192) {
    Serial.print(readbuffer[0]);
    Serial.print(", ");
    Serial.println(readbuffer[1]);
    i = readbuffer[1];
    for (int j = 0; j < 2; j++) {
      readbuffer[j] = 0;
      delay(1);
    }
    setColor(White2, i);
    i = 0;
    break;
  }
}

void readBluetooth(){
  while (BlueBee.available() > 0) {
    int sb = BlueBee.read();
    delay(10);
    readbuffer[rb] = sb;
    rb++;
    if (rb == 2) {
      rb = 0;
      break;
    }
  }
}

void setColor(int _Color, int _PWM){
  if (_PWM < 0){
    _PWM = 0;
  }
  if (_PWM > 255){
    _PWM = 255;
  }
  Palatis::SoftPWM.set(_Color, _PWM);
}

void resetColors(){
  setColor(DeepBlue, 0);
  setColor(Blue, 0);
  setColor(Green, 0);
  setColor(Lime, 0);
  setColor(Yellow, 0);
  setColor(Amber, 0);
  setColor(Red, 0);
  setColor(White1, 0);
  setColor(White2, 0);
}

void ReceiveHeartData(){
  while (BlueBee.available() > 0) {
    previousMillis = currentMillis;
    heartRate = BlueBee.read(); // read the incoming byte:
    BlueBee.print("Heart-rate: ");
    BlueBee.println(heartRate);
    total = total - heartRateIndex[readIndex]; // subtract the last reading:
    heartRateIndex[readIndex] = heartRate; // read from ReceiveBluetoothData()
    total = total + heartRateIndex[readIndex]; // add the reading to the total:
    readIndex = readIndex + 1; // advance to the next position in the array:
    if (readIndex >= numReadings) {   // if we're at the end of the array...
    averageHeartRate = total / numReadings;   // calculate the average:
    BlueBee.print("Average heart-rate: ");
    BlueBee.println(averageHeartRate);
    BlueBee.println("*****************************");
    readIndex = 0;     // ...wrap around to the beginning:
    }
  delay(1);        // delay in between reads for stability
  break;
  }
  // heart rate range from 50 ---> 135
  // if(averageHeartRate < 60){averageHeartRate = 60;}
  // if(averageHeartRate > 100){averageHeartRate = 100;}
  // j = (averageHeartRate - 60) * 6;
  // k = (100 - averageHeartRate) * 6;
  // analogWrite(warmLED, j);
  // analogWrite(coolLED, k);
  // delay(1);
}
