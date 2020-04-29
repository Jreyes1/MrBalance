void displayAccelDetails(void) {
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.println(" m/s^2");
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.println(" m/s^2");
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void accelTest(void){
 // Serial.println("Accelerometer Test");
  //Serial.println("");

  /* Initialise the sensor */
  if (!accel.begin()) {
    /* There was a problem detecting the ADXL345 ... check your connections */
    //Serial.println("Ooops, no LSM303 (Accelerometer) detected ... Check your wiring!");
    while (1);
  }

  //Serial.println("Accelerometer Test Successfull");
}

// Setting up Range and Resolution
void accelSetup(bool info){
  
  accel.setRange(LSM303_RANGE_4G);    // Range: 2G,4G,8G,16G
  accel.setMode(LSM303_MODE_NORMAL);  //Mode: LOW_POWER,NORMAL,HIGH_RESOLUTION

  if(info){
    // Range Info
    Serial.print("Range set to: ");
    lsm303_accel_range_t new_range = accel.getRange();
    switch (new_range) {
    case LSM303_RANGE_2G:
      Serial.println("+- 2G");
      break;
    case LSM303_RANGE_4G:
      Serial.println("+- 4G");
      break;
    case LSM303_RANGE_8G:
      Serial.println("+- 8G");
      break;
    case LSM303_RANGE_16G:
      Serial.println("+- 16G");
      break;
    }
  
    // Mode Info
    Serial.print("Mode set to: ");
    lsm303_accel_mode_t new_mode = accel.getMode();
    switch (new_mode) {
    case LSM303_MODE_NORMAL:
      Serial.println("Normal");
      break;
    case LSM303_MODE_LOW_POWER:
      Serial.println("Low Power");
      break;
    case LSM303_MODE_HIGH_RESOLUTION:
      Serial.println("High Resolution");
      break;
    }
  }
}
