// Function to determine if correct time interval has passed
bool timeSync(unsigned long deltaT)
{
  unsigned long currTime = millis();

  if (currTime - previousTime >= deltaT){
    previousTime = currTime;
    return true;
  }
  else return false;
}

// Send Integer
void sendToPC(int* data)
{
  byte* byteData = (byte*)(data);
  Serial.write(byteData,2);
}

// Send Float or Double (Same in Arduino)
void sendToPC(double* data)
{
  byte* byteData = (byte*)(data);
  Serial.write(byteData,4);
}


// Send 3 Integers
void sendToPC(int* data1, int* data2, int* data3)
{
  byte* byteData1 = (byte*)(data1);
  byte* byteData2 = (byte*)(data2);
  byte* byteData3 = (byte*)(data3);
  byte buf[6] = {byteData1[0], byteData1[1],
                 byteData2[0], byteData2[1],
                 byteData3[0], byteData3[1]};
  Serial.write(buf, 6);
}

// Send 3 floats
void sendToPC(float* data1, float* data2, float* data3)
{
  byte* byteData1 = (byte*)(data1);
  byte* byteData2 = (byte*)(data2);
  byte* byteData3 = (byte*)(data3);
  byte buf[12] = {byteData1[0], byteData1[1], byteData1[2], byteData1[3],
                 byteData2[0], byteData2[1], byteData2[2], byteData2[3],
                 byteData3[0], byteData3[1], byteData3[2], byteData3[3]};
  Serial.write(buf, 12);

}
