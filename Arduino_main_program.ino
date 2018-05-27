/* -----------------------------------------------------------
 *                Arduino main program
 *                     Made by
 *      Belloncle Nicolas, Bernard Martin, De Pasquale Tom
 *                     2017/2018
 * -----------------------------------------------------------
 */

#include <Timer.h>
//Objects
Timer pedalingTimeSinceLastReset;
Timer pedalingTime;

//Pins
const int MAGNETIC_SENSOR = 2;
const int RESET_BUTTON = 3;
const int LED = 13;

//Constant variables
const float wheelPerimeter = 0.622 * PI;    //The diameter is 0.622 meters

//Variables
//Initialization:
String charToSend;
String charToReceive;
bool initialized;

//Magnet detection:
bool magneticSensorHasBeenOn;
float lastTime;

//Values:
float speed;
float averageSpeed;
float averageSpeedSinceLastReset;
float totalDistanceSinceLastReset;
float totalDistance;
float maxTime;
float maxSpeed;

void setup() {
  //Serial
  Serial.begin(9600);

  //Pin mode
  pinMode(MAGNETIC_SENSOR, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(RESET_BUTTON, INPUT_PULLUP);

  //Setup variables and pins
  digitalWrite(LED, LOW);

  magneticSensorHasBeenOn = true;
  initialized = false;

  lastTime = -1;                //It has not been initialized
  speed = 0;
  totalDistanceSinceLastReset = 0;
  totalDistance = 0;
  averageSpeed = 0;
  averageSpeedSinceLastReset = 0;
  maxTime = 0;
  maxSpeed = 0;
}

void loop() {
  //Testing for a connection between Arduino & Processing
  if (serialInitialized()) {
    //Led is on
    digitalWrite(LED, HIGH);

    //Magnet Detection
    if (isMagneticSensorOn())
    {
      //Calculate the speed
      calculateSpeed();

      /* Time objects:
         Starting or resuming the timers.
         - If it has not been started, starts the timer.
         - If it has been paused, resuming.
      */
      pedalingTimeSinceLastReset.start();
      pedalingTime.start();
      pedalingTimeSinceLastReset.resume();
      pedalingTime.resume();
    }
    else  //If magnet isn't detected
    {
      /* It pauses the timers if the magnet hasn't been detected for 3 seconds */
      if (millis() - lastTime > 3000)
      {
        pedalingTimeSinceLastReset.pause();
        pedalingTime.pause();
        speed = 0;
        lastTime = millis();
      }
    }

    /* If the pedaling time is not 0, it calculates the average speed
       else, there is an error (divided by 0)
    */
    if (pedalingTimeSinceLastReset.getTotalTime() > 0)
      averageSpeedSinceLastReset = (totalDistanceSinceLastReset / (pedalingTimeSinceLastReset.getTotalTime() / 1000)) * 3.6;
    if (pedalingTime.getTotalTime() > 0)
      averageSpeed = (totalDistance / (pedalingTime.getTotalTime() / 1000)) * 3.6;

    //Calculate the maximum distance since two reset
    if (pedalingTimeSinceLastReset.getTotalTime() > maxTime)
      maxTime = pedalingTimeSinceLastReset.getTotalTime();
      
    //Calculate the maximum speed
    if(speed > maxSpeed)
      maxSpeed = speed;

    //Function for the reset button
    resetButton();
    //Function to send informations to Processing
    displayInfosOnSerial();
  }
  else
  {
    //Led is off
    digitalWrite(LED, LOW);
  }
}


//Function for detecting magnet
bool isMagneticSensorOn()
{
  /* Delay of 100ms. It is the minimum time between two detections */
  delay(100);
  //Saving the state of the magnetic sensor
  bool stateOfSensor = digitalRead(MAGNETIC_SENSOR);

  //If magnetic sensor detects magnet
  if (stateOfSensor == HIGH)
  {
    //The sensor must have been off before returning on
    if (!magneticSensorHasBeenOn)
    {
      magneticSensorHasBeenOn = true;
      //Updating values
      totalDistanceSinceLastReset += wheelPerimeter;
      totalDistance += wheelPerimeter;
      //Serial.println("Magnet detected.");

      //Magnet is detected, return true to the function
      return true;
    }
  }
  else
  {
    //To et know that the sensor has been off
    if (magneticSensorHasBeenOn == true)
    {
      magneticSensorHasBeenOn = false;

      //Magnet is not detected, return false to the function
      return false;
    }
  }
}

//Function that calculate the speed
void calculateSpeed()
{
  float _speed = 0;     //Speed is 0 at the beginning
  if (lastTime < 0)     //It is the first time that the magnet passes in front of the sensor
  {
    lastTime = millis();  //Getting the time value of the last time
  }
  else
  {
    float dt = millis() - lastTime;     //dt is in milliseconds
    dt /= 1000;                         //Puting dt in seconds
    _speed = (wheelPerimeter / dt) * 3.6;   //Calculating speed in km/h
    lastTime = millis();                    //Updating lastTime value
  }


  if (_speed > 65)          //If the magnet has been detected 2 times in 100ms, the speed is over 65 -> speed = 0
  {
    return;
  }
  else
  {
    speed = _speed;  
  }
}

//Function that send informations to Processing
void displayInfosOnSerial()
{
  /*  Infos are in this order :
      totalAverageSpeed; totalPedalingTime; averageSpeedSinceLastReset; pedalingTimeSinceLastReset; maxSpeed; maxTime; speed;
  */
  Serial.print(averageSpeed, 2);
  Serial.print(";");
  Serial.print(pedalingTime.getTotalTime()/1000, 2);
  Serial.print(";");
  Serial.print(averageSpeedSinceLastReset, 2);
  Serial.print(";");
  Serial.print(pedalingTimeSinceLastReset.getTotalTime()/1000, 2);
  Serial.print(";");
  Serial.print(maxSpeed, 2);
  Serial.print(";");
  Serial.print(maxTime/1000, 2);
  Serial.print(";");
  Serial.print(speed, 2);
  
  Serial.print("\n");   //Char that ends the line /!\ Important
}

//Function for the reset button
void resetButton()
{
  //Checking if button is pressed
  bool stateOfButton = digitalRead(RESET_BUTTON);
  //If it is pressed
  if (stateOfButton == HIGH)
  {
    //Stoping time since last reset
    pedalingTimeSinceLastReset.stop();
    //Reseting all the values
    speed = 0;
    totalDistanceSinceLastReset = 0;
    averageSpeedSinceLastReset = 0;
  }
}

//Function to initialize Arduino, it checks the connection between Processing & Arduino
bool serialInitialized()
{
  //return true;  //Remove the comment if you want to test the program without processing (all the value are in the console)
  //If it has not been initialized
  if (initialized == false)
  {
    //Checking if the Serial is available to read
    if (Serial.available() > 0)
    {
      //Reading char that comes from Processing
      charToReceive = Serial.read();

      //If it receive the good char, initialized = true
      if (charToReceive == "A");
      {
        initialized = true;
      }
    }
    else    //If it is not available to read, send a char to Processing
    {
      Serial.write('T');
    }
    delay(100);
    return false;
  }
  else
  {
    return true;
  }
}
