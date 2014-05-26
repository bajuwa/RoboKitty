#include "TuneManager.h"

// HARDWARE CONSTANTS
// pin of speaker/pezo
int const PIN_PEZO = 11;

// Sound management...
vector<Note*> loadedTune;
int noteIndex = 0;

// Timing
long previousMillis = 0;
long interval = 0;

TuneManager::TuneManager() {
  previousMillis = millis();
  loadedTune.push_back(NULL);
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
  if (loadedTune[noteIndex]) {
    playNote(loadedTune[noteIndex]->getFrequency(), 
             loadedTune[noteIndex]->getDuration());
  } else {
    if (loopTune) {
      // Make sure we reset the current note
      noteIndex = 0;
    } else {
      // Once complete, remove it from memory
      loadedTune.clear();
      loadedTune.push_back(NULL);
    }
  }  
}

void TuneManager::loadTune(char tune[]) {
  loadedTune.clear();
  char separators[] = " -";
  char* noteFreq = strtok(tune, separators);
  char* noteDur = strtok(NULL, separators);
  unsigned int i = 0;
  while (noteFreq != NULL && noteDur != NULL) {
    loadedTune.push_back( new Note(atoi(noteFreq), atoi(noteDur)) );
    noteFreq = strtok(NULL, separators);
    noteDur = strtok(NULL, separators);
  }
  loadedTune.push_back( NULL ); 
}



