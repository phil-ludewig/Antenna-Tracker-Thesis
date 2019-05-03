#include <Arduino.h>



/*
//test home coordinates (middle Luitpoldpark)
float homeLatitude      = 48.168926;
float homeLongitude     = 11.568848;
float homeAltitude      = 0; //[m]
*/

/*
//test home coordinates (mid west side TSV Sportplatz)
float homeLatitude = 48.010070;
float homeLongitude = 11.346103;
float homeAltitude = 0; //[m]

//test home coordinates (south west corner TSV Sportplatz)
float homeLatitude = 48.009610;
float homeLongitude = 11.346097;
float homeAltitude = 608.00; //[m]

/*Test Luitpold Park
Home Positon: 48.168926,11.568848
Quadrant 1: 48.170639,11.570251, Azimut: 28,61°, 217,32m Distance on ground;
Quadrant 2: 48.167487,11.570463, Azimut: 143,19°, 199,97m Distance on ground;
Quadrant 3: 48.167821,11.566685, Azimut: 232,52°, 202,98m Distance on ground
Quadrant 4: 48.170246,11.567995, Azimut: 336,67°, 160,00m Distance on ground
*/

void angleCalculation() {
  float deltaLatitude = UAVLatitude - homeLatitude;
  float deltaLongitude = UAVLongitude - homeLongitude;

  //first need to determine quadrant with comparing variable (multiplication with 1000 to include 4 comma values of the floating point variable, before rounding into an integer (which can be compared)
  long compareLatitude = round(deltaLatitude*1000000);
  long compareLongitude = round(deltaLongitude*1000000);

  //argument for quadrant 1
  if(compareLatitude >= 0 && compareLongitude >=0){
    MapQuadrant = 1;
  }

  //argument for quadrant 2
  if(compareLatitude <= 0 && compareLongitude >= 0){
    MapQuadrant = 2;
  }

  //argument for quadrant 3
  if(compareLatitude <= 0 && compareLongitude <= 0){
    MapQuadrant = 3;
  }

  //argrument for quadrant 4
  if(compareLatitude >= 0 && compareLongitude <= 0){
    MapQuadrant = 4;
  }

  //Azimut and elevation calculations

  if(MapQuadrant == 1){

    //distance difference in km between home position and object position, separately for longitude and latitude
    float distanceLatitudeKm = deltaLatitude*111.3; //111.3km distance between two circles of latitudes
    float distanceLongitudeKm = deltaLongitude*111.3*cos(((homeLatitude+UAVLatitude)/2)*DEG_TO_RAD); //middle value of latitude determined for estimation

    //total distance on ground to home position calculation
    distanceOnGround = sqrt(sq(distanceLatitudeKm)+sq(distanceLongitudeKm)); //pythagoras
    distanceOnGround = distanceOnGround*1000; //converting total distance from km into m (for elevation calculation)

    //only for testing purposes:
    //distanceOnGround = 100;

    //azimut calculation
    newAzimut = atan(distanceLongitudeKm/distanceLatitudeKm); //azimut in RAD
    newAzimut = newAzimut*RAD_TO_DEG;

    //elevation calculation
    altitudeToGround = UAVAltitude - homeAltitude; //calculating the object altitude in respect to home position
    newElevation = atan(altitudeToGround / distanceOnGround);  //elevation in RAD
    newElevation = newElevation*RAD_TO_DEG;
  }

  if(MapQuadrant == 2){

    float distanceLatitudeKm = deltaLatitude*111.3*(-1);
    float distanceLongitudeKm = deltaLongitude*111.3*cos(((homeLatitude+UAVLatitude)/2)*DEG_TO_RAD);

    distanceOnGround = sqrt(sq(distanceLatitudeKm)+sq(distanceLongitudeKm));
    distanceOnGround = distanceOnGround*1000;

    newAzimut = atan(distanceLatitudeKm/distanceLongitudeKm);
    newAzimut = newAzimut*RAD_TO_DEG + 90;

    altitudeToGround = UAVAltitude - homeAltitude;
    newElevation = atan(altitudeToGround / distanceOnGround);
    newElevation = newElevation*RAD_TO_DEG;

  }

  if(MapQuadrant == 3){

    float distanceLatitudeKm = deltaLatitude*111.3*(-1);
    float distanceLongitudeKm = deltaLongitude*111.3*(-1)*cos(((homeLatitude+UAVLatitude)/2)*DEG_TO_RAD);

    distanceOnGround = sqrt(sq(distanceLatitudeKm)+sq(distanceLongitudeKm));
    distanceOnGround = distanceOnGround*1000;

    newAzimut = atan(distanceLongitudeKm/distanceLatitudeKm);
    newAzimut = newAzimut*RAD_TO_DEG + 180;

    altitudeToGround = UAVAltitude - homeAltitude;
    newElevation = atan(altitudeToGround / distanceOnGround);
    newElevation = newElevation*RAD_TO_DEG;

  }

  if(MapQuadrant == 4){

    float distanceLatitudeKm = deltaLatitude*111.3;
    float distanceLongitudeKm = deltaLongitude*111.3*(-1)*cos(((homeLatitude+UAVLatitude)/2)*DEG_TO_RAD);

    distanceOnGround = sqrt(sq(distanceLatitudeKm)+sq(distanceLongitudeKm));
    distanceOnGround = distanceOnGround*1000;

    newAzimut = atan(distanceLatitudeKm/distanceLongitudeKm);
    newAzimut = newAzimut*RAD_TO_DEG + 270;

    altitudeToGround = UAVAltitude - homeAltitude;
    newElevation = atan(altitudeToGround / distanceOnGround);
    newElevation = newElevation*RAD_TO_DEG;

  }


    //Serial1.print("UAV Distance:  ");
    //Serial1.println(distanceOnGround);
    //Serial.print("Altitude to ground: ");
    //Serial.println(altitudeToGround);
    //Serial1.print("UAV Azimut:    ");
    //Serial1.println(newAzimut);

    //Serial1.print("UAV Elevation: ");
    //Serial1.println(newElevation);

}
