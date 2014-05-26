#include "Note.h"

Note::Note(int freq, int dur) {
 frequency = freq;
 duration = dur; 
}

int Note::getFrequency() {
 return frequency; 
}

int Note::getDuration() {
 return duration; 
}




