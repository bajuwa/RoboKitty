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
  songManager->loadTune(" -880 88-784 88-698 88-659 88-880 88-659 88-698 88-659 88-880 88-622 88-659 88-622 88-880 88-587 88-659 88-587 88-880 88-622 88-659 88-622 88-880 88-523 88-587 88-523 88-880 88-494 88-523 88-494 88-880 88-622 88-659 88-622 88-988 176-0 176-330 176-0 176-349 176-0 176-294 176-330 176-0 176-349 176-0 352-523 176-0 176-988 176-0 352-330 176-0 352-349 176-0 176-294 176-330 176-0 176-349 176-0 352-523 176-0 176-294 176-0 352-330 176-0 352-349 176-0 176-294 176-330 176-0 176-349 176-0 352-523 176-0 176-587 176-0 352-659 176-0 352-698 176-0 176-587 176-659 176-0 176-698 176-0 352-523 176-554 176-");
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



