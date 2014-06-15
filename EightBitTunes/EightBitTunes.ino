#include <SD.h>
#include "ABCNoteParser.h"
#include "TuneManager.h"

// RADIO SETTINGS
boolean radioOn = true;
TuneManager* songManager;

void setup() {
  Serial.begin(9600); // set up Serial library at 9600 bps
  songManager = new TuneManager("/tunes/");
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



