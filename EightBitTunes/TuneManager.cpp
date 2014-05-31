#include "TuneManager.h"

// HARDWARE CONSTANTS
// pin of speaker/pezo
int const PIN_PEZO = 8;

// Sound management...
char* encodedTune;
int MIN_NOTE_BUFFER = 5;
int MAX_NOTE_BUFFER = 10;
list<Note*> tune;

// Timing
long previousMillis = 0;
long interval = 0;

TuneManager::TuneManager() {
  previousMillis = millis();
  tune.push_back(new Note(0,0));
}

int firstIndexOf(char* arr, char ch) {
  for (int i=0; i<strlen(arr); i++) {
    if (arr[i] == ch) return i;
  }
  return 0;
}

void TuneManager::addNotesToTune(char* encodedNotes) {
    char separators[] = " -";
    char* noteFreq = strtok(NULL, separators);
    char* noteDur = strtok(NULL, separators);
    unsigned int i = 0;
    while (tune.size() < MAX_NOTE_BUFFER && noteFreq != NULL && noteDur != NULL) {
      // Add our decoded note to the ongoing tune
      tune.push_back( new Note(atoi(noteFreq), atoi(noteDur)) );
      // Remove the encoded note from the encoded tune
      int numOfRemovableChars = firstIndexOf(encodedNotes, '-');
      // Get the next freq and duration
      noteFreq = strtok(NULL, separators);
      noteDur = strtok(NULL, separators);
    }
    
    // Add blanknotes if the end of song has been reached
    if (noteFreq == NULL || noteDur == NULL) {
      while (tune.size() < MAX_NOTE_BUFFER) {
        tune.push_back( new Note(0,0) );
      }
    }
}

void printList(list<Note*> lst) {
  for (list<Note*>::const_iterator iterator = lst.begin(), end = lst.end(); iterator != end; ++iterator) {
      Serial.print((*iterator)->getFrequency());
      Serial.print(", ");
  }
  Serial.println("");
}

//BUG: this is constantly calling addNotesToTune
void TuneManager::playTune() {
  // A note with 0 duration marks the end of the song
  if (tune.front()->getDuration() == 0) return;
  
  // TEST
  printList(tune);
  
  // Check to see if we have a sufficient number of notes loaded from the encoded tune
  if (tune.size() < MIN_NOTE_BUFFER) {
    addNotesToTune(encodedTune);
  }
  
  // play the song by iterating over the notes at given intervals:
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;   
  
    // If the current note isn't a 'rest' beat, 
    // then play the note for the alloted duration (milliseconds)
    tone(PIN_PEZO, tune.front()->getFrequency(), tune.front()->getDuration());
    
    // Set how long to wait until next note 
    // (namely the length of the currently playing note)
    interval = tune.front()->getDuration();
    
    // Remove the note we just played so it doesn't repeat
    tune.pop_front();
  }  
}

void TuneManager::loadTune(char tuneToLoad[]) {
  encodedTune = tuneToLoad;
  strtok(encodedTune, "-");
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



