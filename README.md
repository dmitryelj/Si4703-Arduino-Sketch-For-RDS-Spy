# Si4703-Arduino-Sketch-For-RDS-Spy
Arduino sketch for Si4703 FM Tuner with RDS Spy support.

Arduino sketch helps to use Si4703 FM RDS Tuner Breakout Board with RDS Spy software (http://rdsspy.com).
Made by Dmitrii dmitryelj@gmail.com.
2016-07-03. Version 1.0.
See Wiki section for more details.

# Usage:

1) Connect Si4703 FM RDS Tuner Breakout Board to Arduino:
* 3.3V Arduino -> Si4704 VCC
* GND Arduino  -> Si4704 GND
* A4 Arduino   -> Si4704 SDIO
* A5 Arduino   -> Si4704 SCLK
* D2 Arduino   -> Si4704 RST

2) Upload "Si4703_RDS_Spy.ino" sketch to Arduino.
 
3) Launch RDS Spy, select ASCII G Protocol data source in settings.

4) Enjoy :)
   Frequency can be changed via RDS Spy.
   Commands via serial connections are also available (see the code for details). 
