
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
const double SCALE_LC = -0.0000975808;
const double OFFSET_LC = 0.71636;

HX711 scale;

// Circular array for a three-value median filter for the LC
static double readings[3] = {0, 0, 0};
static int index = 0;

// Pressure Tranducers
const int PT_TC_PIN = A5; // Thrust Chamber Pressure Transducer
const int PT_OX_PIN = A0;// Thrust Chamber Pressure Transducer

const double SCALE_TC = 1.2195;
const double OFFSET_TC = - 121.95;

const double SCALE_OX = 1.8061674;
const double OFFSET_OX = -180.62;

double getPressureTC () {
  return analogRead(PT_TC_PIN) * SCALE_TC + OFFSET_TC;
} 

double getPressureOX () {
  return analogRead(PT_OX_PIN) * SCALE_OX + OFFSET_OX;
} 

double getForce () {
  // Read from the load cell
  readings[index] = scale.read() * SCALE_LC + OFFSET_LC;
  index = (index + 1) % 3;

  // Get the median force value (to remove outliers)
  if ((readings[0] <= readings[1] && readings[1] <= readings[2]) || (readings[2] <= readings[1] && readings[1] <= readings[0])) {
    return readings[1];
  } else if ((readings[1] <= readings[0] && readings[0] <= readings[2]) || (readings[2] <= readings[0] && readings[0] <= readings[1])) {
    return readings[0];
  } else {
    return readings[2];
  }
}

// SD card file
File dataFile;
String filename = "data.txt";

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

  // Initialize the ethernet library
  ethernet_setup();

  // Get unique filename
  int fileIndex = 1;
  
  while (SD.exists(filename)) {
    filename = "data" + String(fileIndex++) + ".txt";
  }
  
  // Open file
  dataFile = SD.open(filename, FILE_WRITE);
  if (!dataFile) {
    // if there was an error loop forever
    while (1); 
  }
  // Write header to file
  dataFile.println("Time(ms), LoadCell(g), TCPressure(psi), OXPressure(psi)");
  dataFile.close();
}

void loop() {
  // Sets up millis
  unsigned long currTime = millis();

  // Make sensor readings and put into a String
  String data = String(currTime);
  data +=",";
  data += getForce();
  data += ",";
  data += getPressureTC();
  data += ",";
  data += getPressureOX();

  // Write the data to the SD card
  dataFile = SD.open(filename, FILE_WRITE);
  dataFile.println(data);
  dataFile.close();

  // Send the data over the ethernet
  ethernet_send(data);

  // Loop as fast as we can
  //delay(0);
  
  // Beeper Intervals (only beep at low pressure)
  if (currTime - prevTime >= beepPeriod && getPressureOX() < 50) {
    digitalWrite(BEEPER_PIN, HIGH);
    prevTime = currTime;
    currBeepState = 1;
  }
  if (currTime - prevTime >= beepLength && currBeepState == 1) {
    digitalWrite(BEEPER_PIN, LOW);
    currBeepState = 0;
  }
}
