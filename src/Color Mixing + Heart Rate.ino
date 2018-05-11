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
int fadeSpeed = 7;
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
int DeepBlue  =   11; //7 //3 //
int Blue      =   4; //5 //2
int Green     =   7; //3 //11
int Lime      =   3; //11 //7
int Yellow    =   10; //10 //10
int Amber     =   2; //4 //5
int Red       =   6; //6 //6
int White1    =   5; //2 //4
int White2    =   12; //12

// everything else
int colorcontrolMode = 1;
int lastColor = 0;
int previousDB = 0, previousB = 0, previousG = 0, previousL = 0, previousY = 0, previousA = 0, previousR = 0, previousW1 = 0, previousW2 = 0;

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
  ReceiveHeartData();
}

void ColorControl(){
  while (readbuffer[0] == 10 && readbuffer[1] == 10) {
    Serial.print(readbuffer[0]);
    Serial.print(", ");
    Serial.println(readbuffer[1]);
    i = readbuffer[1];
    for (int j = 0; j < 2; j++) {
      readbuffer[j] = 0;
      delay(1);
    }
    resetColors();
    i = 0;
    rb = 0;
    break;
  }
  while (readbuffer[0] == 170) {
    Serial.print(readbuffer[0]);
    Serial.print(", ");
    Serial.println(readbuffer[1]);
    i = readbuffer[1];
    for (int j = 0; j < 2; j++) {
      readbuffer[j] = 0;
      delay(1);
    }
    setColor(DeepBlue, i, previousDB);
    previousDB = i;
    i = 0;
    rb = 0;
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
    setColor(Blue, i, previousB);
    previousB = i;
    i = 0;
    rb = 0;
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
    setColor(Green, i, previousG);
    previousG = i;
    i = 0;
    rb = 0;
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
    setColor(Lime, i, previousL);
    previousL = i;
    i = 0;
    rb = 0;
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
    setColor(Yellow, i, previousY);
    previousY = i;
    i = 0;
    rb = 0;
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
    setColor(Amber, i, previousA);
    previousA = i;
    i = 0;
    rb = 0;
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
    setColor(Red, i,  previousR);
    previousR = i;
    i = 0;
    rb = 0;
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
    setColor(White1, i, previousW1);
    previousW1 = i;
    i = 0;
    rb = 0;
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
    setColor(White2, i, previousW2);
    previousW2 = i;
    i = 0;
    rb = 0;
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
      Serial.print(readbuffer[0]);
      Serial.print(", ");
      Serial.println(readbuffer[1]);
      break;
    }
  }
}

void setColor(int _Color, int _value, int _previousValue){
  if (_value < 0){
    _value = 0;
  }
  if (_value > 255){
    _value = 255;
  }
  if(_value > _previousValue){
    for(int c = _previousValue; c <= _value; c++){
      Palatis::SoftPWM.set(_Color, c);
      delay(fadeSpeed);
    }
  }
  else if(_value < _previousValue){
    for(int c = _previousValue; c >= _value; c--){
      Palatis::SoftPWM.set(_Color, c);
      delay(fadeSpeed);
    }
  }
}

void resetColors(){
  Palatis::SoftPWM.set(DeepBlue, 0);
  Palatis::SoftPWM.set(Blue, 0);
  Palatis::SoftPWM.set(Green, 0);
  Palatis::SoftPWM.set(Lime, 0);
  Palatis::SoftPWM.set(Yellow, 0);
  Palatis::SoftPWM.set(Amber, 0);
  Palatis::SoftPWM.set(Red, 0);
  Palatis::SoftPWM.set(White1, 0);
  Palatis::SoftPWM.set(White2, 0);
  previousDB = 0;
  previousB = 0;
  previousG = 0;
  previousL = 0;
  previousY = 0;
  previousA = 0;
  previousR = 0;
  previousW1 = 0;
  previousW2 = 0;
  rb = 0;
}

void ReceiveHeartData(){
  while (readbuffer[0] == 100) {
    Serial.print(readbuffer[0]);
    Serial.print(", ");
    Serial.println(readbuffer[1]);
    i = readbuffer[1];
    for (int j = 0; j < 2; j++) {
      readbuffer[j] = 0;
      delay(1);
    }
    switch (i) {
      case 10:
      if(lastColor > 1){
        FadeInOut(DeepBlue, Blue, fadeSpeed);
      }
      else if(lastColor < 1){
        Palatis::SoftPWM.set(DeepBlue, 255);
      }
      else if(lastColor == 1){
        break;
      }
      lastColor = 1;
      Palatis::SoftPWM.set(Blue, 0);
      Palatis::SoftPWM.set(Green, 0);
      Palatis::SoftPWM.set(Lime, 0);
      Palatis::SoftPWM.set(Yellow, 0);
      Palatis::SoftPWM.set(Amber, 0);
      Palatis::SoftPWM.set(Red, 0);
      Palatis::SoftPWM.set(White1, 0);
      Palatis::SoftPWM.set(White2, 0);
      break;
      case 20:
      if(lastColor > 2){
        FadeInOut(Blue, Green, fadeSpeed);
      }
      else if(lastColor < 2){
        FadeInOut(Blue, DeepBlue, fadeSpeed);
      }
      else if(lastColor == 2){
        break;
      }
      lastColor = 2;
      Palatis::SoftPWM.set(DeepBlue, 0);
      Palatis::SoftPWM.set(Green, 0);
      Palatis::SoftPWM.set(Lime, 0);
      Palatis::SoftPWM.set(Yellow, 0);
      Palatis::SoftPWM.set(Amber, 0);
      Palatis::SoftPWM.set(Red, 0);
      Palatis::SoftPWM.set(White1, 0);
      Palatis::SoftPWM.set(White2, 0);
      break;
      case 30:
      if(lastColor > 3){
        FadeInOut(Green, Lime, fadeSpeed);
      }
      else if(lastColor < 3){
        FadeInOut(Green, Blue, fadeSpeed);
      }
      else if(lastColor == 3){
        break;
      }
      lastColor = 3;
      Palatis::SoftPWM.set(DeepBlue, 0);
      Palatis::SoftPWM.set(Blue, 0);
      Palatis::SoftPWM.set(Lime, 0);
      Palatis::SoftPWM.set(Yellow, 0);
      Palatis::SoftPWM.set(Amber, 0);
      Palatis::SoftPWM.set(Red, 0);
      Palatis::SoftPWM.set(White1, 0);
      Palatis::SoftPWM.set(White2, 0);
      break;
      case 40:
      if(lastColor > 4){
        FadeInOut(Lime, Yellow, fadeSpeed);
      }
      else if(lastColor < 4){
        FadeInOut(Lime, Green, fadeSpeed);
      }
      else if(lastColor == 4){
        break;
      }
      lastColor = 4;
      Palatis::SoftPWM.set(DeepBlue, 0);
      Palatis::SoftPWM.set(Blue, 0);
      Palatis::SoftPWM.set(Green, 0);
      Palatis::SoftPWM.set(Yellow, 0);
      Palatis::SoftPWM.set(Amber, 0);
      Palatis::SoftPWM.set(Red, 0);
      Palatis::SoftPWM.set(White1, 0);
      Palatis::SoftPWM.set(White2, 0);
      break;
      case 50:
      if(lastColor > 5){
        FadeInOut(Yellow, Amber, fadeSpeed);
      }
      else if(lastColor < 5){
        FadeInOut(Yellow, Lime, fadeSpeed);
      }
      else if(lastColor == 5){
        break;
      }
      lastColor = 5;
      Palatis::SoftPWM.set(DeepBlue, 0);
      Palatis::SoftPWM.set(Blue, 0);
      Palatis::SoftPWM.set(Green, 0);
      Palatis::SoftPWM.set(Lime, 0);
      Palatis::SoftPWM.set(Amber, 0);
      Palatis::SoftPWM.set(Red, 0);
      Palatis::SoftPWM.set(White1, 0);
      Palatis::SoftPWM.set(White2, 0);
      break;
      case 60:
      if(lastColor > 6){
        FadeInOut(Amber, Red, fadeSpeed);
      }
      else if(lastColor < 6){
        FadeInOut(Amber, Yellow, fadeSpeed);
      }
      else if(lastColor == 6){
        break;
      }
      lastColor = 6;
      Palatis::SoftPWM.set(DeepBlue, 0);
      Palatis::SoftPWM.set(Blue, 0);
      Palatis::SoftPWM.set(Green, 0);
      Palatis::SoftPWM.set(Lime, 0);
      Palatis::SoftPWM.set(Yellow, 0);
      Palatis::SoftPWM.set(Red, 0);
      Palatis::SoftPWM.set(White1, 0);
      Palatis::SoftPWM.set(White2, 0);
      break;
      case 70:
      if(lastColor > 7){
        Palatis::SoftPWM.set(Red, 255);
      }
      else if(lastColor < 7){
        FadeInOut(Red, Amber, fadeSpeed);
      }
      else if(lastColor == 7){
        break;
      }
      lastColor = 7;
      Palatis::SoftPWM.set(DeepBlue, 0);
      Palatis::SoftPWM.set(Blue, 0);
      Palatis::SoftPWM.set(Green, 0);
      Palatis::SoftPWM.set(Lime, 0);
      Palatis::SoftPWM.set(Yellow, 0);
      Palatis::SoftPWM.set(Amber, 0);
      Palatis::SoftPWM.set(White1, 0);
      Palatis::SoftPWM.set(White2, 0);
      break;
    }
    i = 0;
    rb = 0;
    break;
  }
}

void FadeInOut(int _colorIn, int _colorOut, int _speed){
  for(int k = 0; k <= 255; k++){
    if(k > 255) {k = 255;}
    if(k < 0)   {k = 0;}
    Palatis::SoftPWM.set(_colorIn, k);
    Palatis::SoftPWM.set(_colorOut, 255 - k);
    delay(_speed);
  }
}
