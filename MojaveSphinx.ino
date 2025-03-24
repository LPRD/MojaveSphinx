
#include <Arduino.h>
#include "HX711.h"
#include <SPI.h>
#include <SD.h>


// Beeper
const int BEEPER_PIN = 8;
const unsigned long beepPeriod = 3000;
const unsigned long beepLength = 200;
unsigned long prevTime = 0;
int currBeepState = 0;

// Load Cell with HX711 breakout board
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

const double SCALE_LC = -22.19238;
const double OFFSET_LC = 21503;

HX711 scale;

// Pressure Tranducers
const int PT_TC_PIN = A1;
const int PT_OX_PIN = A0;

const double SCALE_TC = 1.2195;
const double OFFSET_TC = - 121.95;

const int SCALE_OX = 1;
const int OFFSET_OX = 0;

double getPressureTC (double reading) {
  return reading * SCALE_TC + OFFSET_TC;
} 

double getPressureOX (double reading) {
return reading * SCALE_OX + OFFSET_OX;
} 

// SD card file
File myFile;

void setup() {
  // Pressure Transducers
  pinMode(PT_TC_PIN, INPUT);
  pinMode(PT_OX_PIN, INPUT);
  pinMode(BEEPER_PIN, OUTPUT);
  digitalWrite(BEEPER_PIN, LOW);

  // Load Cell
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(SCALE_LC);
  scale.set_offset(OFFSET_LC);

  // SD Card
  // Initialize card
  if (!SD.begin(4)) {
    // if there was an error loop forever
    while (1);
  }

  // Open file
  myFile = SD.open("data.txt", FILE_WRITE);
  if (!myFile) {
    // if there was an error loop forever
    while (1); 
  }
  // Write header to file
  myFile.println("Time(ms), LoadCell(g), TCPressure(psi), OXPressure(psi)");
  myFile.close();
}

void loop() {
  // Sets up millis
  unsigned long currTime = millis();

  // Make sensor readings and write to file
  myFile = SD.open("data.txt", FILE_WRITE);
  myFile.print(currTime);
  myFile.print(",");
  myFile.print(scale.get_units(1), 3);
  myFile.print(",");
  myFile.print(getPressureTC(analogRead(PT_TC_PIN)));
  myFile.print(",");
  myFile.println(getPressureOX(analogRead(PT_OX_PIN)));
  myFile.close();
  // Loop as fast as we can
  //delay(0);
  
  // Beeper Intervals
  if (currTime - prevTime >= beepPeriod) {
    digitalWrite(BEEPER_PIN, HIGH);
    prevTime = currTime;
    currBeepState = 1;
  }
  if (currTime - prevTime >= beepLength && currBeepState == 1) {
    digitalWrite(BEEPER_PIN, LOW);
    currBeepState = 0;
  }
}
