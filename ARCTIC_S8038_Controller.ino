/*

________________________________________________________

Script for controlling 4X ARCTIC S8038 - 10K Fans
Made by Questonblock (PlanetHazel)
Revision: 0
Version 0.6
________________________________________________________
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int pushbuttoninput = 8;
const int setFanspeed = A0;
/*
const int sensorFanspeed1 = A1;
const int sensorFanspeed2 = A2;
const int sensorFanspeed3 = A3;
const int sensorFanspeed4; //add A4 maybe
*/

int pulseCount1;
int pulseCount2;
int pulseCount3;
int pulseCount4;

const int FanPWM = 9;
unsigned int fanspeed;
unsigned int fanspeedPercentage;
unsigned int fanspeedPercentageOld;
unsigned int pushbuttonOld = 1;

int fanRPM1;
int fanRPM2;
int fanRPM3;
int fanRPM4;

int setScreen = 1;
int pushbutton;

void setup() {

  //Inputs
  pinMode(pushbuttoninput, INPUT_PULLUP);
  pinMode(setFanspeed, INPUT);

  //Outputs
  pinMode(FanPWM, OUTPUT);

  Timer1.initialize(40); // 40 µs period = 25 kHz (1 / 0.00004)
  Timer1.pwm(FanPWM, 0);      // Start PWM at 0% duty

  lcd.init();
  Serial.begin(115200);
  lcd.backlight();
  lcd.clear();
  lcd.print("S8038 Controller");
  delay(2000);
  lcd.clear();

}

void loop() {

  pushbutton = digitalRead(pushbuttoninput);
  if(pushbutton < pushbuttonOld) { //Edge detection
    setScreen = setScreen + 1; //increment screen variable
  }

  if(setScreen == 1) {
    screen0();
  } else if(setScreen == 2) {
    screen1();
  } else if(setScreen == 3) {
    screen2();
  }

  Serial.println(setScreen);
  pushbuttonOld = pushbutton;
  if(setScreen == 4) {
    setScreen = 1;
  }

  fanspeed = analogRead(setFanspeed);          
  Timer1.setPwmDuty(FanPWM, fanspeed);  //Since Analog input is 1023 and TimerOne uses 1023 it can just be used directly

  fanspeedPercentage = map(fanspeed, 0, 1023, 0, 100); //map to display in percentage

  if(fanspeedPercentage == 0){ //newly added
    Timer1.setPwmDuty(FanPWM, 0);
  }

  delay(100);
}



void screen0() {                //Fanspeed screen
  if(fanspeedPercentage != fanspeedPercentageOld) {
    lcd.clear();
  }
  fanspeedPercentageOld = fanspeedPercentage;
  lcd.setCursor(1, 0);
  lcd.print("Fan Speed: ");
  lcd.print(fanspeedPercentage);
  lcd.print("%");
//cursed if thingya incoming
  //lcd.setCursor(1, 1);
  //lcd.print("placeholdr");
}

void screen1(){                 //RPM screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("F1:");
  lcd.print(fanRPM1);
  lcd.print(" ");
  lcd.print("F2:");
  lcd.print(fanRPM2);
  lcd.setCursor(0, 1);
  lcd.print("F3:");
  lcd.print(fanRPM3);
  lcd.print(" ");
  lcd.print("F4:");
  lcd.print(fanRPM4);
}

void screen2(){                //About screen
  lcd.setCursor(0, 0);
  lcd.print("By: T.F");
  lcd.setCursor(0, 1);
  lcd.print("Version: 0.5");
}