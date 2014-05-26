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

class TuneManager {
  public:
    TuneManager(); 
    void loadTune(char tune[]);
    void playTune(bool loopTune);
  private:
    vector<Note*> tune;
    void playNote(int freq, int dur);
};

#endif



