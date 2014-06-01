#include <MemoryFree.h>
#include "TuneManager.h"

// HARDWARE CONSTANTS
// pin of speaker/pezo
int const PIN_PEZO = 8;

// Sound management...
char* encodedTune;

int currentNoteIndex = 0;
int loadableNoteIndex = 0;
int tuneFreq[MAX_NOTE_BUFFER];
int tuneDur[MAX_NOTE_BUFFER];

// Timing
long previousMillis = 0;
long interval = 0;

TuneManager::TuneManager() {
  previousMillis = millis();
}

void TuneManager::addNotesToTune(int numOfNotesToAdd) {
    char separators[] = " -";
    char* noteFreq = "";
    char* noteDur = "";
    for (int i=0; i<numOfNotesToAdd; i++) {
      // Immediately abort loading more notes if we have run out of space in the buffer
      if ((loadableNoteIndex+1)%MAX_NOTE_BUFFER == currentNoteIndex) return;
      
      // Get the next freq and duration
      noteFreq = strtok(NULL, separators);
      noteDur = strtok(NULL, separators);
      
      // If we have reached the end of the encoded song, strtok will return nulls
      if (noteFreq != NULL && noteDur != NULL) {
        // Otherwise, add our decoded note to the ongoing tune
        tuneFreq[loadableNoteIndex] = atoi(noteFreq);
        tuneDur[loadableNoteIndex] = atoi(noteDur);
        
        loadableNoteIndex = (loadableNoteIndex+1)%MAX_NOTE_BUFFER;
      }
      
    }
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void TuneManager::playTune() {
  // A note with 0 duration marks the end of the song
  if (currentNoteIndex == loadableNoteIndex) return;
  Serial.println(freeRam());
  
  // play the song by iterating over the notes at given intervals:
  unsigned long currentMillis = millis();
  if ((currentMillis - previousMillis > interval) ) {
    previousMillis = currentMillis;   
  
    // If the current note isn't a 'rest' beat, 
    // then play the note for the alloted duration (milliseconds)
    tone(PIN_PEZO, tuneFreq[currentNoteIndex], tuneDur[currentNoteIndex]);
    
    // Set how long to wait until next note 
    // (namely the length of the currently playing note)
    interval = tuneDur[currentNoteIndex];
    
    // Remove the note we just played so it doesn't repeat
    currentNoteIndex = (currentNoteIndex+1)%MAX_NOTE_BUFFER;
  } else {
     // If we can't play a note yet, might as well buffer some of the upcoming notes
     //Serial.println("Not playing a note, so add to buffer");
     addNotesToTune(1);
  } 
}

void TuneManager::loadTune(char tuneToLoad[]) {
  encodedTune = tuneToLoad;
  strtok(encodedTune, "-");
  
  // In order to 'clear' the last song, reset the indexes and add notes again
  loadableNoteIndex = 0;
  currentNoteIndex = 0;
  
  addNotesToTune(MAX_NOTE_BUFFER);
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



