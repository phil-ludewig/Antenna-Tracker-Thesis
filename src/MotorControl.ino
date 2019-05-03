//This section moves the motors according to the calculated angles

void elevationControl(){
  //Serial1.print("Current Elevation: ");
  //Serial1.println(currentElevation);
  float deltaElevation = 0;
  int i_currentElevation = round(currentElevation); //comparevalue for elevation
  int i_newElevation = round(newElevation); //comparevalue for elevation

  //first make sure the maximum and minimum elevation angle are not exceeded:
  if(i_newElevation > 90){ //failsafe in case calculations go over 90 degrees elevation
    newElevation = 90;
  }

  if(i_newElevation < 0){ //failsafe in case calculations go below 0 degrees elevation
    newElevation = 0;
  }

  int i_deltaElevation = sqrt(sq(i_currentElevation-i_newElevation)); //for comparisons
  if(i_deltaElevation >= 1){ //only adjust if gap larger than those degrees

    if(i_currentElevation < i_newElevation){

      deltaElevation = newElevation - currentElevation;

      int eleFullstepCount = round((deltaElevation*eleStepsPerRev*eleGearRatio)/360);

      unsigned long u_eleFullstepCount = eleFullstepCount;

      unsigned long eleStepCount = eleFullstepCount*16; //has to be split in smaller calculations, otherwise erroneous
      eleStepCount = eleStepCount*8;

      boardA.move(FWD,eleStepCount);

      currentElevation = currentElevation + (eleStepCount*360/(eleGearRatio*eleMicrostepping*eleStepsPerRev));

    }

    if(i_currentElevation > i_newElevation){

      deltaElevation = currentElevation - newElevation;

      int eleFullstepCount = round((deltaElevation*eleStepsPerRev*eleGearRatio)/360);

      unsigned long u_eleFullstepCount = eleFullstepCount;

      unsigned long eleStepCount = eleFullstepCount*16; //has to be split in smaller calculations, otherwise erroneous
      eleStepCount = eleStepCount*8;

      boardA.move(REV,eleStepCount);

      currentElevation = currentElevation - (eleStepCount*360/(eleGearRatio*eleMicrostepping*eleStepsPerRev));

    }

  }

}

void azimutControl(){

  unsigned long i_currentAzimut = round(currentAzimut);
  unsigned long i_newAzimut = round(newAzimut);
  unsigned long i_deltaAzimut = sqrt(sq(i_currentAzimut-i_newAzimut));
  //int i_aziStepCount = 0;
  float deltaAzimut = 0;
  int i_azimutDirection = 0;

    if(i_deltaAzimut > 1){  //only perform if the azimut difference is larger than 4 degrees

      if(i_newAzimut > i_currentAzimut){  //case 1 or 2

        if(i_newAzimut - i_currentAzimut <= 180){ //case 1

          deltaAzimut = newAzimut - currentAzimut;

          i_azimutDirection = 1;  //clockwise
        }


        else{

          deltaAzimut = (360 - newAzimut) + currentAzimut;

          i_azimutDirection = 2;  //counterclockwise
        }
      }

      else{ //case 3 or 4 (newAzimut < currentAzimut)

        if(sqrt(sq(i_newAzimut - i_currentAzimut)) <= 180){ //case 3

          deltaAzimut = currentAzimut - newAzimut;

          i_azimutDirection = 2;  //counterclockwise
        }

        else{ //case 4

          deltaAzimut = (360 - currentAzimut) + newAzimut;

          i_azimutDirection = 1;  //clockwise
        }
      }
    }

    //Serial.print("DeltaAzimut is: ");
    //Serial.println(deltaAzimut);
    int aziFullstepCount = round((deltaAzimut*aziStepsPerRev*aziGearRatio)/360);
    unsigned long u_aziFullstepCount = aziFullstepCount;
    unsigned long aziStepCount = aziFullstepCount*32; //has to be split in smaller calculations, else erroneous
    aziStepCount = aziStepCount*4; //smaller calculation (32*4= 128 microstepping)
    //Serial.print("aziStepCount is:  ");
    //Serial.println(aziStepCount);

    if(i_azimutDirection == 1){

      boardB.move(REV,aziStepCount);  //clockwise
      //while(boardB.busyCheck());

      int i_aziStepCount = aziStepCount;

      //Serial.print("i_aziStepCount is : ");
      //Serial.println(i_aziStepCount);

      //calculation only works in all those steps:
      float actualAzimutChange = aziStepCount/128;
      actualAzimutChange = actualAzimutChange/aziStepsPerRev;
      actualAzimutChange = actualAzimutChange/aziGearRatio;
      actualAzimutChange = actualAzimutChange*360;

      //Serial.print("actualAzimutChange is: ");
      //Serial.println(actualAzimutChange);

      currentAzimut = currentAzimut + actualAzimutChange; //because the motor can not exactly rotate the required azimutchange

      int i_compareAzimut = round(currentAzimut);

      if(i_compareAzimut > 360){  //make sure the currentAzimut does not appear higher than 360°

        currentAzimut = currentAzimut - 360;
      }


    }

    if(i_azimutDirection == 2){

      boardB.move(FWD,aziStepCount);  //counterclockwise
      //while(boardB.busyCheck());

      int i_aziStepCount = aziStepCount;

      //Serial.print("i_aziStepCount is : ");
      //Serial.println(i_aziStepCount);

      //calculation only works in all those steps:
      float actualAzimutChange = aziStepCount/128;
      actualAzimutChange = actualAzimutChange/aziStepsPerRev;
      actualAzimutChange = actualAzimutChange/aziGearRatio;
      actualAzimutChange = actualAzimutChange*360;

      //Serial.print("actualAzimutChange is: ");
      //Serial.println(actualAzimutChange);

      currentAzimut = currentAzimut - actualAzimutChange;

      int i_compareAzimut = round(currentAzimut);

      if(i_compareAzimut < 0){

        currentAzimut = currentAzimut + 360;
      }


    }


//Serial1.print("Current Azimuth is:  ");
//Serial1.println(currentAzimut);

}
