#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <vector>
#include "Note.h"
#include "TuneManager.h"

using std::vector;

// RADIO SETTINGS
boolean radioOn = false;
boolean loopSingleSong = false;
TuneManager* soundManager = new TuneManager();
TuneManager* songManager = new TuneManager();

void setup() {
  Serial.begin(9600); // set up Serial library at 9600 bps

  // Take one text file and convert it to a usable song
  // (only keep one song in memory at a time)
  Serial.println("Parsing text file for song");
  //char pseudoFileText[] = "659 150-659 150-0 150-659 150-0 150-523 150-659 150-0 150-784 150-0 450-392 150-0 450-523 150-0 300-392 150-0 300-330 150-0 150-0 150-440 150-0 150-494 150-0 150-494 150-440 150-0 150-392 200-659 200-784 200-880 150-0 150-698 150-784 150-0 150-659 150-0 150-523 150-587 150-494 150-0 300-523 150-0 300-392 150-0 300-330 150-0 150-0 150-440 150-0 150-494 150-0 150-494 150-440 150-0 150-392 200-659 200-784 200-880 150-0 150-698 150-784 150-0 150-659 150-0 150-523 150-587 150-494 150-0 300";
  songManager->loadTune("659 150-659 150-0 150-659 150-0 150-523 150-659 150-0 150-784 150-0 450-392 150");
}

bool test = true;
int i=0;
void loop() {
  if (millis() > 2000 && test) {
    Serial.println("loading sound");
    test = false;
    soundManager->loadSound(SAD);  
  }
  
  manageTunes();
}

void manageTunes() {
  if (radioOn) {
    // only consider playing music if the radio is 'on'
    songManager->playTune(loopSingleSong);
  } else {
    soundManager->playTune(false);
  }
}



