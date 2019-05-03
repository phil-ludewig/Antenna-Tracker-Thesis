#include <Arduino.h>

/*
void ledIntro(){

  for(int i_fade=0; i_fade<255; i_fade++){
    analogWrite(LedRing, i_fade);
    delay(5);
  }

}

*/

void setupBluetooth(){

  //has to be done every bootup (to change baudrate to 9600)
  Serial1.print("$"); //configuration commands
  Serial1.print("$");
  Serial1.print("$");
  delay(1000);
  Serial1.println("U,9600,N"); //configuration for 9600 baudrate
  delay(1000);
  Serial1.begin(9600);
  delay(500);

}

void setupMagnetometer(){

  delay(500);
  magnetometer.init();
  if (!magnetometer.init()){

    Serial.println("Sensor Initialization Failed\n\r");

  }

  delay(500);

}

void setupGPSsensor(){

  for(int GPScycle = 0; GPScycle <10; GPScycle ++){

    Serial3.println("$PMTK314,0,5,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0*28");
    delay(500);

  }

  for(int GPScycle2 = 0; GPScycle2 <10; GPScycle2++){

    Serial3.println("$PMTK251,9600*17");
    delay(500);

  }

  Serial3.begin(9600);
  Serial.println("GPS setup complete");
}


void updateAngles(){

  const long gpsUpdateInterval = 2000;
  int positionValid = 0;


  //for debugging change both Serial ports in while loop (e.g. if simulating GPS data)
  //Serial.available() and Serial.read() for GPS input via USB
  //Serial1.available() and Serial1.read() for GPS input via Bluetooth
  //Serial3.available() and Serial3.read() for GPS input via RJ45 port
  //also change Serialport in setHomePosition() !

  while(Serial3.available()){

    gpsString = Serial3.read();
    Serial1.println(gpsString); //send GPS string via Bluetooth to maps application for example

    if(gpsSensor.encode(gpsString)){

      gpsSensor.f_get_position(&UAVLatitude,&UAVLongitude);
      UAVAltitude = gpsSensor.f_altitude();
      tone(buzzerPin,1000,50);
      positionValid = 1;

    }

    currentGpsTimestamp = millis();

    if(positionValid == 1 && currentGpsTimestamp - previousGpsTimestamp >= gpsUpdateInterval){

      previousGpsTimestamp = currentGpsTimestamp;

      angleCalculation();

        if(distanceOnGround > minimumUAVDistance){

          elevationControl();
          azimutControl();

        }
        else{

          //playNegativeSound();

        }

      positionValid = 0;

    }
  }
}



void setHomePosition(){

  int homeValid = 0;
  while(homeValid == 0){

    while(Serial3.available()){

      gpsString = Serial3.read();
      tone(buzzerPin,1000,50);

      if(gpsSensor.encode(gpsString)){

        gpsSensor.f_get_position(&homeLatitude,&homeLongitude);
        homeAltitude = gpsSensor.f_altitude();
        homeValid = 1;

      }

    }

  }
}

void checkBattery(){

  if(analogRead(batteryPin) > 50){

    int batteryValue = analogRead(batteryPin);
    float pinVoltage = batteryValue * (5.0 / 1023.0);
    batteryVoltage = (pinVoltage * 13.7) / 4.7; //13.15kOhm and 4.7kOhm resistor
    iBatteryVoltage = round(batteryVoltage * 100); //times hundred for more accuracy

    if(iBatteryVoltage < 1080 && iBatteryVoltage > 600){  //if below 10.8V but not if below 6V (would mean tracker is USB powered)

      batteryAlarm();

    }

  }

}

void batteryAlarm(){

  int batteryAlarmInterval = 3000;

  currentBatteryAlarmTimestamp = millis();

  if(currentBatteryAlarmTimestamp - previousBatteryAlarmTimestamp > batteryAlarmInterval){

    tone(buzzerPin,2000,200);
    previousBatteryAlarmTimestamp = currentBatteryAlarmTimestamp;


  }

}

void printBatteryVoltage(){

  currentBatteryPrintTime = millis();

  if(currentBatteryPrintTime - previousBatteryPrintTime > 500){ //print every XX milliseconds

    Serial.println(batteryVoltage);

    previousBatteryPrintTime = currentBatteryPrintTime;

  }
}

void playPositiveSound(){

  for(int buzzloop = 0; buzzloop < 2; buzzloop++){
    tone(buzzerPin,1000,200);
    delay(200);
    tone(buzzerPin,1500,200);
    delay(200);
    tone(buzzerPin,2000,200);
    delay(200);
  }

}

void playNegativeSound(){

  for(int buzzloop = 0; buzzloop < 2; buzzloop++){
    tone(buzzerPin,2000,200);
    delay(200);
    tone(buzzerPin,1000,300);
    delay(300);
  }

}

void playCountdownSound(){

  tone(buzzerPin,1500,500);
  delay(700);
  tone(buzzerPin,1500,500);
  delay(700);
  tone(buzzerPin,1500,500);
  delay(700);
  tone(buzzerPin,2000,800);
  delay(800);

}
