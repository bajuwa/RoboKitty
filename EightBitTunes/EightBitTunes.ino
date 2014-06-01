#include "TuneManager.h"

// RADIO SETTINGS
boolean radioOn = true;
TuneManager* songManager = new TuneManager();

void setup() {
  Serial.begin(9600); // set up Serial library at 9600 bps

  // Take one text file and convert it to a usable song
  // (only keep one song in memory at a time)
  Serial.println(F("Parsing text file for song"));
  songManager->loadTune(" -988 176-0 352-330 176-0 352-349 176-0 176-294 176-330 176-0 176-349 176-0 352-523 176-0 176-988 176-0 352-330 176-0 352-349 176-0 176-294 176-330 176-0 176-349 176-0 352-880 176-932 176-494 176-0 352-554 176-0 352-587 176-0 176-494 176-554 176-0 176-587 176-0 352-880 176-932 176-988 176-0 352-1109 176-0 352-1175 176-0 176-988 176-1109 176-0 176-1175 176-0 352-880 352-988 176");
}

void loop() {
  manageTunes();
}

void manageTunes() {
  if (radioOn) {
    // only consider playing music if the radio is 'on'
    songManager->playTune();
  }
}



