#ifndef TUNEMANAGER_H
#define TUNEMANAGER_H

#include "Arduino.h"


// HARDWARE CONSTANTS
// pin of speaker/pezo
static const byte PIN_PEZO = 8;
static const byte PIN_CS = 4;
static const byte PIN_HARDWARE_SS = 10;

// Buffer Constants
static const byte MIN_NOTE_BUFFER = 15;
static const byte MAX_NOTE_BUFFER = 100;

class TuneManager {
  public:
    TuneManager(char tuneFolderPath[]); 
    void playTunes();
  private:
    void addNotesToTune(Stream* str, byte numOfNotesToAdd);
    void ensureFile(File* file, File* rootToLoadFrom);
};

#endif



