#ifndef NOTE_H
#define NOTE_H

#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>
#include <vector>

using std::vector;

class Note {
  public:
    Note(int freq, int dur); 
    int getFrequency();
    int getDuration();
  private:
    int frequency;
    int duration;
};

#endif




