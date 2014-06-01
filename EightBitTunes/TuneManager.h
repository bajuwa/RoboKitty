#ifndef TUNEMANAGER_H
#define TUNEMANAGER_H

#include "Arduino.h"

static const int MAX_NOTE_BUFFER = 25;

typedef enum {
  HAPPY,
  SAD,
  PANIC,
  BORED
} SOUNDS;
    
class TuneManager {
  public:
    TuneManager(); 
    void loadTune(char tune[]);
    void playTune();
    void loadSound(SOUNDS sound);
  private:
    int tuneFreq[MAX_NOTE_BUFFER];
    int tuneDur[MAX_NOTE_BUFFER];
    char** sounds;
    void addNotesToTune(int numOfNotesToAdd);
};

#endif



