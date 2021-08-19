#include "UnoJoy.h"

typedef void (* CallbackArray)(int);

const byte GreenPin = 2;
const byte RedPin = 3;
const byte YellowPin = 4;
const byte BluePin = 5;
const byte OrangePin = 6;

const byte DownPin = 7;
const byte UpPin = 8;

const byte SelectPin = 12;
const byte StartPin = 13;

const byte RedLed = 9;
const byte GreenLed = 10;
const byte BlueLed = 11;

const byte frets[] = {GreenPin, RedPin, YellowPin, BluePin, OrangePin};
const byte inputs[] = {UpPin, DownPin, SelectPin, StartPin};

byte buttons[sizeof(frets) + sizeof(inputs)];

const byte outputPins[3] = {RedLed, GreenLed, BlueLed};

void setup(){
  mergeInputs();
  setupInputs();
  setupLeds();
  setupUnoJoy();
}

void mergeInputs() {
  memcpy(buttons, frets, sizeof(frets));
  memcpy(&buttons[sizeof(frets)], inputs, sizeof(inputs));
}

void setupInputs(void){
  for (int i = 0; i < sizeof(buttons); i++){
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }

  pinMode(A0, INPUT);
  digitalWrite(A0, HIGH);
}

void setupLeds() {
  for (int i = 0; i < sizeof(outputPins); i++){
    pinMode(outputPins[i], OUTPUT);
  }
}

void loop(){
  dataForController_t controllerData = getControllerData();
  setControllerData(controllerData);
  setLeds();
}

void setLeds() {
  CallbackArray callbacks[] = {
    lightGreen,
    lightRed,
    lightYellow,
    lightBlue,
    lightOrange
  };

  for (int pin = sizeof(frets) - 1; pin >= 0; pin--){
    if (digitalRead(frets[pin]) == LOW) {
      callbacks[pin](NULL);
      return;
    }
  }

  lightWhite();
}

void lightGreen() {
  digitalWrite(RedLed, LOW);
  digitalWrite(GreenLed, HIGH);
  digitalWrite(BlueLed, LOW);
}

void lightRed() {
  digitalWrite(RedLed, HIGH);
  digitalWrite(GreenLed, LOW);
  digitalWrite(BlueLed, LOW);
}

void lightYellow() {
  analogWrite(RedLed, 200);
  analogWrite(GreenLed, 255);
  digitalWrite(BlueLed, LOW);
}

void lightBlue() {
  digitalWrite(RedLed, LOW);
  digitalWrite(GreenLed, LOW);
  digitalWrite(BlueLed, HIGH);
}

void lightOrange() {
  analogWrite(RedLed, 240);
  analogWrite(GreenLed, 255);
  digitalWrite(BlueLed, LOW);
}

void lightWhite() {
  analogWrite(RedLed, 127);
  analogWrite(GreenLed, 255);
  analogWrite(BlueLed, 255);
}

dataForController_t getControllerData(void){
  dataForController_t controllerData = getBlankDataForController();

  controllerData.squareOn = !digitalRead(GreenPin);
  controllerData.crossOn = !digitalRead(RedPin);
  controllerData.circleOn = !digitalRead(YellowPin);
  controllerData.triangleOn = !digitalRead(BluePin);
  controllerData.l1On = !digitalRead(OrangePin);

  controllerData.dpadUpOn = !digitalRead(UpPin);
  controllerData.dpadDownOn = !digitalRead(DownPin);

  controllerData.selectOn = !digitalRead(SelectPin);
  controllerData.startOn = !digitalRead(StartPin);

  controllerData.leftStickX = analogRead(A0) >> 2;

  return controllerData;
}
