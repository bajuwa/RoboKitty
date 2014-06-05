#include <SD.h>
#include "TuneManager.h"

// RADIO SETTINGS
boolean radioOn = true;
TuneManager* songManager;

void setup() {
  Serial.begin(9600); // set up Serial library at 9600 bps

  // Take one text file and convert it to a usable song
  // (only keep one song in memory at a time)
  songManager = new TuneManager();
}

void loop() {
  manageTunes();
}

void manageTunes() {
  if (radioOn) {
    // only consider playing music if the radio is 'on'
    songManager->playTunes();
  }
}



