#ifndef TUNEMANAGER_H
#define TUNEMANAGER_H

#include "Arduino.h"

static const int MIN_NOTE_BUFFER = 15;
static const int MAX_NOTE_BUFFER = 100;

typedef enum {
  HAPPY,
  SAD,
  PANIC,
  BORED
} SOUNDS;
    
class TuneManager {
  public:
    TuneManager(); 
    void playTunes();
    void loadSound(SOUNDS sound);
  private:
    int tuneFreq[MAX_NOTE_BUFFER];
    int tuneDur[MAX_NOTE_BUFFER];
    char** sounds;
    void addNotesToTune(byte numOfNotesToAdd);
};

#endif



