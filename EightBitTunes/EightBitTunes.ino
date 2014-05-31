#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <list>
#include "Note.h"
#include "TuneManager.h"

using std::list;

// RADIO SETTINGS
boolean radioOn = true;
boolean loopSingleSong = false;
TuneManager* songManager = new TuneManager();

void setup() {
  Serial.begin(9600); // set up Serial library at 9600 bps

  // Take one text file and convert it to a usable song
  // (only keep one song in memory at a time)
  Serial.println("Parsing text file for song");
  songManager->loadTune(" -659 150-659 150-0 150-659 150-0 150-523 150-659 150-0 150-784 150-0 450-392 150-0 450-523 150-0 300-392 150");
}

bool test = true;
int i=0;
void loop() {
  manageTunes();
}

void manageTunes() {
  if (radioOn) {
    // only consider playing music if the radio is 'on'
    songManager->playTune();
  }
}



