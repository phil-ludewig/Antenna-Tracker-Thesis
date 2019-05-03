//This section is dedicated to testcodes

void P4testElevationSpeed(){

  //this program determines the elevation rotational speed
  int P4elevationStartTime = millis();
  boardA.move(FWD, 211584); //90°
  while(boardA.busyCheck());
  int P4elevationStopTime = millis();

  float P4elevationTotalTime = (P4elevationStopTime - P4elevationStartTime)/1000;
  float P4elevationSpeed = 90/P4elevationTotalTime;

  Serial.print("Total seconds required for 90° rotation: ");
  Serial.println(P4elevationTotalTime);
  Serial.print("Calculated rotational speed is ");
  Serial.print(P4elevationSpeed);
  Serial.println("degrees per second.");

}


void P5testAzimutSpeed(){

  //this program determines the azimuth rotational speed
  int P5azimutStartTime = millis();
  boardB.move(FWD, 204800);
  while(boardB.busyCheck());
  int P5azimutStopTime = millis();

  float P5azimutTotalTime = (P5azimutStopTime - P5azimutStartTime)/1000;
  float P5azimutSpeed = 360/P5azimutTotalTime;

  Serial.print("Total seconds required for 360° rotation: ");
  Serial.println(P5azimutTotalTime);
  Serial.print("Calculated rotational speed is ");
  Serial.print(P5azimutSpeed);
  Serial.println("degrees per second.");

}

void P6testElevationMomentum(){

  //this program moves the elevation axis down 90° and up 90° to determine whether or not 5kg can be moved
  boardA.move(FWD, 211584); //90°
  while(boardA.busyCheck());

  delay(5000);

  boardA.move(REV,211584);
  while(boardA.busyCheck());

}

int P9previousGPSProcessingTime = 0;

void P9calculateGPSProcessingRate(){

  int P9currentGPSProcessingTime = millis();
  int P9timeSinceLastGPSProcessing = P9currentGPSProcessingTime - P9previousGPSProcessingTime;
  float P9GPSProcessingRate = P9timeSinceLastGPSProcessing/1000;

  Serial1.print("The GPS signal is processing at a rate of ");
  Serial1.print(P9GPSProcessingRate);
  Serial1.println("Hz.");

}

void testAllRotations(){

  //this program moves the elevation axis down 90° and up 90° to determine whether or not 5kg can be moved
  boardA.move(FWD, 211584); //90°
  while(boardA.busyCheck());

  boardB.move(FWD, 204800);
  while(boardB.busyCheck());
  boardB.move(REV, 204800);
  boardA.move(REV, 211584);
  while(boardA.busyCheck());
  while(boardB.busyCheck());


}
