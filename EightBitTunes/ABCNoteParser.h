#ifndef ABCNOTEPARSER_H
#define ABCNOTEPARSER_H

#include "Arduino.h"

class ABCNoteParser {
  public:
    ABCNoteParser(); 
    void reset();
    void getNextNote(Stream* str, int* freq, int* dur);
  private:
    int getIntegerFromStream(Stream* str, char* previewedChar);
    double delayTimeInMilliseconds(double noteLength, float bpm);
};

#endif



