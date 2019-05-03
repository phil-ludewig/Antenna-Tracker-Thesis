#include <Arduino.h>
#include <TinyGPS.h>
#include <Wire.h>
#include "SFE_HMC6343.h" //Magnetometer library
#include <SparkFunAutoDriver.h>
#include <SPI.h>

//Objects
SFE_HMC6343 magnetometer;
TinyGPS gpsSensor;

AutoDriver boardA(47, 53);
AutoDriver boardB(45, 53);

#define LogicConverterLow 46 //Low voltage reference pin for logic level converter (Magnetometer)
#define buzzerPin 44
#define userSwitch 13
#define batteryPin 0

#define eleStepsPerRev 200  //fixed motor specific
#define eleGearRatio 33.075 //change this according to mechanical ratio
#define eleMicrostepping 128 //set in setupSteppers() function

#define aziStepsPerRev 200
#define aziGearRatio 8

#define minimumUAVDistance 15 //tracking below XX meters could make tracker positions jump due to GPS inaccuracies

unsigned long previousGpsTimestamp = 0;
unsigned long currentGpsTimestamp;

unsigned long previousBatteryAlarmTimestamp = 0;
unsigned long currentBatteryAlarmTimestamp;
float batteryVoltage = 0;
int iBatteryVoltage = 0;

unsigned long previousBatteryPrintTime = 0;
unsigned long currentBatteryPrintTime;

float UAVLatitude = 0;
float UAVLongitude = 0;
float UAVAltitude = 0;

float homeLatitude      = 0;
float homeLongitude     = 0;
float homeAltitude      = 0;

byte gpsString;

//copy back to Angle Calculation:
float altitudeToGround  = 0; //[m]
float distanceOnGround  = 0; //[m]
int MapQuadrant         = 0; //to determine quadrant number
float newAzimut         = 0; //[°]
float currentAzimut     = 0; //the actual azimuth angle the tracker is pointing at currently
float newElevation      = 0; //[°]
float currentElevation  = 0;


void setup() {

  Serial.begin(9600); //USB to PC
  Serial1.begin(115200); //Bluetooth
  Serial3.begin(9600); //RJ45 XBEE hooked up (incoming GPS)

  pinMode(userSwitch, INPUT);
  pinMode(12, OUTPUT);  //5V source for user switch
  digitalWrite(12, HIGH);

  delay(500);
  setupSteppers();

  setupBluetooth();
  setupMagnetometer();
  calibrateElevation();
  calibrateAzimut();
  //checkBattery();
  //printBatteryVoltage(); //printing to USB serial
  setHomePosition();
  playPositiveSound();

}

void loop() {

  updateAngles();
  checkBattery();
  //printBatteryVoltage();

}
