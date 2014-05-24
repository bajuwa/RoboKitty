#ifndef NOTE_H
#define NOTE_H

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
