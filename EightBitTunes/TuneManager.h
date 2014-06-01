#ifndef TUNEMANAGER_H
#define TUNEMANAGER_H

#include "Arduino.h"

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
    int tuneFreq[];
    int tuneDur[];
    char** sounds;
    void addNotesToTune(int numOfNotesToAdd);
};

#endif



