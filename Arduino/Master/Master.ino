#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h> // Gyroscope
#include <Adafruit_LSM303_Accel.h> // Accelerometer



/* Assign a unique instance of sensors at the same time */ 
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(21);

// Show Display Sensor Info
const bool INFO = false;

// Create sensors_event_t object in memory to hold sensor data
sensors_event_t gyroEvent;
sensors_event_t accelEvent;

// Variables for Transmitting Data to plots for visualization
unsigned long previousTime;
const long DELTA_T = 50; // Time between transmissions (milliseconds)


void setup(void) 
{
  Serial.begin(38400);
  previousTime = millis();
  // Gyro Testing
  gyroTest();

  // Accelerometer Testing and set Up
  accelTest();
  accelSetup(INFO);
  

  if (INFO){
    displayGyroDetails();
    displayAccelDetails();
  }
  
}

void loop(void) 
{
  

  // Get a new sensor event, passing in our 'event' placeholder
  gyro.getEvent(&gyroEvent);
  accel.getEvent(&accelEvent);
 
//  /* Display the results (speed is measured in rad/s) */
//  Serial.print("X: "); Serial.print(gyroEvent.gyro.x); Serial.print("  ");
//  Serial.print("Y: "); Serial.print(gyroEvent.gyro.y); Serial.print("  ");
//  Serial.print("Z: "); Serial.print(gyroEvent.gyro.z); Serial.print("  ");
//  Serial.println("rad/s ");

//Serial.println(event.gyro.x );
sendToPC(&gyroEvent.gyro.x,&gyroEvent.gyro.y,&gyroEvent.gyro.z);
  delay(5);
}
