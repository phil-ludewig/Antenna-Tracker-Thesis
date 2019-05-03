void calibrateElevation(){

  boardA.move(FWD,25000);
  while(boardA.busyCheck());
  boardA.goUntil(RESET_ABSPOS,REV,400);
  while(boardA.busyCheck());
  boardA.releaseSw(RESET_ABSPOS,FWD);
  while(boardA.busyCheck());

  /*
  //this passage sets elevation platform horizontal (e.g. if camera mounted)
  //move elevation 90° up
  boardA.move(FWD,211680);
  while(boardA.busyCheck());
  */

}

void calibrateAzimut(){

  magnetometer.enterCalMode();
  boardB.setMaxSpeed(140);
  boardB.setFullSpeed(1000);
  boardB.setAccKVAL(100);
  boardB.setDecKVAL(100);
  boardB.setRunKVAL(100);
  boardB.setHoldKVAL(0);

  delay(500);
  boardB.move(FWD, 409600); //720° counterclock is 409600 steps
  while(boardB.busyCheck());
  magnetometer.exitCalMode();

  pointNorth();
  setupSteppers();
  delay(500);

}

void setupLowAzimutSpeed(){

  boardB.configSyncPin(BUSY_PIN, 0);// BUSY pin low during operations;
  boardB.configStepMode(STEP_FS_128);   // 0 microsteps per step
  boardB.setMaxSpeed(140);        // 10000 steps/s max
  boardB.setFullSpeed(1000);       // microstep below 10000 steps/s
  boardB.setAcc(200);             // accelerate at 10000 steps/s/s
  boardB.setDec(200);
  boardB.setSlewRate(SR_290V_us);   // Upping the edge speed increases torque.
  boardB.setOCThreshold(OC_4500mA);  // OC threshold 2250mA
  boardB.setPWMFreq(PWM_DIV_2, PWM_MUL_2); // 31.25kHz PWM freq
  boardB.setOCShutdown(OC_SD_DISABLE); // don't shutdown on OC
  boardB.setVoltageComp(VS_COMP_DISABLE); // don't compensate for motor V
  boardB.setSwitchMode(SW_HARD_STOP);    // Switch is not hard stop
  boardB.setOscMode(EXT_16MHZ_OSCOUT_INVERT); // for boardA, we want 16MHz
  boardB.setAccKVAL(100);
  boardB.setDecKVAL(100);
  boardB.setRunKVAL(100);
  boardB.setHoldKVAL(0);

}

void pointNorth(){

  magnetometer.readHeading();
  float degreesUntilNorth = (350.0 - magnetometer.heading/10.0);
  float multiplier = degreesUntilNorth/360;
  unsigned long stepsUntilNorth = multiplier*200*8*128;
  boardB.move(REV,stepsUntilNorth);
  while(boardB.busyCheck());

  int foundNorth = 0;
  while (foundNorth == 0){
    magnetometer.readHeading();
    int currentHeading = magnetometer.heading/10.0;
    //Serial.println(currentHeading);

    if(currentHeading <= 360 && currentHeading > 180){

      boardB.move(REV,600); //600 steps is about one degree
      while(boardB.busyCheck());
    }

    else if(currentHeading >= 2 && currentHeading <= 180){

      boardB.move(FWD,600);
      while(boardB.busyCheck());
    }

    else{
      foundNorth =1;
    }

    delay(200);

  }

  boardB.move(FWD, 106240); //180° plus 6.75 degrees for calibration deviation
  while(boardB.busyCheck());

}
