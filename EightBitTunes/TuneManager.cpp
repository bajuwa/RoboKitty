#include <SD.h>
#include <MemoryFree.h>
#include "TuneManager.h"

// HARDWARE CONSTANTS
// pin of speaker/pezo
int const PIN_PEZO = 8;
int const PIN_CS = 4;
int const PIN_HARDWARE_SS = 10;

// Sound management...
File tuneFile;
File root;
int const MAX_CHAR_PER_LINE = 25;

int readNoteIndex = 0;
int writeNoteIndex = 0;
int tuneFreq[MAX_NOTE_BUFFER];
int tuneDur[MAX_NOTE_BUFFER];

// Timing
long previousMillis = 0;
long interval = 0;

TuneManager::TuneManager() {
  Serial.println(F("Initializing TuneManager"));
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output
  // or the SD library functions will not work.
  pinMode(PIN_HARDWARE_SS, OUTPUT);
  
  previousMillis = millis();
  
  if (!SD.begin(PIN_CS)) {
    Serial.println(F("SD Card Initialization Failed"));
  } else {
    Serial.println(F("SD Card Initialized Successfully"));
  }
  
  // Open the root folder so we can rely on 'openNextFile' for our playlist 'shuffle'
  root = SD.open("/");
}

void getNextLine(File file, char nextLine[]) {
  char inputChar = file.read();
  byte j = 0;
  while (inputChar != EOF && inputChar != '\n' && j < MAX_CHAR_PER_LINE-1) {
    nextLine[j++] = inputChar;    
    inputChar = file.read();
  }
  nextLine[j] = NULL;
}

void TuneManager::addNotesToTune(byte numOfNotesToAdd) {
  char separators[] = " -";
  char* noteFreq = "";
  char* noteDur = "";
  for (int i=0; i<numOfNotesToAdd; i++) {
    // Immediately abort loading more notes if we have run out of space in the buffer
    if ((writeNoteIndex+1)%MAX_NOTE_BUFFER == readNoteIndex) return;
    
    // Get the next freq and duration
    if (tuneFile.available()) {
      // Get our next note from the file
      char nextLine[MAX_CHAR_PER_LINE];
      getNextLine(tuneFile, nextLine);
      
      // Decode the note
      noteFreq = strtok(nextLine, separators);
      noteDur = strtok(NULL, separators);
      if (noteFreq != NULL && noteDur != NULL) {
        // Otherwise, add our decoded note to the ongoing tune
        tuneFreq[writeNoteIndex] = atoi(noteFreq);
        tuneDur[writeNoteIndex] = atoi(noteDur);
        // Increment the index of where we are writing our notes
        writeNoteIndex = (writeNoteIndex+1)%MAX_NOTE_BUFFER;
      }
    } else {
      // If we have reached the end of the song, the file will no longer be available
      // Close it and return it to null so that a new tune file can be played
      Serial.println(F("Reached end of tune, closing file"));
      tuneFile.close();
      break;
    }
  }
}

/*
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
*/

void TuneManager::playTunes() {
  // Make sure we have a song to play
  if (!tuneFile) {
    // Proceed to the next song if it exists
    tuneFile = root.openNextFile();
    // If we've reached the end, then just stop for now
    if (!tuneFile) return;
    Serial.print(F("Loaded next tune file: "));
    Serial.println(tuneFile.name());
    addNotesToTune(MAX_NOTE_BUFFER);
  }
  
  // play the song by iterating over the notes at given intervals:
  unsigned long currentMillis = millis();
  if (readNoteIndex != writeNoteIndex && (currentMillis - previousMillis > interval) ) {
    previousMillis = currentMillis;   
  
    // If the current note isn't a 'rest' beat, 
    // then play the note for the alloted duration (milliseconds)
    tone(PIN_PEZO, tuneFreq[readNoteIndex], tuneDur[readNoteIndex]);
    
    // Set how long to wait until next note 
    // (namely the length of the currently playing note plus some rest time to distinguish between repeated notes)
    interval = tuneDur[readNoteIndex];
    
    // Remove the note we just played so it doesn't repeat
    readNoteIndex = (readNoteIndex+1)%MAX_NOTE_BUFFER;
  } else {
     // If we can't play a note yet, might as well buffer some of the upcoming notes
     //Serial.println("Not playing a note, so add to buffer");
     addNotesToTune(MIN_NOTE_BUFFER);
  } 
}

void TuneManager::loadSound(SOUNDS sound) {
  switch (sound) {
    case HAPPY:
      // Mario One-Up 
      //this->loadTune("330 150-392 150-587 150-494 150-523 150-698 150");
      break;
    case SAD:
      // Mario Death
      //this->loadTune("1047 50-1109 50-1175 50-0 450-247 150-698 150-0 150-698 150-698 200-659 200-587 200-523 150-330 150-0 150-330 150-262 150");
      break;
    case PANIC:
      //this->loadTune("1000 200-1300 200-1000 200-1300 200");
      break;
    case BORED:
      //this->loadTune("1000 200-1300 200-1000 200-1300 200");
      break;
  }
}



