#include <Wire.h>
#include "SparkFunSi4703rds.h"

// Arduino sketch helps to use SI4703 RDS decoder with RDS Spy (http://rdsspy.com)
// Sketch made by Dmitrii dmitryelj@gmail.com
// 2016-07-03. Version 1.0
//
// Usage:
//
// 1) Connect Si4703 FM RDS Tuner Breakout Board to Arduino
//   3.3V Arduino -> Si4704 VCC
//   GND Arduino  -> Si4704 GND
//   A4 Arduino   -> Si4704 SDIO
//   A5 Arduino   -> Si4704 SCLK
//   D2 Arduino   -> Si4704 RST
//
// 2) Upload this sketch to Arduino
// 
// 3) Launch RDS Spy, select ASCII G Protocol data source in settings
//
// 4) Enjoy :)
//    Frequency can be changed via RDS Spy.
//    Commands via serial connections are also available (see the code below). 

int resetPin = 2;
int SDIO = A4;
int SCLK = A5;
int initialFrequency = 945; // 94.5MHz
int initialVolume = 5;
Si4703_Breakout radio(resetPin, SDIO, SCLK);

void setup()
{
  Serial.begin(19200);
  Serial.println("Si4703 to RDS Spy decoder");
  Serial.println("===========================");  
  Serial.println("880..1050 - set new frequency");
  Serial.println("s0..s15 - set audio volume");
  Serial.println("");
  
  radio.powerOn();
  radio.setVolume(initialVolume);
  radio.setChannel(initialFrequency);
}

void loop()
{
  if (Serial.available())
  {
    String data_str = Serial.readStringUntil('\n');  
    int data = data_str.toInt();
    // Frequency range: 88000 to 105000
    if (data >= 880 && data <= 1050) {
      Serial.println("Set new frequency: " + String(data) + "00KHz");
      radio.setChannel(data);
    }
    // Frequency from RDS Spy: command like "088100*F*D*R"
    int symPos = data_str.indexOf("*F");
    if (symPos >= 5) {
      String freq = data_str.substring(0, symPos); // 088100*F*D*R => 088100
      freq = freq.substring(0, freq.length() - 2); // 088100 => 0881
      int channel = freq.toInt();
      radio.setChannel(channel);
    }
    // Volume level
    if (data_str.length() > 1 && data_str.charAt(0) == 's') {
      String val = data_str.substring(1);
      Serial.println("Set new volume: " + val);
      radio.setVolume(val.toInt());
    }
  }

  // If RDS available, get 4 blocks, send them to RDS Spy
  radio.readRegisters();
  if (radio.getRegister(radio.STATUSRSSI) & (1<<radio.RDSR)) {

    String blockA = String(radio.getRegister(radio.RDSA), HEX), blockB = String(radio.getRegister(radio.RDSB), HEX), blockC = String(radio.getRegister(radio.RDSC), HEX), blockD = String(radio.getRegister(radio.RDSD), HEX);
    blockA.toUpperCase();
    // Append block to 4-chars length
    if (blockA.length() == 1) blockA = "000" + blockA;
    if (blockA.length() == 2) blockA = "00" + blockA;
    if (blockA.length() == 3) blockA = "0" + blockA;

    blockB.toUpperCase();
    // Append block to 4-chars length
    if (blockB.length() == 1) blockB = "000" + blockB;
    if (blockB.length() == 2) blockB = "00" + blockB;
    if (blockB.length() == 3) blockB = "0" + blockB;
    
    blockC.toUpperCase();
    // Append block to 4-chars length
    if (blockC.length() == 1) blockC = "000" + blockC;
    if (blockC.length() == 2) blockC = "00" + blockC;
    if (blockC.length() == 3) blockC = "0" + blockC;
    
    blockD.toUpperCase();
    // Append block to 4-chars length
    if (blockD.length() == 1) blockD = "000" + blockD;
    if (blockD.length() == 2) blockD = "00" + blockD;
    if (blockD.length() == 3) blockD = "0" + blockD;

    // Format data according to the ASCII G Protocol 
    Serial.print("G:"); Serial.write(13); Serial.write(10);
    Serial.print(blockA); 
    Serial.print(blockB); 
    Serial.print(blockC); 
    Serial.print(blockD); 
    Serial.write(13); Serial.write(10); 
    Serial.write(13); Serial.write(10);
    delay(40); //Wait for the RDS bit to clear
  } else {
    delay(30); //From AN230, using the polling method 40ms should be sufficient amount of time between checks
  }
}

