#ifndef TUNEMANAGER_H
#define TUNEMANAGER_H

#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <list>
#include "Note.h"
#include "Arduino.h"

using std::list;

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
    list<Note*> tune;
    char** sounds;
    void addNotesToTune(char* encodedNotes, int numOfNotesToAdd);
};

#endif



