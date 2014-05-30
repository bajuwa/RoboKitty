#include "TuneManager.h"

// HARDWARE CONSTANTS
// pin of speaker/pezo
int const PIN_PEZO = 8;

// Sound management...
char* encodedTune;
int MIN_NOTE_BUFFER = 5;
int MAX_NOTE_BUFFER = 10;
vector<Note*> tune;
int noteIndex = 0;

// Timing
long previousMillis = 0;
long interval = 0;

TuneManager::TuneManager() {
  previousMillis = millis();
  tune.push_back(NULL);
  Serial.println(tune.size());
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

int firstIndexOf(char* arr, char ch) {
  for (int i=0; i<strlen(arr); i++) {
    if (arr[i] == ch) return i;
  }
  return 0;
}

void addNotesToTune(char* encodedNotes) {
    char separators[] = " -";
    char* noteFreq = strtok(encodedNotes, separators);
    char* noteDur = strtok(NULL, separators);
    unsigned int i = 0;
    while (tune.size() < MAX_NOTE_BUFFER && noteFreq != NULL && noteDur != NULL) {
      //Serial.println("loading next note");
      // Add our decoded note to the ongoing tune
      tune.push_back( new Note(atoi(noteFreq), atoi(noteDur)) );
      // Remove the encoded note from the encoded tune
      int numOfRemovableChars = firstIndexOf(encodedNotes, '-')+1;
      memcpy( encodedNotes, encodedNotes + numOfRemovableChars, (strlen(encodedNotes) - numOfRemovableChars) * sizeof(char) );
      // Get the next freq and duration
      noteFreq = strtok(encodedNotes, separators);
      noteDur = strtok(NULL, separators);
    }
    //Serial.println("done loading notes into buffer");
    // Add a null terminator if this is the last note
    if (strlen(encodedNotes) == 0 && tune[tune.size()] != NULL) {
      Serial.println("adding null terminator");
      tune.push_back( NULL );
    }  
}

void TuneManager::playTune(bool loopTune) {
  // Check to see if we have a sufficient number of notes loaded from the encoded tune
  if (tune.size() < MIN_NOTE_BUFFER && strlen(encodedTune) > 0) {
    addNotesToTune(encodedTune);
  }
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
  encodedTune = tuneToLoad;
  tune.clear();
  addNotesToTune(encodedTune);
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



