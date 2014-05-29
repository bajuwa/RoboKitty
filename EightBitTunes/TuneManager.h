#ifndef TUNEMANAGER_H
#define TUNEMANAGER_H

#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <vector>
#include "Note.h"
#include "Arduino.h"

using std::vector;

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
    void playTune(bool loopTune);
    void loadSound(SOUNDS sound);
  private:
    vector<Note*> tune;
    char** sounds;
    void playNote(int freq, int dur);
};

#endif



