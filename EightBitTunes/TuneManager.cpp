#include "TuneManager.h"

// HARDWARE CONSTANTS
// pin of speaker/pezo
int const PIN_PEZO = 11;

// Sound management...
vector<Note*> tune;
int noteIndex = 0;

// Timing
long previousMillis = 0;
long interval = 0;

TuneManager::TuneManager() {
  previousMillis = millis();
  tune.push_back(NULL);
}

void TuneManager::playNote(int freq, int dur) {
  // play the song by iterating over the notes at given intervals:
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;   
  
    // If the current note isn't a 'rest' beat, 
    // then play the note for the alloted duration (milliseconds)
    tone(PIN_PEZO, freq, dur);
    
    // Set how long to wait until next note 
    // (namely the length of the currently playing note)
    interval = dur;
    
    noteIndex++;
  }  
}

void TuneManager::playTune(bool loopTune) {
  // Play the currently loaded sound
  if (tune[noteIndex]) {
    playNote(tune[noteIndex]->getFrequency(), 
             tune[noteIndex]->getDuration());
  } else {
    if (loopTune) {
      // Make sure we reset the current note
      noteIndex = 0;
    } else {
      // Once complete, remove it from memory
      tune.clear();
      tune.push_back(NULL);
    }
  }  
}

void TuneManager::loadTune(char tuneToLoad[]) {
  tune.clear();
  char separators[] = " -";
  char* noteFreq = strtok(tuneToLoad, separators);
  char* noteDur = strtok(NULL, separators);
  unsigned int i = 0;
  while (noteFreq != NULL && noteDur != NULL) {
    tune.push_back( new Note(atoi(noteFreq), atoi(noteDur)) );
    noteFreq = strtok(NULL, separators);
    noteDur = strtok(NULL, separators);
  }
  tune.push_back( NULL ); 
}

void TuneManager::loadSound(SOUNDS sound) {
  switch (sound) {
    case HAPPY:
      // Mario One-Up 
      this->loadTune("330 150-392 150-587 150-494 150-523 150-698 150");
      break;
    case SAD:
      // Mario Death
      this->loadTune("1047 50-1109 50-1175 50-0 450-247 150-698 150-0 150-698 150-698 200-659 200-587 200-523 150-330 150-0 150-330 150-262 150");
      break;
    case PANIC:
      this->loadTune("1000 200-1300 200-1000 200-1300 200");
      break;
    case BORED:
      this->loadTune("1000 200-1300 200-1000 200-1300 200");
      break;
  }
}



